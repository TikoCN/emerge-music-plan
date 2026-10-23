import QtQuick
import Tiko
import DataType
import MediaerAPI
import PlayView

TikoButtonBase {
    id: mouseArea
    onLeftClicked: page()
    onRightClicked: position => menu(position)
    implicitHeight: columnView.height + 10
    property string name: ""
    property string normalIcon: ""
    property string loadIcon: ""
    property string subtitle: ""
    property bool scrolling: false
    isHover: containsMouse && !scrolling

    signal menu(point position) // 菜单
    signal page() // 跳到页面
    signal play() // 播放

    states: [
        State {
            name: "active"
            when: isHover
            PropertyChanges {
                target: columnView
                y: 0
            }
        },
        State {
            name: "inactive"
            when: !isHover
            PropertyChanges {
                target: columnView
                y: 10
            }
        }
    ]

    transitions: Transition {
            enabled: !mouseArea.scrolling
            NumberAnimation {
                properties: "y"
                duration: 250
                easing.type: Easing.OutQuad
            }
    }

    Column {
        id: columnView
        width: Math.max(0, parent.width - TikoSeit.subitemSpace * 4)
        anchors.horizontalCenter: parent.horizontalCenter

        //封面显示区
        AutoCoverImage {
            id: coverItem
            width: parent.width
            height: width
            normalUrl: normalIcon
            baseUrl: loadIcon
            loadingPaused: mouseArea.scrolling
        }

        // 名
        TikoTextLine {
            id: textLine
            text: name
            level: 2
            height: 30
            leftPadding: 10
            width: parent.width
        }

        // 时长
        TikoTextLine {
            id: timeLine
            text: subtitle
            level: 0
            height: 30
            leftPadding: 10
            width: parent.width
        }
    }
}
