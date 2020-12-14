#pragma once
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <string>

#include "MessageBox.h"
#include "utils.h"

int openDB();
int insertMessage(int chatSessionId, string nickname, string content);
int createSession(string host_ip);