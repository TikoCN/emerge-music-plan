import QtQuick
import Tiko
import MediaerAPI   
import Widget

Item{
    id: alumbDataShow
    property var alumb

    TikoButtonIcon{
        y: -10
        icon.source: "qrc:/image/back.png"
        onClicked: mainView.turnToAlumb()
    }

    // 专辑信息背景
    Rectangle {
        id: alumbDataBack
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 20
        width: parent.width - 60
        height: 270
        color: TikoSeit.backdropColor
        radius: 15
    }

    Image {
        id: alumbDataCover
        width: 250
        height: 250
        sourceSize.width: width
        sourceSize.height: height
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 30
    }

    TikoTextLine {
        id: alumbText
        width: parent.width - alumbDataCover.width - 60
        anchors.left: alumbDataCover.right
        anchors.leftMargin: 30
        anchors.top: alumbDataCover.top
        exSize: 20
    }

    TikoTextLine {
        id: alumbHelp
        width: parent.width - alumbDataCover.width
        anchors.top: alumbText.bottom
        anchors.topMargin: 10
        anchors.left: alumbText.left
        exSize: 5
        opacity: 0.5
    }

    // 音乐列表
    ListView {
        id: musicList
        anchors.top: alumbDataCover.bottom
        anchors.left: parent.left
        anchors.margins: 30
        width: parent.width - 60
        height: parent.height - alumbDataBack.height - 40
        clip: true

        model: ListModel {
            id: alumbMusicList
        }

        delegate: CoreMusicLine {
            width: musicList.width
            listId: musicListId
            music: musicCore
            alumb: alumbDataShow.alumb
            type: 1
        }
    }

    function openAlumbData (alumb) {
        var allTime = 0
        alumbDataCover.source = "image://cover/file:" +  alumb.musicList[0].coreId.toString()
        alumbMusicList.clear()
        for (var i=0; i<alumb.musicList.length; i++) {
            alumbMusicList.append({
                                      musicListId: i,
                                      musicCore: alumb.musicList[i]
                                  })
            allTime += alumb.musicList[i].endTime
        }
        alumbText.text = alumb.name
        alumbHelp.text = alumb.musicList.length.toString()+" "+qsTr("首歌曲") +"-"
                +alumb.getStringTime()+" "+qsTr("歌曲长度")
        alumbDataShow.alumb = alumb
    }
}
