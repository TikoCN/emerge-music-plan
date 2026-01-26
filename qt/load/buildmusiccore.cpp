#include "buildmusiccore.h"

#include <utility>
#include "ffmpeg.h"

BuildMusicCore::BuildMusicCore(QFileInfoList infoList)
    : m_infoList(std::move(infoList))
{
    setAutoDelete(true);
}

void BuildMusicCore::run()
{
    FFmpeg ff;
    for (const auto & i : m_infoList) {
        MediaData data;
        ff.getDict(&data, i.filePath());
        m_musicList.append(data);
    }
    emit dataLoaded(m_musicList);
}




