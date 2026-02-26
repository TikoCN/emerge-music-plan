import QtQuick
import PlayView
import Tiko
import MediaerAPI

Item{
    id: style
    property string artist: qsTr("标题")
    property string title: qsTr("标题")
    property string icon: ""
    property bool show: false

    BackImage {
        anchors.fill: parent
        source: style.icon
    }

    AudioVsualizationCircle {
        id: canvas
        anchors.fill: style
    }

    Item {
        width: childrenRect.width
        height: childrenRect.height
        anchors.centerIn: canvas

        TikoTextLine{
            id: titleItem
            text: title
            anchors.horizontalCenter: positionControl.horizontalCenter
        }

        TikoTextLine{
            id: artistItem
            text: artist
            anchors.top: titleItem.bottom
            anchors.horizontalCenter: positionControl.horizontalCenter
        }

        PositionControl{
            id: positionControl
            anchors.top: artistItem.bottom
            anchors.margins: TikoSeit.emphasizeMargins
            width: canvas.r
        }

        PlayDown {
            id: playDown
            anchors.top: positionControl.bottom
            anchors.left: playState.right
            anchors.margins: TikoSeit.emphasizeMargins
        }

        PlayState {
            id: playState
            anchors.horizontalCenter: positionControl.horizontalCenter
            anchors.top: positionControl.bottom
            anchors.margins: TikoSeit.emphasizeMargins
        }

        PlayUp {
            id: playUp
            anchors.top: positionControl.bottom
            anchors.right: playState.left
            anchors.margins: TikoSeit.emphasizeMargins
        }
    }
}
