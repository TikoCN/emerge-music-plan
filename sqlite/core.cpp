#include "core.h"
#include <QDebug>

void Core::logError(QString error)
{
    if (this->error != nullptr) {
        error += " " + QString(this->error);
    }
    if (sqlite3_errmsg(db) != nullptr && QString(sqlite3_errmsg(db)) != QString(this->error)) {
        error += " " + QString(sqlite3_errmsg(db));
    }
    error = "SQLITE3 MODE：ERROR CODE " + QString::number(r) + ", " + error;
    qDebug()<<error;
    throw error;
}

void Core::stmtPrepare(sqlite3_stmt **stmt, const char *sql)
{
    r = sqlite3_prepare_v2(db, sql, -1, stmt, nullptr);
    if (r != SQLITE_OK) {
        logError(QString("检测 %1 初始化失败").arg(QString(sql)));
    }
}

void Core::stmtBindText(sqlite3_stmt *stmt, int pos, QString s)
{
    r = sqlite3_bind_text(stmt, pos, s.toStdString().c_str(), -1, SQLITE_TRANSIENT);
    if (r != SQLITE_OK) {
        QString error = QString("绑定 %1 位置 %2 变量为 %3 失败").arg(sqlite3_sql(stmt), QString::number(pos), s);
        logError(error);
    }
}

void Core::stmtBindInt(sqlite3_stmt *stmt, int pos, long long i)
{
    r = sqlite3_bind_int(stmt, pos, i);
    if (r != SQLITE_OK) {
        QString error = QString("绑定 %1 位置 %2 变量为 %3 失败").arg(sqlite3_sql(stmt), QString::number(pos), QString::number(i));
        logError(error);
    }
}

bool Core::stmtStep(sqlite3_stmt *stmt)
{
    r = sqlite3_step(stmt);
    if (r == SQLITE_ROW) {
        return true;
    }
    else if (r == SQLITE_DONE) {
        return false;
    }

    char *sql = sqlite3_expanded_sql(stmt);
    QString error = QString("执行 %1 失败").arg(sql);
    sqlite3_free(sql);
    logError(error);
    return false;
}

void Core::stmtReset(sqlite3_stmt *stmt)
{
    r = sqlite3_reset(stmt); // 删除绑定
    if (r != SQLITE_OK) {
        char *sql = sqlite3_expanded_sql(stmt);
        QString error = QString("重置 %1 失败，当前为 %2").arg(sqlite3_sql(stmt), sql);
        sqlite3_free(sql);
        logError(error);
    }
}

void Core::stmtFree(sqlite3_stmt *stmt)
{
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

void Core::sqlExec(const char *sql, sqlite3_callback back, void *data)
{
    r = sqlite3_exec(db, sql, back, data, &error);
    if (r != SQLITE_OK) {
        QString error = QString("执行 %1 失败").arg(sql);
        logError(error);
    }

}

bool Core::begin()
{
    try {
        r = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &error);
        if (r != SQLITE_OK) logError("开始事务失败");
    } catch (QString e) {
        return false;
    }
    return true;
}

bool Core::rollback()
{
    try {
        r = sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, &error);
        if (r != SQLITE_OK) logError("回滚事务失败");
    } catch (QString e) {
        return false;
    }
    return true;
}

bool Core::commit()
{
    try {
        r = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &error);
        if (r != SQLITE_OK) logError("结束事务失败");
    } catch (QString e) {
        return false;
    }
    return true;
}

int Core::countCallBack(void *data, int argc, char **argv, char **azColName)
{
    QString valueStr(*argv);
    int *value = static_cast<int *>(data);
    *value = valueStr.toInt();
    return SQLITE_OK;
}

int Core::idListCallBack(void *data, int argc, char **argv, char **azColName)
{
    QList<int> *idList = static_cast<QList<int> *>(data);
    idList->append(QString(argv[0]).toInt());
    return SQLITE_OK;
}
