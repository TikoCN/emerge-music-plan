#ifndef MUSICMODEL_H
#define MUSICMODEL_H

#include <QAbstractListModel>

#include "baseclass/Music.h"
#include "macro/NotifyProperty.h"
#include "datacore/DataLoader.h"

class MusicModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum MusicRoles {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        ArtistRole,
        AlbumRole,
        UrlRole,
        DurationRole,
        LastEditRole,
        NameKeyRole,
        LastEditTimeRole,
        InsetTimeRole,
        LevelRole,
        PlayNumberRole,
        IsLoveRole
    };

    enum MusicType {
        KeyModel,
        AlbumModel,
        ArtistModel,
        PlaylistModel,
        MostPlayedModel,
        MostNewModel,
        RandModel,
        NowQueueModel
    };

    Q_ENUM(MusicType)
    QT_NOTIFY_PROPERTY(MusicType, type, Type)
    QT_NOTIFY_PROPERTY(int, artistId, ArtistId)
    QT_NOTIFY_PROPERTY(int, albumId, AlbumId)
    QT_NOTIFY_PROPERTY(int, playlistId, PlaylistId)
    QT_NOTIFY_PROPERTY(QString, key, Key)
    QT_NOTIFY_PROPERTY(int, sort, Sort)

public:
    explicit MusicModel(QObject *parent = nullptr);

    [[nodiscard]] int                    rowCount(const QModelIndex &parent) const override;
    [[nodiscard]] QVariant               data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void clear();
    /**
     * @brief 追加下一批歌曲，完整列表查询只加载一次。
     * @param parent 父索引，仅支持无效索引对应的根列表。
     * @override
     */
    void             fetchMore(const QModelIndex &parent) override;
    /**
     * @brief 判断根列表是否还有待加载的数据。
     * @param parent 父索引。
     * @return 根列表尚未加载完毕时返回 true。
     * @override
     */
    bool             canFetchMore(const QModelIndex &parent) const override;

    Q_INVOKABLE void updateMusicLove(int musicId, bool isLove);
    Q_INVOKABLE void updateMusicLevel(int musicId, int level);

private:
    QList<MusicPtr> m_musicList;
    Loader          m_loader;
};

#endif // MUSICMODEL_H
