//
// Created by admin on 2026/7/30.
//

#include "QueueRepository.h"
#include "baseclass/LiteralConstant.h"
#include "baseclass/DataException.h"

void QueueRepository::buildMusic(QList<int> ids) const {
    sqlite3_stmt *stmt = nullptr;
    try {
        core->begin();

        // 删除现有数据
        const auto deleteSql = QString("DELETE FROM %1")
               .arg(LiteralConstant::Table::NOW_QUEUE);
        core->sqlExecute(deleteSql.toUtf8(), "删除队列数据失败");

        // 插入新数据
        const auto insertSql = QString("INSERT INTO %1(%2, %3) VALUES(?, ?)")
                              .arg(LiteralConstant::Table::NOW_QUEUE)
                              .arg(LiteralConstant::Column::MUSIC_ID)
                              .arg(LiteralConstant::Column::POSITION);
        core->stmtPrepare(&stmt, insertSql.toUtf8());

        int position = 0;
        for (int id: ids) {
            core->stmtReset(stmt);
            core->stmtBindInt(stmt, 1, id);
            core->stmtBindInt(stmt, 2, position++);
            core->stmtStep(stmt);
        }

        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
    }
    Core::stmtFree(stmt);
}

void QueueRepository::appendMusic(QList<int> ids) const {
    sqlite3_stmt *stmt    = nullptr;
    sqlite3_stmt *maxStmt = nullptr;
    try {
        core->begin();

        // 查询当前最大position
        int        maxPosition = 0;
        const auto maxSql      = QString("SELECT COALESCE(MAX(%1), -1) FROM %2")
                           .arg(LiteralConstant::Column::POSITION)
                           .arg(LiteralConstant::Table::NOW_QUEUE);
        core->stmtPrepare(&maxStmt, maxSql.toUtf8());
        if (core->stmtStep(maxStmt)) {
            maxPosition = sqlite3_column_int(maxStmt, 0) + 1;
        }
        Core::stmtFree(maxStmt);
        maxStmt = nullptr;

        // 插入新数据
        const auto insertSql = QString("INSERT INTO %1(%2, %3) VALUES(?, ?)")
                              .arg(LiteralConstant::Table::NOW_QUEUE)
                              .arg(LiteralConstant::Column::MUSIC_ID)
                              .arg(LiteralConstant::Column::POSITION);
        core->stmtPrepare(&stmt, insertSql.toUtf8());

        int position = maxPosition;
        for (int id: ids) {
            core->stmtReset(stmt);
            core->stmtBindInt(stmt, 1, id);
            core->stmtBindInt(stmt, 2, position++);
            core->stmtStep(stmt);
        }

        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
    }
    Core::stmtFree(stmt);
}

QList<int> QueueRepository::getMusic() const {
    QList<int>    list;
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = QString("SELECT %1 FROM %2 ORDER BY %3 ASC")
                        .arg(LiteralConstant::Column::MUSIC_ID)
                        .arg(LiteralConstant::Table::NOW_QUEUE)
                        .arg(LiteralConstant::Column::POSITION);
        core->stmtPrepare(&stmt, sql.toUtf8());
        while (core->stmtStep(stmt)) {
            list.append(sqlite3_column_int(stmt, 0));
        }
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        list.clear();
    }
    Core::stmtFree(stmt);
    return list;
}
