#include "selectmusicurl.h"

SelectMusicUrl::SelectMusicUrl(QFileInfoList list)
    : _list(list)
{
    setAutoDelete(true);
}

void SelectMusicUrl::run()
{
    QFileInfoList musicList;
    for(int i=0; i<_list.size(); i++){
        QString suffix = _list[i].suffix();
        if (!_musicSuffix.contains(suffix)) continue;

        musicList.append(_list[i]);
    }
    emit fileSelected(musicList);
}
