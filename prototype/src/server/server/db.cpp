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

	string initQuries[3] = {
		"CREATE TABLE IF NOT EXISTS chat_session(session_id INT AUTO_INCREMENT, created_at DATETIME DEFAULT CURRENT_TIMESTAMP, PRIMARY KEY (session_id));",
		"CREATE TABLE IF NOT EXISTS socket_session(session_id INT AUTO_INCREMENT, nickname VARCHAR(24), ip VARCHAR(39), created_at DATETIME DEFAULT CURRENT_TIMESTAMP, PRIMARY KEY (session_id));",
		"CREATE TABLE IF NOT EXISTS message(message_id INT AUTO_INCREMENT, chat_session_id INT, socket_session_id INT, created_at DATETIME DEFAULT CURRENT_TIMESTAMP, content VARCHAR(140), PRIMARY KEY (message_id), FOREIGN KEY (chat_session_id) REFERENCES chat_session(session_id), FOREIGN KEY (socket_session_id) REFERENCES socket_session(session_id));"
	};

	for (int i = 0; i < 3; i++) {
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

int createChatSession()
{
	/* Return value: the session id */
	executeQuery("INSERT INTO chat_session VALUES ();", false);
	executeQuery("SELECT LAST_INSERT_ID();", true);
	int sessionId = safeToInt(mysql_fetch_row(queryResult)[0]);
	return sessionId;
}

int createSocketSession(string ip, string nickName)
{
	/* Return value: the session id */
	string query = "INSERT INTO socket_session (nickname, ip) VALUES ('" + 
		safeToSQL(nickName) + "', '" + safeToSQL(ip) + "');";
	executeQuery(query, false);
	executeQuery("SELECT LAST_INSERT_ID();", true);
	int sessionId = safeToInt(mysql_fetch_row(queryResult)[0]);
	return sessionId;
}

int saveMessage(int chat_session_id, int socket_session_id, string msg)
{
	string query = "INSERT INTO message(chat_session_id, socket_session_id, content) VALUES (" +
		to_string(chat_session_id) + ",\'" + to_string(socket_session_id) + "\', \'" + safeToSQL(msg) + "\');";
	return executeQuery(query, false);
}