#ifndef TABLE_H
#define TABLE_H

#include <QObject>
#include "music.h"

class Table : public QObject //播放列表
{
    Q_OBJECT
public:
    QString name;//列表名
    QString url;//文件夹路径
    QString search = "";//搜索
    QList<Music *>musics;//音乐库列表
    QList<Music *>showMusics;//显示音乐列表
    int key = 1;//排序关键词
    int tableId;//列表id
    bool forward = true;//排序方向
    bool isDir;

public:
    void copy(Table *a);

    //排序
    void sortMusic();

    //设置排序
    void setSort(int key, bool forward);

    //得到最后id
    Q_INVOKABLE int getLastCoreId();

    //插入新音乐核心
    Q_INVOKABLE void insertMusic(Music *core);
    Q_INVOKABLE void insertMusic(QList<Music *> core);

    //移除音乐核心
    Q_INVOKABLE void removeMusic(int listId);

    //打开本地文件夹
    Q_INVOKABLE void openDir();

    //显示喜爱歌曲
    Q_INVOKABLE void showLove();

    //显示所有歌曲
    Q_INVOKABLE void showAllMusic();

    //建立显示列表
    void buildShowMusics();

    QList<Music *> getShowMusics() const;
    void setShowMusics(const QList<Music *> &newShowMusics);

    QString getName() const;
    void setName(const QString &newName);

    QList<Music *> getMusicList() const;
    void setMusicList(const QList<Music *> &newMusicList);

    QString getSearch() const;
    void setSearch(const QString &newSearch);

    bool getForward() const;
    void setForward(bool newForward);

    int getKey() const;
    void setKey(int newKey);

    QList<Music *> getMusics() const;
    void setMusics(const QList<Music *> &newMusics);

signals:
    //插入新条目
    void addMusic(int);

    //更新封面
    void cppUpdateCover();

    void showMusicsChanged();
    void nameChanged();
    void musicListChanged();
    void searchChanged();
    void forwardChanged();
    void keyChanged();
    void musicsChanged();

private:
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QList<Music *> showMusics READ getShowMusics WRITE setShowMusics NOTIFY showMusicsChanged FINAL)
    Q_PROPERTY(QString search READ getSearch WRITE setSearch NOTIFY searchChanged FINAL)
    Q_PROPERTY(bool forward READ getForward WRITE setForward NOTIFY forwardChanged FINAL)
    Q_PROPERTY(int key READ getKey WRITE setKey NOTIFY keyChanged FINAL)
    Q_PROPERTY(QList<Music *> musics READ getMusics WRITE setMusics NOTIFY musicsChanged FINAL)
};
#endif // TABLE_H
