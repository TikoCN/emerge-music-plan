import QtQuick
import Tiko
import TikoAPI

TikoMenu{
    id: rightMenu
    cascade: true

    TikoMenuItem{
        text: qsTr("播放")
        onTriggered: root.play()
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
        text: qsTr("喜欢")
        icon.source: "qrc:/image/love.png"
    }

    TikoMenuItem{
        text: qsTr("打开文件夹")
        onTriggered: root.core.openMusicDir()
        icon.source: "qrc:/image/dir.png"
    }

    TikoMenuItem{
        text: qsTr("打开歌词")
        onTriggered: root.core.openMusicLrc()
    }

    TikoMenuItem{
        text: qsTr("打开封面")
        onTriggered: root.core.openMusicCover()
    }

    TikoMenuSpeacer{}

    TikoMenuItem{
        text: qsTr("复制 文件路径")
        onTriggered: root.core.copyMusicUrl()
    }

    TikoMenuItem{
        text: qsTr("复制 歌曲信息")
        onTriggered: root.core.copyMusicData()
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
            onTriggered: OnLine.downCover(core.getSearchString(), core.url);
        }
        TikoMenuItem{
            text: qsTr("从 QQ音乐 下载 封面")
            onTriggered: OnLine.downCoverFromQQMusic(core.getSearchString(), core.url);
        }
        TikoMenuItem{
            text: qsTr("从 网易云 下载 封面")
            onTriggered: OnLine.downCoverFromNetEase(core.getSearchString(), core.url);
        }
        TikoMenuItem{
            text: qsTr("从 Bing搜索 下载 封面")
            onTriggered: OnLine.downCoverFromBing(core.getSearchString(), core.url);
        }
        TikoMenuItem{
            text: qsTr("从 Baidu搜索 下载 封面")
            onTriggered: OnLine.downCoverFromBaidu(core.getSearchString(), core.url);
        }
        TikoMenuSpeacer{}
        TikoMenuItem{
            text: qsTr("从 网络中下载 歌词")
            onTriggered: OnLine.downLrc(core.getSearchString(), core.url);
        }
        TikoMenuItem{
            text: qsTr("从 QQ音乐 下载 歌词")
            onTriggered: OnLine.downLrcFromQQMusic(core.getSearchString(), core.url);
        }
        TikoMenuItem{
            text: qsTr("从 网易云 下载 歌词")
            onTriggered: OnLine.downLrcFromNetEase(core.getSearchString(), core.url);
        }
    }

    TikoMenu{
        title: qsTr("格式转换为")
        Repeater{
            delegate: TikoMenuItem{
                text: suffix
                onTriggered: core.setSuffix(suffix);
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
                text: MediaPlayer.tableList[aim].name
                onTriggered: MediaPlayer.tableList[aim].insertMusic(core)
            }
        }

        onOpened: {
            addMenuData.clear()
            for(var i=0; i<MediaPlayer.tableList.length; i++){
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
                text: MediaPlayer.tableList[aim].name
                onTriggered: MediaPlayer.tableMoveMusic(root.tableId, root.listId, aim)
            }
        }

        onOpened: {
            moveMenuData.clear()
            for(var i=0; i<MediaPlayer.tableList.length; i++){
                moveMenuData.append({aim: i})
            }
        }
    }
}
