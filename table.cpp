#include "table.h"
#include <QUrl>
#include <QDesktopServices>

//复制列表
QList<Music *> Table::getMusics() const
{
    return musics;
}

void Table::setMusics(const QList<Music *> &newMusics)
{
    if (musics == newMusics)
        return;
    musics = newMusics;
    emit musicsChanged();
}

void Table::copy(Table* a)
{
    name = a->name;
    tableId = a->tableId;
    musics.clear();
    musics.append(a->showMusics);
}

void Table::sortMusic(int type)
{
    if(showMusics.size() == 0){
        return;
    }

    SORT_TYPE inSort = static_cast<SORT_TYPE>(type);
    if(sort == inSort){
        return;
    }

    sort = inSort;

    switch (sort) {
    case SORT_TITTLE_ASC:
        std::sort(showMusics.begin(), showMusics.end(), [this](Music *a, Music *b){
            bool isRight = a->title > b->title;
            return isRight;
        });
        break;
    case SORT_TITTLE_DESC:
        std::sort(showMusics.begin(), showMusics.end(), [this](Music *a, Music *b){
            bool isRight = a->title < b->title;
            return isRight;
        });
        break;
    case SORT_ATRIST_ASC:
        std::sort(showMusics.begin(), showMusics.end(), [this](Music *a, Music *b){
            bool isRight = a->artist > b->artist;
            return isRight;
        });
        break;
    case SORT_ATRIST_DESC:
        std::sort(showMusics.begin(), showMusics.end(), [this](Music *a, Music *b){
            bool isRight = a->artist < b->artist;
            return isRight;
        });
        break;
    case SORT_ALUMB_ASC:
        std::sort(showMusics.begin(), showMusics.end(), [this](Music *a, Music *b){
            bool isRight = a->alumb > b->alumb;
            return isRight;
        });
        break;
    case SORT_ALUMB_DESC:
        std::sort(showMusics.begin(), showMusics.end(), [this](Music *a, Music *b){
            bool isRight = a->alumb < b->alumb;
            return isRight;
        });
        break;
    case SORT_NED_TIME_ASC:
        std::sort(showMusics.begin(), showMusics.end(), [this](Music *a, Music *b){
            bool isRight = a->endTime > b->endTime;
            return isRight;
        });
        break;
    case SORT_NED_TIME_DESC:
        std::sort(showMusics.begin(), showMusics.end(), [this](Music *a, Music *b){
            bool isRight = a->endTime < b->endTime;
            return isRight;
        });
        break;
    case SORT_LAST_EDIT_TIME_ASC:
        std::sort(showMusics.begin(), showMusics.end(), [this](Music *a, Music *b){
            bool isRight = a->lastEditTime > b->lastEditTime;
            return isRight;
        });
        break;
    case SORT_LAST_EDIT_TIME_DESC:
        std::sort(showMusics.begin(), showMusics.end(), [this](Music *a, Music *b){
            bool isRight = a->lastEditTime < b->lastEditTime;
            return isRight;
        });
        break;
    }

    //qml重新生成控件
    emit rebuildShowMusic();
}

int Table::getSort()
{
    return static_cast<int>(sort);
}

void Table::searchMusic(QString search)
{
    //清空并重建
    showMusics.clear();
    for(int i=0; i<musics.size(); i++){
        if(musics[i]->isSearch(search)){
            showMusics.append(musics[i]);
        }
    }

    //qml重新生成控件
    emit rebuildShowMusic();
}

int Table::getLastCoreId()
{
    return showMusics.last()->coreId;
}

/*
 * 插入新音乐核心
*/
void Table::insertMusic(Music *core)
{
    musics.append(core);//插入到数据库
    showMusics.append(core);//符合条件插入显示
    emit addMusic(1);
}

void Table::insertMusic(QList<Music *> core)
{
    musics.append(core);//插入到数据库

    int success = 0;
    for(int i=0; i<core.size(); i++){
            showMusics.append(core[i]);//符合条件插入显示
            success++;
    }
    emit addMusic(success);
}

/*
 * 移除音乐核心
*/
void Table::removeMusic(int listId)
{
    musics.removeAt(listId);
}

void Table::openDir()
{
    if(isDir){
        QString url = "file:///" + this->url;
        QDesktopServices::openUrl(QUrl(url));
    }
}

void Table::showLove()
{
    showMusics.clear();
    for(int i=0; i<musics.size(); i++){
        if(musics[i]->love){
            showMusics.append(musics[i]);
        }
    }

    //qml重新生成控件
    emit rebuildShowMusic();
}

void Table::showAllMusic()
{
    showMusics.clear();
    showMusics = musics;

    //qml重新生成控件
    emit rebuildShowMusic();
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
}

QList<Music *> Table::getShowMusics() const
{
    return showMusics;
}

void Table::setShowMusics(const QList<Music *> &newShowMusics)
{
    if (showMusics == newShowMusics)
        return;
    showMusics = newShowMusics;
    emit showMusicsChanged();
}


