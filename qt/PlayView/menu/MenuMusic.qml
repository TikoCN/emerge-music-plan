import QtQuick
import PlayView
import Tiko
import MediaerAPI
import DataType

TikoMenu{
    id: menuMusic
    cascade: true
    onClosed: {
        menuMusic.destroy()
    }

    property int musicId: -1
    property int type: 0
    property string musicTitle: ""
    property string artist : ""
    property string album: ""
    property string fileUrl: ""

    Component.onCompleted:{
        const Json = DataActive.getMusicJson(musicId);
        musicTitle = Json.title
        artist = Json.artist
        album = Json.album
        fileUrl = Json.url
    }

    TikoMenuItem {
        text: qsTr("播放")
        onClicked: MediaPlayer.buildPlayingListByMusicId(musicId)
        icon.source: "qrc:/image/play.png"
    }

    TikoMenuItem {
        text: qsTr("添加到播放队列")
        onClicked: MediaPlayer.appendPlayingListByMusicId(musicId)
    }

    TikoMenuItem {
        text: qsTr("添加到正在下一首播放")
        onClicked: MediaPlayer.insertPlayingListByMusicId(musicId)
    }
    TikoMenuSpeacer{}

    TikoMenuItem{
        text: qsTr("打开文件夹")
        onTriggered: {
            const url = BaseTool.fileManagement.getParentDir(fileUrl);
            BaseTool.fileManagement.deskOpenFile(url)
        }
        icon.source: "qrc:/image/dir.png"
    }

    TikoMenuItem{
        text: qsTr("打开歌词")
        onTriggered: BaseTool.fileManagement.deskOpenFile(BaseTool.fileManagement.getMusicLrcUrl(musicId))
    }

    TikoMenuItem{
        text: qsTr("打开封面")
        onTriggered: {
            const url = BaseTool.fileManagement.getBaseUrl(fileUrl) + ".jpg";
            BaseTool.fileManagement.deskOpenFile(url)
        }
    }

    TikoMenuSpeacer{}

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

    TikoMenuSpeacer{}

    TikoMenuItem{
        text: qsTr("编辑音乐信息")
        onTriggered: CoreData.editMusic(menuMusic.parent, menuMusic.musicId)
    }

    TikoMenu{
        title: qsTr("从网络中下载")
        TikoMenuItem{
            text: qsTr("从 网络中下载 封面")
            onTriggered: OnLine.downMusicCover(BaseTool.fileManagement.getFileName(fileUrl), fileUrl);
        }
        TikoMenuItem{
            text: qsTr("从 QQ音乐 下载 封面")
            onTriggered: OnLine.downMusicCoverQQMusic(BaseTool.fileManagement.getFileName(fileUrl), fileUrl);
        }
        TikoMenuItem{
            text: qsTr("从 网易云 下载 封面")
            onTriggered: OnLine.downMusicCoverNetEase(BaseTool.fileManagement.getFileName(fileUrl), fileUrl);
        }
        TikoMenuItem{
            text: qsTr("从 Bing搜索 下载 封面")
            onTriggered: OnLine.downMusicCoverBing(BaseTool.fileManagement.getFileName(fileUrl), fileUrl);
        }
        TikoMenuItem{
            text: qsTr("从 Baidu搜索 下载 封面")
            onTriggered: OnLine.downCoverBaidu(BaseTool.fileManagement.getFileName(fileUrl), fileUrl);
        }
        TikoMenuSpeacer{}
        TikoMenuItem{
            text: qsTr("从 网络中下载 歌词")
            onTriggered: OnLine.downLrc(BaseTool.fileManagement.getFileName(fileUrl), fileUrl);
        }
        TikoMenuItem{
            text: qsTr("从 QQ音乐 下载 歌词")
            onTriggered: OnLine.downLrcFromQQMusic(BaseTool.fileManagement.getFileName(fileUrl), fileUrl);
        }
        TikoMenuItem{
            text: qsTr("从 网易云 下载 歌词")
            onTriggered: OnLine.downLrcFromNetEase(BaseTool.fileManagement.getFileName(fileUrl), fileUrl);
        }
    }

    TikoMenu{
        title: qsTr("格式转换为...")
        Repeater{
            delegate: TikoMenuItem{
                text: suffix
                onTriggered: music.setSuffix(suffix);
            }
            model: ListModel{
                ListElement{suffix: "MP3"}
                ListElement{suffix: "FLAC"}
                ListElement{suffix: "ALAC"}
                ListElement{suffix: "AAC"}
                ListElement{suffix: "WMA"}
                ListElement{suffix: "PCM16"}
                ListElement{suffix: "PCM32"}
            }
        }
    }

    TikoMenuSpeacer{}

    TikoMenu{
        title: qsTr("添加到...")
        icon.source: "qrc:/image/move.png"

        Repeater{
            delegate: addMenu
            model: ListModel{
                id: addMenuData
            }
        }
        Component {
            id: addMenu
            TikoMenuItem {
                text: CoreData.playlist[aim]["name"]
                enabled: !CoreData.playlist[aim].isDir
            }
        }

        onOpened: {
            addMenuData.clear()
            for(let i=0; i<CoreData.playlist.length; i++){
                addMenuData.append({aim: i})
            }
        }
    }
}
