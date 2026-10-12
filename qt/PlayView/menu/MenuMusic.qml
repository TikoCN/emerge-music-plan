import QtQuick
import PlayView
import Tiko
import MediaerAPI

TikoMenu{
    id: menuMusic
    cascade: true
    onClosed: {
        menuMusic.destroy()
    }

    property int musicId: -1
    property int type: 0
    property string musicTitle: ""
    property string artist: ""
    property string album: ""
    property string fileUrl: ""
    signal playlistChanged()

    Component.onCompleted: {
        let music = MusicLibrary.getData(musicId)
        musicTitle = music.title
        artist = music.artist
        album = music.album
        fileUrl = music.url
    }

    TikoMenuItem {
        text: qsTr("播放")
        onTriggered: MediaPlayer.buildPlayingListByMusicId(musicId)
        icon.source: "qrc:/image/play.svg"
    }

    TikoMenuItem {
        text: qsTr("添加到播放队列")
        onTriggered: MediaPlayer.appendPlayingListByMusicId(musicId)
    }

    TikoMenuItem {
        text: qsTr("添加到正在下一首播放")
        onTriggered: MediaPlayer.insertPlayingListByMusicId(musicId)
    }
    TikoMenuSpacer{}

    TikoMenuItem{
        text: qsTr("打开文件夹")
        enabled: BaseTool.fileManagement.pathExists(BaseTool.fileManagement.getParentDir(fileUrl))
        onTriggered: {
            const url = BaseTool.fileManagement.getParentDir(fileUrl);
            BaseTool.fileManagement.deskOpenFile(url)
        }
        icon.source: "qrc:/image/dir.svg"
    }

    TikoMenuItem{
        text: qsTr("打开歌词")
        enabled: BaseTool.fileManagement.pathExists(BaseTool.fileManagement.getMusicLrcUrl(musicId))
        onTriggered: BaseTool.fileManagement.deskOpenFile(BaseTool.fileManagement.getMusicLrcUrl(musicId))
    }

    TikoMenuItem{
        text: qsTr("打开封面")
        enabled: BaseTool.fileManagement.pathExists(BaseTool.fileManagement.getBaseUrl(fileUrl) + ".jpg")
        onTriggered: {
            const url = BaseTool.fileManagement.getBaseUrl(fileUrl) + ".jpg";
            BaseTool.fileManagement.deskOpenFile(url)
        }
    }

    TikoMenuSpacer{}

    TikoMenuItem{
        text: qsTr("复制 文件路径")
        onTriggered: BaseTool.copyString(fileUrl)
    }

    TikoMenuItem{
        text: qsTr("复制 歌曲信息")
        onTriggered: {
            const str = qsTr("标题") + " " + musicTitle +
                qsTr("专辑") + " " + album +
                qsTr("作曲家") + " " + artist;
            BaseTool.copyString(str)
        }
    }

    TikoMenuSpacer{}

    TikoMenuItem{
        text: qsTr("编辑音乐信息")
        onTriggered: CoreData.editMusic(menuMusic.parent, menuMusic.musicId)
    }

    TikoMenu{
        title: qsTr("从网络中下载")
        TikoMenuItem{
            text: qsTr("从 网络中下载 封面")
            onTriggered: DataActive.downMusicCover(musicId);
        }
        TikoMenuItem{
            text: qsTr("从 QQ音乐 下载 封面")
            onTriggered: DataActive.downMusicCoverQQMusic(musicId);
        }
        TikoMenuItem{
            text: qsTr("从 网易云 下载 封面")
            onTriggered: DataActive.downMusicCoverNetEase(musicId);
        }
        TikoMenuItem{
            text: qsTr("从 Bing搜索 下载 封面")
            onTriggered: DataActive.downCoverBing(musicId);
        }
        TikoMenuItem{
            text: qsTr("从 Baidu搜索 下载 封面")
            onTriggered: DataActive.downCoverBaidu(musicId);
        }
        TikoMenuSpacer{}
        TikoMenuItem{
            text: qsTr("从 网络中下载 歌词")
            onTriggered: DataActive.downLrc(musicId);
        }
        TikoMenuItem{
            text: qsTr("从 QQ音乐 下载 歌词")
            onTriggered: DataActive.downLrcFromQQMusic(musicId);
        }
        TikoMenuItem{
            text: qsTr("从 网易云 下载 歌词")
            onTriggered: DataActive.downLrcFromNetEase(musicId);
        }
    }

    TikoMenuSpacer{}

    TikoMenu{
        title: qsTr("添加到...")
        icon.source: "qrc:/image/move.svg"

        Repeater{
            delegate: TikoMenuItem {
                text: model.name
                enabled: !model.isDir
                onTriggered: {
                    if (PlaylistLibrary.addSingleMusicToPlaylist(menuMusic.musicId, model.id))
                        menuMusic.playlistChanged()
                }
            }
            model: PlaylistModel {
                type: PlaylistModel.UserModel
            }
        }
    }
}
