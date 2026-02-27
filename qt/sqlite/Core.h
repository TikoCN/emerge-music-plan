#ifndef CORE_H
#define CORE_H

#include "sqlite3.h"
#include "Tlog.h"
#include "baseclass/SortType.h"
#include "basetool/BaseTool.h"
#include "baseclass/LiteralConstant.h"

class Core : public QObject
{
    Q_OBJECT
protected:
    TLog *m_loger;
    BaseTool *m_tool;
    sqlite3 *m_db = nullptr;
    int m_r = 0;
    char *m_error = nullptr;

    void throwError(const QString& error) const;

    static int countCallBack(void *data, int argc, char **argv, char **azColName);
    static int idListCallBack(void *data, int argc, char **argv, char **azColName);

    void stmtPrepare(sqlite3_stmt **stmt, const char *sql);
    void stmtBindText(sqlite3_stmt *stmt, int pos, const QString& s);
    void stmtBindInt(sqlite3_stmt *stmt, int pos, int i);
    void stmtBindLong(sqlite3_stmt *stmt, int pos, long long i);
    bool stmtStep(sqlite3_stmt *stmt);
    void stmtReset(sqlite3_stmt *stmt);

    static void stmtFree(sqlite3_stmt *stmt);

    void sqlExecuteCallBack(const char *sql, sqlite3_callback back, void *data);

    void sqlExecute(const char *sql, QString error);

public:
    explicit Core(TLog *log, BaseTool *tool);
    bool commit();

    bool begin();

    bool rollback();
};

#endif // CORE_H
