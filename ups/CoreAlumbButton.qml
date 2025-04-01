import QtQuick
import Tiko
import DataCore
import MediaerAPI

Item{
    id: alumbButton
    width: 160
    height: showItem.height + 10

    property AlumbData alumb
    property int coverId: alumb.musicList[0].coreId

    // 整体背景
    Rectangle {
        anchors.fill: parent
        color: Setting.transparentColor
        radius: 10
        opacity: mouse.containsMouse ? 0.1 : 0
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: CoreData.mainTurnAlumbPlayer(alumb)
        hoverEnabled: true

        Item {
            id: showItem
            height: childrenRect.height
            x: 5
            y: 5
            width: parent.width - 10

            TikoImageAuto {
                id: alumbCover
                width: parent.width
                height: parent.width
                normalUrl: "qrc:/image/alumb.png"
                loadUrl: "image://cover/file:" + alumbButton.coverId.toString()
            }

            // 播放按钮
            TikoButtonIcon {
                width: alumbCover.width * 0.2
                height: width
                radius: width / 2
                icon.source: "qrc:/image/play.png"
                icon.width: width / 2
                icon.height: height / 2
                anchors.bottom: alumbCover.bottom
                anchors.left: alumbCover.left
                anchors.margins: alumbCover.width * 0.05
                visible: mouse.containsMouse
                normal: 0.5
                hover: 1
                borderSize: 1.5
                autoColor: Setting.backdropColor
            }

            // 菜单按钮
            TikoButtonIcon {
                width: alumbCover.width * 0.2
                height: width
                radius: width / 2
                icon.source: "qrc:/image/more.png"
                icon.width: width / 2
                icon.height: height / 2
                anchors.top: alumbCover.top
                anchors.right: alumbCover.right
                anchors.margins: alumbCover.width * 0.05
                visible: mouse.containsMouse
                normal: 0.5
                hover: 1
                borderSize: 1.5
                autoColor: Setting.backdropColor
                onClicked: CoreData.openMenuAlumb(this, alumb)
            }

            // 图片上背景
            Rectangle {
                id: textBack
                width: alumbCover.width * 0.7
                height: alumbCover.height / 5
                anchors.bottom: alumbCover.bottom
                anchors.right: alumbCover.right
                topLeftRadius: 10
                bottomRightRadius: 10
                opacity: 0.6
                color: Setting.transparentColor
            }

            // 图片上文字
            TikoTextLine {
                anchors.fill: textBack
                font.pixelSize: 13
                font.bold: true
                color: Setting.backdropColor
                opacity: 0.7
                text: alumb.getArtist()
                horizontalAlignment: Text.AlignHCenter
            }

            // 专辑名
            TikoTextLine {
                id: textLine
                anchors.top: alumbCover.bottom
                anchors.topMargin: 10
                width: parent.width
                text: alumb.name
            }

            // 专辑时长
            TikoTextLine {
                id: timeLine
                anchors.top: textLine.bottom
                width: parent.width
                opacity: 0.4
                text: alumb.getStringTime()
            }
        }
    }
}
