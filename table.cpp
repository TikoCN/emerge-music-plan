#include "table.h"
#include "popupdata.h"
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

void Table::sortMusic()
{
    if(showMusics.size() == 0){
        return;
    }

    switch (key) {
    case 0:
        std::sort(showMusics.begin(), showMusics.end(), [this](Music *a, Music *b){
            bool isRight = a->title > b->title;
            return forward ? isRight : !isRight;
        });
        break;
    case 1:
        std::sort(showMusics.begin(), showMusics.end(), [this](Music *a, Music *b){
            bool isRight = a->artist > b->artist;
            return forward ? isRight : !isRight;
        });
        break;
    case 2:
        std::sort(showMusics.begin(), showMusics.end(), [this](Music *a, Music *b){
            bool isRight = a->alumb > b->alumb;
            return forward ? isRight : !isRight;
        });
        break;
    case 3:
        std::sort(showMusics.begin(), showMusics.end(), [this](Music *a, Music *b){        
            bool isRight = a->endTime > b->endTime;
            return forward ? isRight : !isRight;
        });
        break;
    case 4:
        std::sort(showMusics.begin(), showMusics.end(), [this](Music *a, Music *b){
            bool isRight = a->lastEditTime > b->lastEditTime;
            return forward ? isRight : !isRight;
        });
        break;
    default:
        break;
    }
}

void Table::setSort(int key, bool forward)
{
    this->key = key;
    this->forward = forward;
    emit keyChanged();
    emit forwardChanged();
    sortMusic();//排序
    emit showMusicsChanged();
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

    if(core->isSearch(search)){
        showMusics.append(core);//符合条件插入显示
        emit addMusic(1);
    }
}

void Table::insertMusic(QList<Music *> core)
{
    musics.append(core);//插入到数据库

    int success = 0;
    for(int i=0; i<core.size(); i++){
        if(core[i]->isSearch(search)){
            showMusics.append(core[i]);//符合条件插入显示
            success++;
        }
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
    else{
        PopupData *popup = PopupData::getInstance();
        popup->message(tr("打开失败，不是本地文件夹"));
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

    sortMusic();
    emit showMusicsChanged();
}

void Table::showAllMusic()
{
    showMusics.clear();
    showMusics = musics;

    sortMusic();
    emit showMusicsChanged();
}

void Table::buildShowMusics()
{
    //清空并重建
    showMusics.clear();
    for(int i=0; i<musics.size(); i++){
        if(musics[i]->isSearch(search)){
            showMusics.append(musics[i]);
        }
    }

    sortMusic();//排序
    emit showMusicsChanged();
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

QString Table::getSearch() const
{
    return search;
}

void Table::setSearch(const QString &newSearch)
{
    if (search == newSearch)
        return;
    search = newSearch;
    emit searchChanged();

    //清空并重建
    buildShowMusics();
}

bool Table::getForward() const
{
    return forward;
}

void Table::setForward(bool newForward)
{
    if (forward == newForward)
        return;
    forward = newForward;

    setSort(key, forward);
}

int Table::getKey() const
{
    return key;
}

void Table::setKey(int newKey)
{
    if (key == newKey)
        return;
    key = newKey;

    setSort(key, forward);
}
