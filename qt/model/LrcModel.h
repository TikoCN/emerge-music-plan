#ifndef LRCMODEL_H
#define LRCMODEL_H

#include <QAbstractListModel>

#include "baseclass/LrcData.h"

/**
 * @brief 以 QAbstractListModel 角色向歌词列表提供行数据。
 */
class LrcModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum LrcRoles {
        LrcIdRole = Qt::UserRole + 1,
        StartTimeRole,
        EndTimeRole,
        StartListRole,
        EndListRole,
        TextListRole,
        HelpTextListRole
    };

    /**
     * @brief 创建歌词列表模型。
     * @param parent 父对象。
     */
    explicit LrcModel(QObject *parent = nullptr);

    /**
     * @brief 获取顶层歌词行数。
     * @param parent 父索引。
     * @return 歌词行数。
     * @override
     */
    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief 按角色获取歌词行字段。
     * @param index 歌词行索引。
     * @param role 数据角色。
     * @return 对应角色的数据。
     * @override
     */
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief 返回供 QML 委托绑定的角色名称。
     * @return 角色编号与名称映射。
     * @override
     */
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief 用新歌词替换模型内容。
     * @param lyrics 按播放顺序排列的歌词数据。
     */
    void setLyrics(const QList<LrcDataPtr> &lyrics);

    /**
     * @brief 清空歌词行。
     */
    void clear();

private:
    QList<LrcDataPtr> m_lyrics;
};

#endif
