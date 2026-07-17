//
// Created by changzhi on 2026/2/16.
//

#include "delete.h"
#include "baseclass/DataException.h"

/**
 * @brief 删除"正在播放列表"中 position 的大于 position 的 musicId
 * @param position 起始位置
 */
bool Delete::deletePlayingList(const int position) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;

    try {
        // DELETE FROM playinglist WHERE position >= ?
        const auto sql = QString("DELETE FROM %1 WHERE %2 >= ?")
                        .arg(LiteralConstant::Table::PLAYINGLIST)
                        .arg(LiteralConstant::Column::POSITION);
        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtBindInt(stmt, 1, position);
        core->stmtStep(stmt);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }

    return result;
}
