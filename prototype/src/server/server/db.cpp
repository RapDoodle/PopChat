#pragma comment(lib, "libmysql.lib")
#include <stdlib.h>
#include <iostream>
#include <string>

#include "db.h"
#include "utils.h"
#include ".\MySQL\include\mysql.h"

/* Database connection */
MYSQL dbConn;
MYSQL_RES* queryResult;

int dbInit()
{
	if (mysql_init(&dbConn) == NULL) {
		consoleLog("Failed to initialize MySQL connection");
		return -1;
	} else {
		consoleLog("Sucessfully initialized MySQL connection");
	}

	if (mysql_real_connect(&dbConn, "localhost", "root", "", "pop", 3306, NULL, 0) == NULL) {
		consoleLog("Failed to connect to MySQL server");
		return -1;
	} else {
		consoleLog("Sucessfully connected to MySQL server");
	}

	string initQuries[2] = {
		"CREATE TABLE IF NOT EXISTS session(session_id INT AUTO_INCREMENT, created_at DATETIME DEFAULT CURRENT_TIMESTAMP, PRIMARY KEY (session_id));",
		"CREATE TABLE IF NOT EXISTS message(message_id INT AUTO_INCREMENT, session_id INT, created_at DATETIME DEFAULT CURRENT_TIMESTAMP, content VARCHAR(140), PRIMARY KEY (message_id), FOREIGN KEY (session_id) REFERENCES `session`(session_id));"
	};

	for (int i = 0; i < 2; i++) {
		if (executeQuery(initQuries[i], false) != 0) {
			consoleLog("Error verifying database");
			return -1;
		}
	}
	consoleLog("Database verified");

	return 0;
}

int executeQuery(string query, bool isSelect)
{
	int queryRet = mysql_real_query(&dbConn, query.c_str(), (unsigned int)strlen(query.c_str()));
	if (isSelect) {
		queryResult = mysql_store_result(&dbConn);
	}
	return queryRet;
}

int createSession()
{
	/* Return value: the session id */
	executeQuery("INSERT INTO session VALUES ();", false);
	executeQuery("SELECT LAST_INSERT_ID();", true);
	int sessionId = safeToInt(mysql_fetch_row(queryResult)[0]);
	return sessionId;
}

int saveMessage(int session_id, string msg)
{
	string query = "INSERT INTO message (session_id, content) VALUES (" + to_string(session_id) + ", \"" + msg + "\");";
	cout << query << endl;
	return executeQuery(query, false);
}