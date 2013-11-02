-- COMP 3004
-- Team: KungFu Panda
-- Project: TAEval prototype 1
-- Database: tables and dummy entries

-- SQLite related settings
.header on
.mode column

-- drop tables if exists (restore to original status)
DROP TABLE if EXISTS COURSES;
DROP TABLE if EXISTS USERS;
DROP TABLE if EXISTS TASKS;
-----------------------------------------------------
DROP TABLE if EXISTS TACOURSES;

-- Create tables
CREATE TABLE USERS(
	name TEXT PRIMARY KEY NOT NULL,
	role TEXT
);
CREATE TABLE COURSES (
	name TEXT PRIMARY KEY NOT NULL,
	title TEXT,
	term TEXT,
	num INTEGER,
	InsName TEXT -- foreign key: USERS(name)
);
CREATE TABLE TASKS (
	TAName TEXT, -- foreign key: USERS(name)
	CourseName TEXT, -- foreign key: COURSES(name)
	id INTEGER PRIMARY KEY,
	description TEXT,
	startDate TEXT,
	endDate TEXT,
	rating INTEGER,
	feedback TEXT
);
CREATE TABLE TACOURSES(
	TAName TEXT, -- foreign key: USERS(name)
	CourseName TEXT, -- foreign key: COURSES(name)
	PRIMARY KEY (TAName,CourseName)
);

-- add some data
-- USERS
INSERT INTO USERS (name, role) VAlUES ('Christine','Instructor');
INSERT INTO USERS (name, role) VAlUES ('Louis','Instructor');
INSERT INTO USERS (name, role) VAlUES ('Pat','Instructor');
INSERT INTO USERS (name, role) VAlUES ('Peter','Instructor');
INSERT INTO USERS (name, role) VAlUES ('Timmy','Admin');
INSERT INTO USERS (name, role) VAlUES ('Matilda','Admin');
INSERT INTO USERS (name, role) VAlUES ('Dunni','TA');
INSERT INTO USERS (name, role) VAlUES ('Abdallah','TA');
INSERT INTO USERS (name, role) VAlUES ('Siyang','TA');
INSERT INTO USERS (name, role) VAlUES ('Nathan','TA');

-- COURSES
INSERT INTO COURSES (name, title, term, num, InsName) 
	VALUES ('COMP2404W2013', 'COMP','W2013', 2404, 'Christine');
INSERT INTO COURSES (name, title, term, num, InsName) 
	VALUES ('COMP3004F2013', 'COMP','F2013', 3004, 'Christine');
INSERT INTO COURSES (name, title, term, num, InsName) 
	VALUES ('COMP2401F2013', 'COMP','F2013', 2401, 'Christine');
INSERT INTO COURSES (name, title, term, num, InsName) 
	VALUES ('COMP3005F2013', 'COMP','F2013', 3005, 'Louis');
INSERT INTO COURSES (name, title, term, num, InsName) 
	VALUES ('BUSI2701F2012', 'BUSI','F2012', 2701, 'Peter');
INSERT INTO COURSES (name, title, term, num, InsName) 
	VALUES ('COMP3004F2012', 'COMP','F2012', 3004, 'Christine');
INSERT INTO COURSES (name, title, term, num, InsName) 
	VALUES ('COMP2402F2013', 'COMP','F2013', 2402, 'Pat');
INSERT INTO COURSES (name, title, term, num, InsName) 
	VALUES ('COMP1405F2013', 'COMP','F2013', 1405, 'Pat');

-- TA_COURSES
INSERT INTO TACOURSES (TAName, CourseName) VALUES ('Nathan','COMP2402F2013');
INSERT INTO TACOURSES (TAName, CourseName) VALUES ('Dunni','COMP1405F2013');
INSERT INTO TACOURSES (TAName, CourseName) VALUES ('Siyang','COMP1405F2013');
----------------------------------------------------------------------------------
INSERT INTO TACOURSES (TAName, CourseName) VALUES ('Abdallah','COMP3004F2013');
INSERT INTO TACOURSES (TAName, CourseName) VALUES ('Abdallah','COMP3004F2012');

-- TASKS
INSERT INTO TASKS (TAName, CourseName, description, startDate, endDate, rating, feedback) 
	VALUES ('Nathan','COMP2402F2013','Grade assignments','2013-09-05','2013-12-01', NULL, NULL);
INSERT INTO TASKS (TAName, CourseName, description, startDate, endDate, rating, feedback) 
	VALUES ('Dunni','COMP1405F2013','Grade assignments','2013-10-01','2013-12-10', NULL, NULL);
INSERT INTO TASKS (TAName, CourseName, description, startDate, endDate, rating, feedback) 
	VALUES ('Dunni','COMP1405F2013','Online assistance','2013-10-01','2013-12-10', NULL, NULL);
INSERT INTO TASKS (TAName, CourseName, description, startDate, endDate, rating, feedback) 
	VALUES ('Siyang','COMP1405F2013','Grade assignments','2013-09-01','2013-10-29', NULL, NULL);
INSERT INTO TASKS (TAName, CourseName, description, startDate, endDate, rating, feedback) 
	VALUES ('Siyang','COMP1405F2013','Weekly Meeting','2013-09-01','2013-10-29', NULL, NULL);
INSERT INTO TASKS (TAName, CourseName, description, startDate, endDate, rating, feedback) 
	VALUES ('Abdallah','COMP3004F2013','Grade assignments','2013-09-01','2013-12-19', NULL, NULL);
INSERT INTO TASKS (TAName, CourseName, description, startDate, endDate, rating, feedback) 
	VALUES ('Abdallah','COMP3004F2012',NULL,NULL,NULL, NULL, NULL);



-- test codes
-- Select * from USERS;
-- Select * from COURSES;
-- Select * from TACOURSES;
-- Select * from TASKS;
-- INSERT INTO TASKS (TAName, CourseName, description, startDate, endDate, rating, feedback) VALUES ('Nathan','COMP2402F2013','Proctor exams','2013-09-05','2013-12-02', NULL, NULL);
-- UPDATE TASKS SET description = 'Research' where TAName = 'Nathan' AND CourseName = 'COMP2402F2013';
