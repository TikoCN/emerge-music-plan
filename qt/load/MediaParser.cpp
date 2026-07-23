#include "MediaParser.h"

#include <utility>
#include "ffmpeg.h"

MediaParser::MediaParser(QFileInfoList infoList)
    : m_infoList(std::move(infoList)) {
    setAutoDelete(true);
}

void MediaParser::run() {
    FFmpeg ff;
    for (const auto &info : m_infoList) {
        MediaData data;
        ff.getDict(&data, info.filePath());
        m_dataList.append(data);
    }
    emit parseFinished(m_dataList);
}