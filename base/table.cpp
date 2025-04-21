#include "table.h"
#include "sqlite/sqlite.h"
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
    tableId = a->tableId;
    musicList = a->musicList;
}

void Table::sortMusic(int type)
{


    // qml重新生成控件
    emit buildShow();
}

int Table::getSort()
{
    return static_cast<int>(sort);
}

/*
 * 插入新音乐核心
*/
void Table::appendMusic(int core)
{
    musicList.append(core);//插入到数据库
    emit musicAppend(musicList.size()-1, 1);
}

void Table::appendMusic(QList<int> core)
{
    musicList.append(core);//插入到数据库

    int success = core.size();
    emit musicAppend(musicList.size()-success, success);
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

void Table::showAllMusic()
{
    musicList.clear();

    emit buildShow();
}

void Table::showLoveMusic()
{
    musicList.clear();


    emit buildShow();
}

void Table::showSearchMusic(QString search)
{
    //清空并重建
    musicList.clear();



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




