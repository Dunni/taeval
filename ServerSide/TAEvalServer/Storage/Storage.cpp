#include "Storage.h"

bool Storage::connectToDB(QString connection, QString path, QString DBType){
    db = QSqlDatabase::addDatabase(DBType,connection);
    db.setHostName("localhost");
    db.setDatabaseName(path);
    return this->connected = db.open();
}

void Storage::disconnect(){
    this->db.close();
}

#ifdef DEBUG
bool Storage::dumpDB(){
    bool ok = false;
    QSqlQuery query(db);
    QString queryString;

    /* dump Table USERS */
    queryString = "SELECT * FROM USERS";
    ok =  query.exec(queryString);
    if (!ok) return false;

    qDebug() << QString("\n###USERS###");
    qDebug() << QString("UserName\tRole");
    while(query.next()){
        QString user = query.value(0).toString();
        QString role = query.value(1).toString();
        qDebug() << QString("%1\t%2").arg(user).arg(role);
    }

    /* dump Table COURSES */
    queryString = "SELECT * FROM COURSES";
    ok =  query.exec(queryString);
    if (!ok) return false;
    qDebug() << QString("\n###COURSES###");
    qDebug() << QString("CourseName\tTitle\tTerm\tNumber\tInstructorName");
    while(query.next()){
        QString name = query.value(0).toString();
        QString title = query.value(1).toString();
        QString term = query.value(2).toString();
        INT num = query.value(3).toInt();
        QString InsName = query.value(4).toString();
        qDebug() << QString("%1\t%2\t%3\t%4\t%5").arg(name).arg(title).arg(term).arg(num).arg(InsName);
    }

    /* dump Table TACOURSES */
    queryString = "SELECT * FROM TACOURSES";
    ok =  query.exec(queryString);
    if (!ok) return false;
    qDebug() << QString("\n###TACOURSES###");
    qDebug() << QString("TAName\tCourseName");
    while(query.next()){
        QString TA = query.value(0).toString();
        QString course = query.value(1).toString();
        qDebug() << QString("%1\t%2").arg(TA).arg(course);
    }

    /* dump Table TASKS */
    queryString = "SELECT * FROM TASKS";
    ok =  query.exec(queryString);
    if (!ok) return false;
    qDebug() << QString("\n###TASKS###");
    qDebug() << QString("TAName\tCourseName\t\tTaskID\tDescription\tstartDate\tendDate\trating\tfeedback");
    while(query.next()){
        QString TA = query.value(0).toString();
        QString course = query.value(1).toString();
        INT task = query.value(2).toInt();
        QString desc = query.value(3).toString();
        QString start = query.value(4).toString();
        QString end = query.value(5).toString();

        INT rating;
        rating = query.value(6).isNull() ? -1 : query.value(6).toInt();

        QString feedback;
        feedback = query.value(7).isNull() ? "N/A" : query.value(7).toString();

        qDebug() << QString("%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8").arg(TA).arg(course).arg(task).arg(desc).arg(start).arg(end).arg(rating).arg(feedback);
    }

    return ok;
}
#endif

bool Storage::manageTask(QString action, Task task){
    if(action.toLower().contains("create"))
        return this->createTask(task.getTaID(),task.getCourseID(),task.getDescription(),task.getStartDate().toString(),task.getDueDate().toString());

    if(action.toLower().contains("edit"))
        return this->editTask(task.getId().toInt(),task.getDescription(),task.getStartDate().toString(),task.getDueDate().toString());

    if(action.toLower().contains("delete"))
        return this->deleteTask(task.getId().toInt());
    return false;
}

QString Storage::getUserKey(QString name){
    QSqlQuery query(db);
    QString queryString;
    queryString = QString("SELECT * FROM USERS WHERE name = '%1'").arg(name);
    if (!query.exec(queryString)) return NULL;
    while(query.next()){
        return query.value(0).toString();
    }
    return NULL;
}

QString Storage::getCourseKey(QString term, QString title, INT num){
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

bool Storage::createTask(QString TAKey, QString courseKey, QString desc, QString start, QString end){
    QSqlQuery query(db);
    QString queryString;
    queryString = QString("INSERT INTO TASKS ").\
            append(QString("(TAName, CourseName, description, startDate, endDate, rating, feedback)")).\
            append(QString("VALUES ('%1','%2','%3','%4','%5',%6 ,%7)").\
            arg(TAKey).arg(courseKey).arg(desc).arg(start).arg(end).arg("NULL").arg("NULL"));

   return query.exec(queryString);
}

bool Storage::createTask(QString TAName, QString courseTitle, QString courseTerm, INT courseNum, \
                         QString desc, QString start, QString end){

    QString TAKey = this->getUserKey(TAName);
    if (TAKey.isNull()) return false;

    QString courseKey = this->getCourseKey(courseTerm, courseTitle, courseNum);
    if (courseKey.isNull()) return false;

    QSqlQuery query(db);
    QString queryString;
    queryString = QString("INSERT INTO TASKS ").\
            append(QString("(TAName, CourseName, description, startDate, endDate, rating, feedback)")).\
            append(QString("VALUES ('%1','%2','%3','%4','%5',%6 ,%7)").\
            arg(TAKey).arg(courseKey).arg(desc).arg(start).arg(end).arg("NULL").arg("NULL"));

   return query.exec(queryString);
}

bool Storage::editTask(INT TaskID, QString desc, QString start, QString end){
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

bool Storage::deleteTask(INT TaskID){
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

bool Storage::getCoursesTeaching(QString Instrcutor, QString term, QList<Course> *list){
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
        list->append(Course(name,title,QString(num),term,InsName));
    }
    return true;
}

bool Storage::viewCoursesTeaching(QString Instrcutor, QString term){
    QString teacher = this->getUserKey(Instrcutor);
    if (teacher.isNull()) return false;

    QSqlQuery query(db);
    QString queryString;
    queryString = QString("SELECT * FROM COURSES WHERE InsName = '%1' AND term = '%2'").arg(teacher).arg(term);
    if (!query.exec(queryString)) return false;

    qDebug() << QString("\n###CoursesTeaching###");
    qDebug() << QString("CourseName\tTitle\tTerm\tNumber\tInstructorName");
    while(query.next()){
        QString name = query.value(0).toString();
        QString title = query.value(1).toString();
        QString term = query.value(2).toString();
        INT num = query.value(3).toInt();
        QString InsName = query.value(4).toString();
        qDebug() << QString("%1\t%2\t%3\t%4\t%5").arg(name).arg(title).arg(term).arg(num).arg(InsName);
    }
    return true;
}

bool Storage::getTAsForCourse(QString courseKey, QList<TA> *list){
    QSqlQuery query(db);
    QString queryString;
    queryString = QString("SELECT TAName,role from USERS join TACOURSES on USERS.name = TACOURSES.TAName WHERE CourseName = '%1'").\
            arg(courseKey);
    if (!query.exec(queryString)) return false;

    list = new QList<TA>();
    while(query.next()){
        QString taName = query.value(0).toString();
//        QString role = query.value(0).toString();
        list->append(TA(taName));
    }
    return true;
}

bool Storage::viewTAsforCourse(QString term, QString title, INT num){
    QString courseKey = this->getCourseKey(term,title,num);
    if (courseKey.isNull()) return false;

    QSqlQuery query(db);
    QString queryString;
    queryString = QString("SELECT TAName from USERS join TACOURSES on USERS.name = TACOURSES.TAName WHERE CourseName = '%1'").\
            arg(courseKey);
    if (!query.exec(queryString)) return false;

    qDebug() << QString("\n###TAsFor###");
    qDebug() << QString("TAName");
    while(query.next()){
        QString TA = query.value(0).toString();
        qDebug() << QString("%1").arg(TA);
    }
    return true;
}


bool Storage::getTasksForTA(QString courseKey, QString TAKey, QList<Task> *list){
    QSqlQuery query(db);
    QString queryString;
    queryString = QString("SELECT * FROM TASKS WHERE TAName = '%1' AND CourseName = '%2'").\
            arg(TAKey).arg(courseKey);
    if (!query.exec(queryString)) return false;

    list = new QList<Task>();
    while(query.next()){
        QString TA = query.value(0).toString();
        QString course = query.value(1).toString();
        INT task = query.value(2).toInt();
        QString desc = query.value(3).toString();
        QString start = query.value(4).toString();
        QString end = query.value(5).toString();
        QStringList startl = this->splitDate(start);
        QStringList endl = this->splitDate(end);
        QDate s(startl.at(0).toInt(),startl.at(1).toInt(),startl.at(2).toInt());
        QDate e(endl.at(0).toInt(),endl.at(1).toInt(),endl.at(2).toInt());

        INT rating;
        rating = query.value(6).isNull() ? -1 : query.value(6).toInt();

        QString feedback;
        feedback = query.value(7).isNull() ? "NA" : query.value(7).toString();

        list->append(Task(TA,course,desc,s,e,QString(task),rating,feedback));
    }
    return true;
}

bool Storage::viewTasksforTA(QString term, QString title, INT num, QString TAKey){
    QString courseKey = this->getCourseKey(term,title,num);
    if (courseKey.isNull()) return false;

    QSqlQuery query(db);
    QString queryString;
    queryString = QString("SELECT * FROM TASKS WHERE TAName = '%1' AND CourseName = '%2'").\
            arg(TAKey).arg(courseKey);
    if (!query.exec(queryString)) return false;

    qDebug() << QString("\n###TASKS###");
    qDebug() << QString("TAName\tCourseName\t\tTaskID\tDescription\tstartDate\tendDate\trating\tfeedback");
    while(query.next()){
        QString TA = query.value(0).toString();
        QString course = query.value(1).toString();
        INT task = query.value(2).toInt();
        QString desc = query.value(3).toString();
        QString start = query.value(4).toString();
        QString end = query.value(5).toString();

        INT rating;
        rating = query.value(6).isNull() ? -1 : query.value(6).toInt();

        QString feedback;
        feedback = query.value(7).isNull() ? "N/A" : query.value(7).toString();

        qDebug() << QString("%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8").arg(TA).arg(course).arg(task).arg(desc).arg(start).arg(end).arg(rating).arg(feedback);
    }
    return true;
}

QStringList Storage::getSemesters(QString instrcutor){
    QSqlQuery query(db);
    QString queryString;
    QStringList rv;
    queryString = QString("SELECT DISTINCT term FROM COURSES WHERE InsName = '%1'").\
            arg(instrcutor);
    if (!query.exec(queryString)) {rv.append("-1"); return rv;}

    while(query.next()){
        rv.append(query.value(0).toString());
    }
    return rv;
}


bool Storage::enterEvaluation(INT TaskID, INT rating, QString feedback){
    QSqlQuery query(db);
    QString queryString;

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
