import QtQuick
import PlayView
import Tiko
import MediaerAPI
import DataType

TikoMenu{
    id: menuMusic
    cascade: true
    onClosed: menuMusic.destroy()

    property MusicData music: Core.getMusic(musicId)
    property int musicId: -1
    property int type: 0

    TikoMenuItem{
        text: qsTr("播放")
        onTriggered: MediaPlayer.buildMusic(musicId)
        icon.source: "qrc:/image/play.png"
    }

    TikoMenuItem{
        text: qsTr("下一首播放")
        onTriggered: MediaPlayer.insertMusic(musicId)
    }

    TikoMenuItem{
        text: qsTr("添加到播放队列")
        onTriggered: MediaPlayer.appendMusic(musicId)
    }

    TikoMenuSpeacer{}

    TikoMenuItem{
        text: qsTr("打开文件夹")
        onTriggered: {
            var url = Base.getParentDir(music.url)
            Base.deskOpenFile(url)
        }
        icon.source: "qrc:/image/dir.png"
    }

    TikoMenuItem{
        text: qsTr("打开歌词")
        onTriggered: Base.deskOpenFile(music.getLrcUrl())
    }

    TikoMenuItem{
        text: qsTr("打开封面")
        onTriggered: {
            var url = Base.getBaseUrl(music.url) + ".jpg"
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
        onTriggered: Base.copyString(music.url)
    }

    TikoMenuItem{
        text: qsTr("复制 歌曲信息")
        onTriggered: {
            var str = qsTr("标题") +" " + music.title +
                    qsTr("专辑") +" " + music.album +
                    qsTr("作曲家") +" " + music.artist
            Base.copyString(str)
        }
    }

    TikoMenuSpeacer{}

    TikoMenuItem{
        text: qsTr("编辑音乐信息")
        onTriggered: CoreData.editMusic(menuMusic.parent, menuMusic.music)
    }

    TikoMenu{
        title: qsTr("从网络中下载")
        TikoMenuItem{
            text: qsTr("从 网络中下载 封面")
            onTriggered: OnLine.downMusicCover(Base.getFileName(music.url), music.url);
        }
        TikoMenuItem{
            text: qsTr("从 QQ音乐 下载 封面")
            onTriggered: OnLine.downMusicCoverQQMusic(Base.getFileName(music.url), music.url);
        }
        TikoMenuItem{
            text: qsTr("从 网易云 下载 封面")
            onTriggered: OnLine.downMusicCoverNetEase(Base.getFileName(music.url), music.url);
        }
        TikoMenuItem{
            text: qsTr("从 Bing搜索 下载 封面")
            onTriggered: OnLine.downMusicCoverBing(Base.getFileName(music.url), music.url);
        }
        TikoMenuItem{
            text: qsTr("从 Baidu搜索 下载 封面")
            onTriggered: OnLine.downCoverBaidu(Base.getFileName(music.url), music.url);
        }
        TikoMenuSpeacer{}
        TikoMenuItem{
            text: qsTr("从 网络中下载 歌词")
            onTriggered: OnLine.downLrc(Base.getFileName(music.url), music.url);
        }
        TikoMenuItem{
            text: qsTr("从 QQ音乐 下载 歌词")
            onTriggered: OnLine.downLrcFromQQMusic(Base.getFileName(music.url), music.url);
        }
        TikoMenuItem{
            text: qsTr("从 网易云 下载 歌词")
            onTriggered: OnLine.downLrcFromNetEase(Base.getFileName(music.url), music.url);
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
                text: CoreData.table[aim]["name"]

            }
        }

        onOpened: {
            addMenuData.clear()
            for(var i=0; i<CoreData.table.length; i++){
                addMenuData.append({aim: i})
            }
        }
    }
}
