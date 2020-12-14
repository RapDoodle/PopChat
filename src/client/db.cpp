#include "db.h"

QSqlDatabase database;

int openDB()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("local_store.db");
    if (!database.open()) {
        msgBoxCritical("Unable to open the local database.");
        return -1;
    }

    /* Set up the tables if not exists */
    QSqlQuery query;
    std::string queries[2] = {
        "CREATE TABLE IF NOT EXISTS chat_session(session_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "host_ip VARCHAR(39), created_at DATETIME DEFAULT CURRENT_TIMESTAMP);",
        "CREATE TABLE IF NOT EXISTS message(message_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "chat_session_id INTEGER, nickname VARCHAR(24), created_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "content VARCHAR(140), FOREIGN KEY (chat_session_id) REFERENCES chat_session(session_id));"
    };

    for (int i = 0; i < 2; i++) {
        if (!query.exec(QString::fromStdString(queries[i]))) {
            msgBoxCritical("Error occurred on initialization.");
            return -1;
        }
    }
}

int insertMessage(int chatSessionId, string nickname, string content)
{
    QSqlQuery query;
    string qurtyStr = "INSERT INTO message(chat_session_id, nickname, content) VALUES "
        "(" + to_string(chatSessionId) + ", '" + safeToSQL(nickname) + "', '" + safeToSQL(content) + "');";

    if (!query.exec(QString::fromStdString(qurtyStr)))
        return -1;
    else
        return 0;
}

int createSession(string host_ip)
{
    QSqlQuery query;
    string qurtyStr = "INSERT INTO chat_session (host_ip) VALUES ('" + safeToSQL(host_ip) + "')";
    if (!query.exec(QString::fromStdString(qurtyStr))) {
        return -1;
    } else {
        if (!query.exec("SELECT last_insert_rowid()")) {
            return -2;
        } else {
            query.next();
            return query.value(0).toInt();
        }
            
        return -3;
    }
}

void closeDB()
{
    database.close();
}