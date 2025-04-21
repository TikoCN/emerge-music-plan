#include "table.h"
#include "sqlite/sqlite.h"
#include "musiccore.h"
#include <QUrl>
#include <QDesktopServices>

Table::Table(QObject *parent)
    :QObject(parent)
{
    sort = SORT_TITTLE_ASC;
}

long long Table::getDuraiton() const
{
    return duraiton;
}

QList<int> Table::getMusicList() const
{
    return musicList;
}

bool Table::getIsDir() const
{
    return isDir;
}

void Table::copy(Table* a)
{
    name = a->name;
    id = a->id;
    musicList = a->musicList;
}

void Table::sortMusic(int type)
{
    sort = static_cast<SORT_TYPE>(type);
    musicList = MusicCore::getInstance()->sortMusic(musicList, sort);
    // qml重新生成控件
    emit buildShow();
}

int Table::getSort()
{
    return static_cast<int>(sort);
}

/*
 * 移除音乐核心
*/
void Table::removeMusic(int listId)
{
    musicList.removeAt(listId);
}

void Table::openDir()
{
    if(isDir){
        QString url = "file:///" + this->url;
        QDesktopServices::openUrl(QUrl(url));
    }
}

void Table::showOrignMusic()
{
    musicList = SQLite::getInstance()->getTableMusicList(id);
    emit buildShow();
}

void Table::showLoveMusic()
{
    musicList = MusicCore::getInstance()->selectLoveMusic(musicList);
    emit buildShow();
}

void Table::showSearchMusic(QString search)
{
    musicList = MusicCore::getInstance()->selectSearchMusic(musicList, search);
    emit buildShow();
}

QString Table::getName() const
{
    return name;
}

void Table::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();

    SQLite::getInstance()->updateTable(this);
}


