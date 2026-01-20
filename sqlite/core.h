#ifndef CORE_H
#define CORE_H

#include <QObject>
#include "sqlite3.h"

class Core
{
protected:
    sqlite3 *db;
    int r;
    char *error;

    void logError(QString error);

    static int countCallBack(void *data, int argc, char **argv, char **azColName);
    static int idListCallBack(void *data, int argc, char **argv, char **azColName);

    void stmtPrepare(sqlite3_stmt **stmt, const char *sql);
    void stmtBindText(sqlite3_stmt *stmt, int pos, QString s);
    void stmtBindInt(sqlite3_stmt *stmt, int pos, long long i);
    bool stmtStep(sqlite3_stmt *stmt);
    void stmtReset(sqlite3_stmt *stmt);
    void stmtFree(sqlite3_stmt *stmt);

    void sqlExec(const char *sql, sqlite3_callback back, void *data);

public:
    bool commit();

    bool begin();

    bool rollback();
};

#endif // CORE_H
