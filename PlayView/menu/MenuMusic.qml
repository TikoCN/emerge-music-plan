import QtQuick
import PlayView
import Tiko
import MediaerAPI
import DataType

TikoMenu{
    id: menuMusic
    cascade: true
<<<<<<< Updated upstream:PlayView/menu/MenuMusic.qml
    onClosed: menuMusic.destroy()
=======
    onClosed: {
        menuMusic.destroy()
    }
>>>>>>> Stashed changes:qt/PlayView/menu/MenuMusic.qml

    property int musicId: -1
    property int type: 0
    property string musicTitle: ""
    property string artist : ""
    property string album: ""
    property string fileUrl: ""

<<<<<<< Updated upstream:PlayView/menu/MenuMusic.qml
    TikoMenuItem{
=======
    Component.onCompleted:{
        var Json = Core.getMusicJson(musicId)
        musicTitle = Json.title
        artist = Json.artist
        album = Json.album
        fileUrl = Json.url
    }

    TikoMenuItem {
>>>>>>> Stashed changes:qt/PlayView/menu/MenuMusic.qml
        text: qsTr("播放")
        onClicked: MediaPlayer.buildPlayingListByMusicList(musicId)
        icon.source: "qrc:/image/play.png"
    }

    TikoMenuItem {
        text: qsTr("添加到播放队列")
        onClicked: MediaPlayer.appendPlayingListByMusicList(musicId)
    }

    TikoMenuItem {
        text: qsTr("添加到正在下一首播放")
        onClicked: MediaPlayer.insertPlayingListByMusicList(musicId)
    }
    TikoMenuSpeacer{}

    TikoMenuItem{
        text: qsTr("打开文件夹")
        onTriggered: {
            var url = Base.getParentDir(fileUrl)
            Base.deskOpenFile(url)
        }
        icon.source: "qrc:/image/dir.png"
    }

    TikoMenuItem{
        text: qsTr("打开歌词")
        onTriggered: Base.deskOpenFile(FileMan.getMusicLrcUrl(musicId))
    }

    TikoMenuItem{
        text: qsTr("打开封面")
        onTriggered: {
            var url = Base.getBaseUrl(fileUrl) + ".jpg"
            Base.deskOpenFile(url)
        }
    }

    TikoMenuSpeacer{}

    TikoMenuItem{
        text: qsTr("删除")
    }

    TikoMenuItem{
        text: qsTr("删除本地文件")
    }

    TikoMenuItem{
        text: qsTr("更新数据")
    }

    TikoMenuSpeacer{}

    TikoMenuItem{
        text: qsTr("复制 文件路径")
        onTriggered: Base.copyString(fileUrl)
    }

    TikoMenuItem{
        text: qsTr("复制 歌曲信息")
        onTriggered: {
            var str = qsTr("标题") +" " + musicTitle +
                    qsTr("专辑") +" " + album +
                    qsTr("作曲家") +" " + artist
            Base.copyString(str)
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
            onTriggered: OnLine.downMusicCover(Base.getFileName(fileUrl), fileUrl);
        }
        TikoMenuItem{
            text: qsTr("从 QQ音乐 下载 封面")
            onTriggered: OnLine.downMusicCoverQQMusic(Base.getFileName(fileUrl), fileUrl);
        }
        TikoMenuItem{
            text: qsTr("从 网易云 下载 封面")
            onTriggered: OnLine.downMusicCoverNetEase(Base.getFileName(fileUrl), fileUrl);
        }
        TikoMenuItem{
            text: qsTr("从 Bing搜索 下载 封面")
            onTriggered: OnLine.downMusicCoverBing(Base.getFileName(fileUrl), fileUrl);
        }
        TikoMenuItem{
            text: qsTr("从 Baidu搜索 下载 封面")
            onTriggered: OnLine.downCoverBaidu(Base.getFileName(fileUrl), fileUrl);
        }
        TikoMenuSpeacer{}
        TikoMenuItem{
            text: qsTr("从 网络中下载 歌词")
            onTriggered: OnLine.downLrc(Base.getFileName(fileUrl), fileUrl);
        }
        TikoMenuItem{
            text: qsTr("从 QQ音乐 下载 歌词")
            onTriggered: OnLine.downLrcFromQQMusic(Base.getFileName(fileUrl), fileUrl);
        }
        TikoMenuItem{
            text: qsTr("从 网易云 下载 歌词")
            onTriggered: OnLine.downLrcFromNetEase(Base.getFileName(fileUrl), fileUrl);
        }
    }

    TikoMenu{
        title: qsTr("格式转换为")
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
        title: qsTr("添加到")
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
<<<<<<< Updated upstream:PlayView/menu/MenuMusic.qml
                text: CoreData.table[aim]["name"]

=======
                text: CoreData.playlist[aim]["name"]
>>>>>>> Stashed changes:qt/PlayView/menu/MenuMusic.qml
            }
        }

        onOpened: {
            addMenuData.clear()
            for(var i=0; i<CoreData.playlist.length; i++){
                addMenuData.append({aim: i})
            }
        }
    }
}
