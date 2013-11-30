#include "PersistImpSQL.h"

bool PersistImpSQL::connectToDB(QString connection, QString path, QString DBType){
    db = QSqlDatabase::addDatabase(DBType,connection);
    db.setHostName("localhost");
    db.setDatabaseName(path);
    return this->connected = db.open();
}

void PersistImpSQL::disconnect(){
    this->db.close();
}

bool PersistImpSQL::restore(QString path){
    QFile schemaFile(path);
    schemaFile.open(QFile::ReadOnly);
    QStringList schemaTableList = QString(schemaFile.readAll()).split(";");
    foreach(const QString schemaTable, schemaTableList){
        if(!schemaTable.trimmed().isEmpty())
            db.exec(schemaTable);
    }
    schemaFile.close();
    return true;
}

void PersistImpSQL::fixNull(QString &s){if (s.isNull()) s = QString("");}

bool PersistImpSQL::verifyUser(QString name, QString &role){
    QSqlQuery query(db);
    QString queryString;
    queryString = QString("SELECT * FROM USERS WHERE name = '%1'").arg(name);
    if (!query.exec(queryString)) return false;

    role = QString();
    int numRows = 0;

    if (db.driver()->hasFeature(QSqlDriver::QuerySize)) {
        numRows = query.size();
    } else {
        query.last();
        numRows = query.at() + 1;
    }
    if (numRows == 0 || numRows == -1) return false;

    query.seek(-1);
    while(query.next()){
        role = query.value(1).toString();
    }
    return true;
}

bool PersistImpSQL::manageTask(QString user, QString action, Task task){
    if(action.toLower().contains("create"))
        return this->createTask(user,task.getTaID(),task.getCourseID(),task.getDescription(),task.getStartDate().toString(Qt::ISODate),task.getDueDate().toString(Qt::ISODate));

    if(action.toLower().contains("edit"))
        return this->editTask(task.getId().toInt(),task.getDescription(),task.getStartDate().toString(Qt::ISODate),task.getDueDate().toString(Qt::ISODate));

    if(action.toLower().contains("delete"))
        return this->deleteTask(task.getId().toInt());
    return false;
}

QString PersistImpSQL::getUserKey(QString name){
    QSqlQuery query(db);
    QString queryString;
    queryString = QString("SELECT * FROM USERS WHERE name = '%1'").arg(name);
    if (!query.exec(queryString)) return NULL;
    while(query.next()){
        return query.value(0).toString();
    }
    return NULL;
}

QString PersistImpSQL::getCourseKey(QString term, QString title, INT num){
    QSqlQuery query(db);
    QString queryString;
    queryString = QString("SELECT * FROM COURSES WHERE title = '%1' AND term = '%2' AND num = %3")\
            .arg(title).arg(term).arg(num);
    if (!query.exec(queryString)) return NULL;
    while(query.next()){
        return query.value(0).toString();
    }
    return NULL;
}

QString PersistImpSQL::getInstructor(QString courseKey){
    QSqlQuery query(db);
    QString queryString;
    queryString = QString("SELECT * from COURSES WHERE name = '%1'").arg(courseKey);
    if (!query.exec(queryString)) return QString("");
    while(query.next()){
        return query.value(4).toString();
    }
    return QString("");
}

bool PersistImpSQL::createTask(QString user, QString TAKey, QString courseKey, QString desc, QString start, QString end){
    /* first verify the user has permission */
    if(user.compare(this->getInstructor(courseKey)) != 0) return false;

    /* second check the ta is assigned to the course */
    QList<TA> *list;
    if (!this->getTAsForCourse(courseKey,list)){
        delete list;
        return false;
    }
    bool contains = false;
    for(int i=0;i<list->size();i++){
        if (TA(list->at(i)).getId().compare(TAKey) == 0){
            contains = true;
            break;
        }
    }
    if(!contains){
        delete list;
        return false;
    }

    /*  everything goes fine, create */
    QSqlQuery query(db);
    QString queryString;
    queryString = QString("INSERT INTO TASKS ").\
            append(QString("(TAName, CourseName, description, startDate, endDate, rating, feedback)")).\
            append(QString("VALUES ('%1','%2','%3','%4','%5',%6 ,%7)").\
            arg(TAKey).arg(courseKey).arg(desc).arg(start).arg(end).arg("NULL").arg("NULL"));

   return query.exec(queryString);
}

bool PersistImpSQL::editTask(INT TaskID, QString desc, QString start, QString end){
    QSqlQuery query(db);
    QString queryString;

    QList<QString> updates;
    if (!desc.isEmpty()) updates.append(QString(" description = '%1' ").arg(desc));
    if (!start.isEmpty()) updates.append(QString(" startDate = '%1' ").arg(start));
    if (!end.isEmpty()) updates.append(QString(" endDate = '%1' ").arg(end));
    if (updates.isEmpty()) return false;

    QString update = "";
    for(int i=0; i< updates.size(); ++i){
        if(i > 0) update.append(" , ");
        update.append(updates.at(i));
    }

    queryString = QString("UPDATE TASKS SET").append(update).append(QString("WHERE id = %1").arg(TaskID));
    return query.exec(queryString);
}

bool PersistImpSQL::deleteTask(INT TaskID){
    QSqlQuery query(db);
    QString queryString;

    queryString = QString("SELECT count(*) FROM TASKS WHERE id = %1").arg(TaskID);
    if (!query.exec(queryString)) return false;

    query.next();
    if (!query.value(0).toInt()) return false;
    query.clear();

    queryString = QString("DELETE FROM TASKS WHERE id = %1").arg(TaskID);
    return query.exec(queryString);
}

bool PersistImpSQL::getCoursesTeaching(QString Instrcutor, QString term, QList<Course> *&list){
    QString teacher = this->getUserKey(Instrcutor);
    if (teacher.isNull()) return false;

    QSqlQuery query(db);
    QString queryString;
    queryString = QString("SELECT * FROM COURSES WHERE InsName = '%1' AND term = '%2'").arg(teacher).arg(term);
    if (!query.exec(queryString)) return false;

    list = new QList<Course>();
    while(query.next()){
        QString name = query.value(0).toString();
        QString title = query.value(1).toString();
        QString term = query.value(2).toString();
        INT num = query.value(3).toInt();
        QString InsName = query.value(4).toString();
        list->append(Course(name,title,QString::number(num),term,InsName));
    }
    return true;
}


bool PersistImpSQL::getTAsForCourse(QString courseKey, QList<TA> *&list){
    QSqlQuery query(db);
    QString queryString;
    queryString = QString("SELECT TAName,role from USERS join TACOURSES on USERS.name = TACOURSES.TAName WHERE CourseName = '%1'").\
            arg(courseKey);
    if (!query.exec(queryString)) return false;

    list = new QList<TA>();
    while(query.next()){
        QString taName = query.value(0).toString();
        list->append(TA(taName));
    }
    return true;
}


bool PersistImpSQL::getTasksForTA(QString courseKey, QString TAKey, QList<Task> *&list){
    QSqlQuery query(db);
    QString queryString;
    if(courseKey.length() > 10){
        queryString = QString("SELECT * FROM TASKS WHERE TAName = '%1' AND CourseName = '%2'").\
                arg(TAKey).arg(courseKey);
    }
    else{
        queryString = QString("SELECT * FROM TASKS WHERE TAName = '%1'").\
                arg(TAKey);
    }
    if (!query.exec(queryString)) return false;

    list = new QList<Task>();
    while(query.next()){
        QString TA = query.value(0).toString();
        QString course = query.value(1).toString();
        INT task = query.value(2).toInt();
        QString desc = query.value(3).toString();
        this->fixNull(desc);

        QString start = query.value(4).toString();
        QString end = query.value(5).toString();
        this->fixNull(start);
        this->fixNull(end);

        QStringList startl = this->splitDate(start);
        QStringList endl = this->splitDate(end);
        QDate s;
        if (!start.isEmpty())
            s = QDate(startl.at(0).toInt(),startl.at(1).toInt(),startl.at(2).toInt());
        QDate e;
        if (!end.isEmpty())
            e = QDate(endl.at(0).toInt(),endl.at(1).toInt(),endl.at(2).toInt());

        INT rating;
        rating = query.value(6).isNull() ? -1 : query.value(6).toInt();

        QString feedback = query.value(7).toString();
        this->fixNull(feedback);

        list->append(Task(TA,course,desc,s,e,QString::number(task),rating,feedback));
    }
    return true;
}

QStringList PersistImpSQL::getSemesters(QString user){
    QSqlQuery query(db);
    QString queryString;
    QStringList rv;
    queryString = QString("SELECT DISTINCT term FROM COURSES WHERE InsName = '%1'").\
            arg(user);
    if (!query.exec(queryString)) {rv.append("-1"); return rv;}

    while(query.next()){
        rv.append(query.value(0).toString());
    }
    return rv;
}

bool PersistImpSQL::getTask(QString taskID, Task *&rv){
    QSqlQuery query(db);
    QString queryString;
    queryString = QString("SELECT * FROM TASKS WHERE id = %1").\
            arg(taskID);
    if (!query.exec(queryString)) return false;

    while(query.next()){
        QString TA = query.value(0).toString();
        QString course = query.value(1).toString();
        INT task = query.value(2).toInt();
        QString desc = query.value(3).toString();
        this->fixNull(desc);

        QString start = query.value(4).toString();
        QString end = query.value(5).toString();
        this->fixNull(start);
        this->fixNull(end);

        QStringList startl = this->splitDate(start);
        QStringList endl = this->splitDate(end);
        QDate s;
        if (!start.isEmpty())
            s = QDate(startl.at(0).toInt(),startl.at(1).toInt(),startl.at(2).toInt());
        QDate e;
        if (!end.isEmpty())
            e = QDate(endl.at(0).toInt(),endl.at(1).toInt(),endl.at(2).toInt());

        INT rating;
        rating = query.value(6).isNull() ? -1 : query.value(6).toInt();

        QString feedback = query.value(7).toString();
        this->fixNull(feedback);

        rv = new Task(TA,course,desc,s,e,QString::number(task),rating,feedback);
    }
    return true;
}

bool PersistImpSQL::enterEvaluation(INT TaskID, INT rating, QString feedback){
    QSqlQuery query(db);
    QString queryString;
    Task *t = NULL;
    if(!this->getTask(QString::number(TaskID),t) || t == NULL) return false;

    QList<QString> updates;
    if (rating != -1) updates.append(QString(" rating = %1 ").arg(rating));
    if (!feedback.isEmpty()) updates.append(QString(" feedback = '%1' ").arg(feedback));
    if (updates.isEmpty()) return false;

    QString update = "";
    for(int i=0; i< updates.size(); ++i){
        if(i > 0) update.append(" , ");
        update.append(updates.at(i));
    }

    queryString = QString("UPDATE TASKS SET").append(update).append(QString("WHERE id = %1").arg(TaskID));
    return query.exec(queryString);
}
