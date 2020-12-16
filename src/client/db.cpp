#include "db.h"

QSqlDatabase database;

int openDB()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("local_storage.db");
    if (!database.open()) {
        msgBoxCritical("Unable to open the local database.");
        return -1;
    }

    /* Set up the tables if not exists */
    QSqlQuery query;
    std::string queries[2] = {
        "CREATE TABLE IF NOT EXISTS chat_session(session_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "host VARCHAR(64), nickname VARCHAR(24), created_at DATETIME DEFAULT CURRENT_TIMESTAMP);",
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

int insertMessage(string chatSessionId, string nickname, string content)
{
    QSqlQuery query;
    string qurtyStr = "INSERT INTO message(chat_session_id, nickname, content) VALUES "
        "(" + chatSessionId + ", '" + safeToSQL(nickname) + "', '" + safeToSQL(content) + "');";

    if (!query.exec(QString::fromStdString(qurtyStr)))
        return -1;
    else
        return 0;
}

int createSession(string host, string nickname)
{
    QSqlQuery query;
    string qurtyStr = "INSERT INTO chat_session (host, nickname) VALUES ('" + safeToSQL(host) + 
        "', '" + safeToSQL(nickname) + "')";
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

QList<struct ChatEntry> getChatData(int sessionId, string content, string nickname, string host)
{
    QList<struct ChatEntry> list;
    QSqlQuery query;
    string queryStr = "SELECT message.message_id, message.chat_session_id, message.nickname, "
        "datetime(message.created_at, 'localtime'), message.content FROM message, chat_session "
        "WHERE chat_session_id = " + to_string(sessionId) + " AND message.chat_session_id = "
        "chat_session.session_id ";

    if (content.length() > 0) {
        queryStr += "AND message.content LIKE '%" + safeToSQL(content) + "%' ";
    }

    if (nickname.length() > 0) {
        queryStr += "AND message.nickname LIKE '%" + safeToSQL(nickname) + "%' ";
    }

    if (host.length() > 0) {
        queryStr += "AND chat_session.host LIKE '%" + safeToSQL(host) + "%' ";
    }

    queryStr += " ORDER BY message.created_at ASC";

    query.exec(QString::fromStdString(queryStr));

    while (query.next()) {
        struct ChatEntry entry;
        entry.nickname = query.value(2).toString();
        entry.createdAt = query.value(3).toString();
        entry.content = query.value(4).toString();
        list.append(entry);
    }


    return list;
}

QList<struct ListEntry> findSession(int type, string content, string nickname, string host)
{
    QList<struct ListEntry> list;
    QSqlQuery query;
    string queryStr = "";
    if (type == LIST_ALL) {
        /* Perfrom a simple search */
        queryStr = "SELECT session_id, host, nickname, datetime(created_at, 'localtime') "
            "FROM chat_session ORDER BY created_at DESC";

    } else {
        queryStr = "SELECT DISTINCT session_id, host, chat_session.nickname, datetime("
            "chat_session.created_at, 'localtime') FROM chat_session, message "
            "WHERE chat_session.session_id = message.chat_session_id ";

        if (content.length() > 0) {
            queryStr += "AND message.content LIKE '%" + safeToSQL(content) + "%' ";
        }

        if (nickname.length() > 0) {
            queryStr += "AND message.nickname LIKE '%" + safeToSQL(nickname) + "%' ";
        }

        if (host.length() > 0) {
            queryStr += "AND chat_session.host LIKE '%" + safeToSQL(host) + "%' ";
        }

        queryStr += "ORDER BY chat_session.created_at DESC";

        // msgBoxInfo(QString::fromStdString(queryStr));
        
    }

    query.exec(QString::fromStdString(queryStr));

    while (query.next()) {
        struct ListEntry entry;
        entry.sessionId = query.value(0).toInt();
        entry.host = query.value(1).toString();
        entry.nickname = query.value(2).toString();
        entry.createdAt = query.value(3).toString();
        list.append(entry);
    }
    
    

    return list;
}

void closeDB()
{
    database.close();
}

