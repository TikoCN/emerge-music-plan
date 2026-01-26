#include "selectmusicurl.h"

#include <utility>

SelectMusicUrl::SelectMusicUrl(QFileInfoList list)
    : m_list(std::move(list))
{
    setAutoDelete(true);
}

void SelectMusicUrl::run()
{
    QFileInfoList musicList;
    for(const QFileInfo & i : m_list){
        if (QString suffix = i.suffix(); !m_musicSuffix.contains(suffix))
            continue;

        musicList.append(i);
    }
    emit fileSelected(musicList);
}
