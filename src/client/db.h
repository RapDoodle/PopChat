#pragma once
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QList>
#include <string>

#include "MessageBox.h"
#include "utils.h"

#define NULL_STR ""

#define LIST_ALL 1
#define SEARCH 2

int openDB();
int insertMessage(string chatSessionId, string nickname, string content);
int createSession(string host, string nickname);

QList<struct ListEntry> findSession(int type, string content,
	string nickname, string host);
QList<struct ChatEntry> getChatData(int sessionId, string content,
	string nickname, string host);

struct ListEntry
{
	int sessionId;
	QString host;
	QString nickname;
	QString createdAt;
};

struct ChatEntry
{
	QString nickname;
	QString createdAt;
	QString content;
};