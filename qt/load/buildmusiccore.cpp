#include "buildmusiccore.h"
#include "ffmpeg.h"

BuildMusicCore::BuildMusicCore(QFileInfoList infoList)
    : _infoList(infoList)
{
    setAutoDelete(true);
}

void BuildMusicCore::run()
{
    FFmpeg ff;
    for (int i = 0; i < _infoList.size(); ++i) {
        MediaData data;
        ff.getDict(&data, _infoList[i].filePath());
        _musicList.append(data);
    }
    emit dataLoaded(_musicList);
}




