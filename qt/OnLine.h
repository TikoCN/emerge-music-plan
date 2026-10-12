#ifndef ONLINE_H
#define ONLINE_H

#include <QObject>

class OnLine : public QObject {
    Q_OBJECT

private:
    OnLine();

public:
    static OnLine &getInstance() {
        static OnLine instance;
        return instance;
    }

    //下载封面
    static void downMusicCover(const QString &key, const QString &url);
    static void downArtistCover(const QString &key, const QString &url);
    static void downAlbumCover(const QString &key, const QString &url);

    static void downMusicCoverNetEase(const QString &key, const QString &url);

    static void downMusicCoverQQMusic(const QString &key, const QString &url);

    static void downCoverBing(const QString &key, const QString &url);

    static void downCoverBaidu(const QString &key, const QString &url);

    //加载图片写入文件
    static void writeCoverToFile(const QString &url, const QString &file);

    //下载歌词
    void downLrc(const QString &key, const QString &url, int musicId);

    void downLrcFromNetEase(const QString &key, const QString &url, int musicId = -1);

    void downLrcFromQQMusic(const QString &key, const QString &url, int musicId = -1);

    // 下载音乐元数据
    static void downMediaFromNetEase(const QString &search, const QString &musicUrl);

    static void downMediaFromQQMusic(const QString &search, const QString &musicUrl);

    /**
     * @brief 将用户确认的预览资源写入目标文件。
     * @param filePath 目标文件路径
     * @param data 预览数据
     * @param isImage 是否为 Base64 编码的图片数据
     */
    Q_INVOKABLE void savePreviewResource(const QString &filePath, const QString &data, bool isImage);

signals:
    void lrcDowned(int musicId);
    /**
     * @brief 通知界面展示当前资源与网络资源的对比。
     * @param oldResourceUrl 当前图片地址或当前歌词文本
     * @param newResourceData 新图片的 Base64 数据或新歌词文本
     * @param filePath 确认后写入的目标文件路径
     * @param isImage 是否为图片资源
     */
    void resourcePreviewReady(const QString &oldResourceUrl, const QString &newResourceData, const QString &filePath,
                              bool isImage);
    void resourceDownloadFailed(const QString &message);
};

#endif // ONLINE_H
