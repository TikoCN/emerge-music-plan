import QtQuick
import Tiko
import DataType
import MediaerAPI
import PlayView

TikoButtonBase {
    id: mouseArea
    onLeftClicked: page()
    onRightClicked: menu()
    implicitHeight: childrenRect.height
    property string name: ""
    property string normalIcon: ""
    property string loadIcon: ""
    property string subtitle: ""

    signal menu() // 菜单
    signal page() // 跳到页面
    signal play() // 播放

    states: [
        State {
            name: "active"
            when: isHover
            PropertyChanges {
                target: mouseArea
                y: -10
            }
        },
        State {
            name: "inactive"
            when: !isHover
            PropertyChanges {
                target: mouseArea
                y: 0
            }
        }
    ]

    transitions: Transition {
        // 同时动画 width 和 opacity
            NumberAnimation {
                properties: "y"
                duration: 250
                easing.type: Easing.OutQuad
            }
    }

    Column {
        width: parent.width - TikoSeit.subitemSpace * 4
        anchors.horizontalCenter: parent.horizontalCenter

        //封面显示区
        AutoCoverImage {
            id: coverItem
            width: parent.width
            height: width
            normalUrl: normalIcon
            baseUrl: loadIcon
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
