import QtQuick
import Widget
import Tiko
import ControlAPI

TikoMenu{
    id: menuMusic
    cascade: true
    onClosed: menuMusic.destroy()
    property var musicLine
    property var music

    TikoMenuItem{
        text: qsTr("播放")
        onTriggered: menuMusic.musicLine.play()
        icon.source: "qrc:/image/playBlack.png"
    }

    TikoMenuItem{
        text: qsTr("下一首播放")
        onTriggered: MediaPlayer.playingInsertMusic(coreId)
    }

    TikoMenuItem{
        text: qsTr("添加到播放")
        onTriggered: MediaPlayer.musicInsertPlayingTable(coreId)
    }

    TikoMenuSpeacer{}

    TikoMenuItem{
        text: qsTr("打开文件夹")
        onTriggered: menuMusic.music.openMusicDir()
        icon.source: "qrc:/image/dir.png"
    }

    TikoMenuItem{
        text: qsTr("打开歌词")
        onTriggered: menuMusic.music.openMusicLrc()
    }

    TikoMenuItem{
        text: qsTr("打开封面")
        onTriggered: menuMusic.music.openMusicCover()
    }

    TikoMenuSpeacer{}

    TikoMenuItem{
        text: qsTr("复制 文件路径")
        onTriggered: menuMusic.music.copyMusicUrl()
    }

    TikoMenuItem{
        text: qsTr("复制 歌曲信息")
        onTriggered: menuMusic.music.copyMusicData()
    }

    TikoMenuSpeacer{}

    TikoMenuItem{
        text: qsTr("编辑音乐信息")
        onTriggered: CoreData.editMusic(menuMusic.musicLine, menuMusic.music)
    }

    TikoMenu{
        title: qsTr("从网络中下载")
        TikoMenuItem{
            text: qsTr("从 网络中下载 封面")
            onTriggered: OnLine.downCover(menuMusic.music.getSearchString(), menuMusic.music.url);
        }
        TikoMenuItem{
            text: qsTr("从 QQ音乐 下载 封面")
            onTriggered: OnLine.downCoverFromQQMusic(menuMusic.music.getSearchString(), menuMusic.music.url);
        }
        TikoMenuItem{
            text: qsTr("从 网易云 下载 封面")
            onTriggered: OnLine.downCoverFromNetEase(menuMusic.music.getSearchString(), menuMusic.music.url);
        }
        TikoMenuItem{
            text: qsTr("从 Bing搜索 下载 封面")
            onTriggered: OnLine.downCoverFromBing(menuMusic.music.getSearchString(), menuMusic.music.url);
        }
        TikoMenuItem{
            text: qsTr("从 Baidu搜索 下载 封面")
            onTriggered: OnLine.downCoverFromBaidu(menuMusic.music.getSearchString(), menuMusic.music.url);
        }
        TikoMenuSpeacer{}
        TikoMenuItem{
            text: qsTr("从 网络中下载 歌词")
            onTriggered: OnLine.downLrc(menuMusic.music.getSearchString(), menuMusic.music.url);
        }
        TikoMenuItem{
            text: qsTr("从 QQ音乐 下载 歌词")
            onTriggered: OnLine.downLrcFromQQMusic(menuMusic.music.getSearchString(), menuMusic.music.url);
        }
        TikoMenuItem{
            text: qsTr("从 网易云 下载 歌词")
            onTriggered: OnLine.downLrcFromNetEase(menuMusic.music.getSearchString(), menuMusic.music.url);
        }
    }

    TikoMenu{
        title: qsTr("格式转换为")
        Repeater{
            delegate: TikoMenuItem{
                text: suffix
                onTriggered: menuMusic.music.setSuffix(suffix);
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
                onTriggered: Core.tableList[aim].insertMusic(menuMusic.music)
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
                onTriggered: Core.tableMoveMusic(menuMusic.music.tableId, menuMusic.music.listId, aim)
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
