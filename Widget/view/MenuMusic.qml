import QtQuick
import Tiko
import TikoAPI

TikoMenu{
    id: rightMenu
    cascade: true

    TikoMenuItem{
        text: qsTr("播放")
        onTriggered: musicLine.play()
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
        onTriggered: musicLine.music.openMusicDir()
        icon.source: "qrc:/image/dir.png"
    }

    TikoMenuItem{
        text: qsTr("打开歌词")
        onTriggered: musicLine.music.openMusicLrc()
    }

    TikoMenuItem{
        text: qsTr("打开封面")
        onTriggered: musicLine.music.openMusicCover()
    }

    TikoMenuSpeacer{}

    TikoMenuItem{
        text: qsTr("复制 文件路径")
        onTriggered: musicLine.music.copyMusicUrl()
    }

    TikoMenuItem{
        text: qsTr("复制 歌曲信息")
        onTriggered: musicLine.music.copyMusicData()
    }

    TikoMenuSpeacer{}

    TikoMenuItem{
        text: qsTr("编辑音乐信息")
        onTriggered: CoreData.editMusic(root, root.core)
    }

    TikoMenu{
        title: qsTr("从网络中下载")
        TikoMenuItem{
            text: qsTr("从 网络中下载 封面")
            onTriggered: OnLine.downCover(musicLine.music.getSearchString(), musicLine.music.url);
        }
        TikoMenuItem{
            text: qsTr("从 QQ音乐 下载 封面")
            onTriggered: OnLine.downCoverFromQQMusic(musicLine.music.getSearchString(), musicLine.music.url);
        }
        TikoMenuItem{
            text: qsTr("从 网易云 下载 封面")
            onTriggered: OnLine.downCoverFromNetEase(musicLine.music.getSearchString(), musicLine.music.url);
        }
        TikoMenuItem{
            text: qsTr("从 Bing搜索 下载 封面")
            onTriggered: OnLine.downCoverFromBing(musicLine.music.getSearchString(), musicLine.music.url);
        }
        TikoMenuItem{
            text: qsTr("从 Baidu搜索 下载 封面")
            onTriggered: OnLine.downCoverFromBaidu(musicLine.music.getSearchString(), musicLine.music.url);
        }
        TikoMenuSpeacer{}
        TikoMenuItem{
            text: qsTr("从 网络中下载 歌词")
            onTriggered: OnLine.downLrc(musicLine.music.getSearchString(), musicLine.music.url);
        }
        TikoMenuItem{
            text: qsTr("从 QQ音乐 下载 歌词")
            onTriggered: OnLine.downLrcFromQQMusic(musicLine.music.getSearchString(), musicLine.music.url);
        }
        TikoMenuItem{
            text: qsTr("从 网易云 下载 歌词")
            onTriggered: OnLine.downLrcFromNetEase(musicLine.music.getSearchString(), musicLine.music.url);
        }
    }

    TikoMenu{
        title: qsTr("格式转换为")
        Repeater{
            delegate: TikoMenuItem{
                text: suffix
                onTriggered: musicLine.music.setSuffix(suffix);
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
                onTriggered: Core.tableList[aim].insertMusic(musicLine.music)
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
        icon.source: "qrc:/image/addR.png"

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
                onTriggered: Core.tableMoveMusic(musicLine.music.tableId, musicLine.music.listId, aim)
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
