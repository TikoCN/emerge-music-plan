#include "core.h"
#include <QDebug>
#include "baseclass/dataexception.h"

void Core::throwError(const QString& error) const {
    if (m_db != nullptr) {
        const auto sqliteErr = QString(sqlite3_errmsg(m_db));
        QString fullError = "SQLITE3 ERROR CODE " + QString::number(m_r) + ": " + error;
        if (!sqliteErr.isEmpty() && sqliteErr != error) {
            fullError += " (" + sqliteErr + ")";
        }
        throw DataException(fullError);
    } else {
        throw DataException(error);
    }
}

void Core::stmtPrepare(sqlite3_stmt **stmt, const char *sql)
{
    m_r = sqlite3_prepare_v2(m_db, sql, -1, stmt, nullptr);
    if (m_r != SQLITE_OK) {
        throwError(QString("检测 %1 初始化失败").arg(QString(sql)));
    }
}

void Core::stmtBindText(sqlite3_stmt *stmt, const int pos, const QString& s)
{
    m_r = sqlite3_bind_text(stmt, pos, s.toStdString().c_str(), -1, SQLITE_TRANSIENT);
    if (m_r != SQLITE_OK) {
        const QString error = QString("绑定 %1 位置 %2 变量为 %3 失败").arg(sqlite3_sql(stmt), QString::number(pos), s);
        throwError(error);
    }
}

void Core::stmtBindInt(sqlite3_stmt *stmt, const int pos, const int i)
{
    m_r = sqlite3_bind_int(stmt, pos, i);
    if (m_r != SQLITE_OK) {
        const QString error = QString("绑定 %1 位置 %2 变量为 %3 失败").arg(sqlite3_sql(stmt), QString::number(pos), QString::number(i));
        throwError(error);
    }
}

void Core::stmtBindLong(sqlite3_stmt *stmt, const int pos, const long long i) {
    m_r = sqlite3_bind_int64(stmt, pos, i);
    if (m_r != SQLITE_OK) {
        const QString error = QString("绑定 %1 位置 %2 变量为 %3 失败").arg(sqlite3_sql(stmt), QString::number(pos), QString::number(i));
        throwError(error);
    }
}

bool Core::stmtStep(sqlite3_stmt *stmt)
{
    m_r = sqlite3_step(stmt);
    if (m_r == SQLITE_ROW) {
        return true;
    }
    else if (m_r == SQLITE_DONE) {
        return false;
    }

    char *sql = sqlite3_expanded_sql(stmt);
    const QString error = QString("执行 %1 失败").arg(sql);
    sqlite3_free(sql);
    throwError(error);
    return false;
}

void Core::stmtReset(sqlite3_stmt *stmt)
{
    m_r = sqlite3_reset(stmt); // 删除绑定
    if (m_r != SQLITE_OK) {
        char *sql = sqlite3_expanded_sql(stmt);
        const QString error = QString("重置 %1 失败，当前为 %2").arg(sqlite3_sql(stmt), sql);
        sqlite3_free(sql);
        throwError(error);
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
    m_r = sqlite3_exec(m_db, sql, back, data, &m_error);
    if (m_r != SQLITE_OK) {
        const QString error = QString("执行 %1 失败").arg(sql);
        throwError(error);
    }

}

Core::Core(TLog *log)
    :tlog(log){
}

bool Core::begin()
{
    try {
        m_r = sqlite3_exec(m_db, "BEGIN TRANSACTION;", nullptr, nullptr, &m_error);
        if (m_r != SQLITE_OK) throwError("开始事务失败");
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        return false;
    }
    return true;
}

bool Core::rollback()
{
    try {
        m_r = sqlite3_exec(m_db, "ROLLBACK;", nullptr, nullptr, &m_error);
        if (m_r != SQLITE_OK) throwError("回滚事务失败");
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        return false;
    }
    return true;
}

bool Core::commit()
{
    try {
        m_r = sqlite3_exec(m_db, "COMMIT;", nullptr, nullptr, &m_error);
        if (m_r != SQLITE_OK) throwError("结束事务失败");
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        return false;
    }
    return true;
}

int Core::countCallBack(void *data, int argc, char **argv, char **azColName)
{
    const QString valueStr(argv[0]);
    *static_cast<int*>(data) = valueStr.toInt();
    return SQLITE_OK;
}

int Core::idListCallBack(void *data, int argc, char **argv, char **azColName)
{
    auto *idList = static_cast<QList<int> *>(data);
    idList->append(QString(argv[0]).toInt());
    return SQLITE_OK;
}
