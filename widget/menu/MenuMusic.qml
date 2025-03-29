import QtQuick
import Widget
import Tiko
import ControlAPI
import DataCore

TikoMenu{
    id: menuMusic
    cascade: true
    onClosed: menuMusic.destroy()

    property MusicData music
    property int type: 0

    TikoMenuItem{
        text: qsTr("播放")
        onTriggered: {
            MediaPlayer.appendPlayMusic(music)
            MediaPlayer.playMusicListId(MediaPlayer.musicList.length - 1)
        }
        icon.source: "qrc:/image/play.png"
    }

    TikoMenuItem{
        text: qsTr("下一首播放")
        onTriggered: MediaPlayer.appendMusic(music)
    }

    TikoMenuItem{
        text: qsTr("添加到播放队列")
        onTriggered: MediaPlayer.insertMusic(music)
    }

    TikoMenuSpeacer{}

    TikoMenuItem{
        text: qsTr("打开文件夹")
        onTriggered: music.openMusicDir()
        icon.source: "qrc:/image/dir.png"
    }

    TikoMenuItem{
        text: qsTr("打开歌词")
        onTriggered: music.openMusicLrc()
    }

    TikoMenuItem{
        text: qsTr("打开封面")
        onTriggered: music.openMusicCover()
    }

    TikoMenuSpeacer{}

    TikoMenuItem{
        text: qsTr("复制 文件路径")
        onTriggered: music.copyMusicUrl()
    }

    TikoMenuItem{
        text: qsTr("复制 歌曲信息")
        onTriggered: music.copyMusicData()
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
            onTriggered: OnLine.downCover(music.getSearchString(), music.url);
        }
        TikoMenuItem{
            text: qsTr("从 QQ音乐 下载 封面")
            onTriggered: OnLine.downCoverFromQQMusic(music.getSearchString(), music.url);
        }
        TikoMenuItem{
            text: qsTr("从 网易云 下载 封面")
            onTriggered: OnLine.downCoverFromNetEase(music.getSearchString(), music.url);
        }
        TikoMenuItem{
            text: qsTr("从 Bing搜索 下载 封面")
            onTriggered: OnLine.downCoverFromBing(music.getSearchString(), music.url);
        }
        TikoMenuItem{
            text: qsTr("从 Baidu搜索 下载 封面")
            onTriggered: OnLine.downCoverFromBaidu(music.getSearchString(), music.url);
        }
        TikoMenuSpeacer{}
        TikoMenuItem{
            text: qsTr("从 网络中下载 歌词")
            onTriggered: OnLine.downLrc(music.getSearchString(), music.url);
        }
        TikoMenuItem{
            text: qsTr("从 QQ音乐 下载 歌词")
            onTriggered: OnLine.downLrcFromQQMusic(music.getSearchString(), music.url);
        }
        TikoMenuItem{
            text: qsTr("从 网易云 下载 歌词")
            onTriggered: OnLine.downLrcFromNetEase(music.getSearchString(), music.url);
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
                text: Core.tableList[aim].name
                onTriggered: Core.tableList[aim].insertMusic(music)
                enabled: Core.tableList[aim].isDir
            }
        }

        onOpened: {
            addMenuData.clear()
            for(var i=0; i<Core.tableList.length; i++){
                addMenuData.append({aim: i})
            }
        }
    }

    TikoMenu{
        title: qsTr("移动到")
        icon.source: "qrc:/image/move.png"
        enabled: type === 0

        Repeater{
            delegate: moveMenu
            model: ListModel{
                id: moveMenuData
            }
        }
        Component {
            id: moveMenu
            TikoMenuItem {
                text: Core.tableList[aim].name
                onTriggered: Core.tableMoveMusic(music, aim)
                enabled: !Core.tableList[aim].isDir
            }
        }

        onOpened: {
            moveMenuData.clear()
            for(var i=0; i<Core.tableList.length; i++){
                moveMenuData.append({aim: i})
            }
        }
    }
}
