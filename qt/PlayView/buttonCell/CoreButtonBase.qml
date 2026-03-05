import QtQuick
import Tiko
import DataType
import MediaerAPI
import PlayView

TikoButtonBase {
    id: mouseArea
    onLeftClicked: page()
    onRightClicked: menu()
    width: parent.width
    height: coverItem.height + textLine.height + timeLine.height + mouseArea.space + TikoSeit.subitemSpace

    property string name: ""
    property string normalIcon: ""
    property string loadIcon: ""
    property string subtitle: ""
    property int space: 10

    signal menu() // 菜单
    signal page() // 跳到页面
    signal play() // 播放
    property Rectangle hoverItem: null

    dynamicState.onIsHoverChanged: {
        upAnim.stop()
        downAnim.stop()

        if (dynamicState.isHover) {
            upAnim.start()
        }
        else {
            downAnim.start()
        }
    }

    //封面显示区
    AutoCoverImage {
        x: mouseArea.space
        id: coverItem
        width: parent.width
        height: width
        anchors.horizontalCenter: parent.horizontalCenter
        normalUrl: normalIcon
        baseUrl: loadIcon

        PropertyAnimation on y {
            id: upAnim
            from: mouseArea.space
            to: 0
            duration: 300
        }

        PropertyAnimation on y {
            id: downAnim
            from: 0
            to: mouseArea.space
            duration: 300
        }

        Loader {
            anchors.fill: coverItem
            sourceComponent: mouseArea.dynamicState.isHover ? hoverItemCom : null

            Component {
                id: hoverItemCom

                Rectangle {
                    color: TikoSeit.theme.baseTheme.transparentAntiEmphasize
                    radius: 10

                    // 播放按钮
                    TikoButtonIconNormal {
                        icon.source: "qrc:/image/play.png"
                        icon.width: width / 2
                        icon.height: height / 2
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.margins: parent.width * 0.05
                        onClicked: play()
                    }

                    // 菜单按钮
                    TikoButtonIconNormal {
                        icon.source: "qrc:/image/more.png"
                        icon.width: width / 2
                        icon.height: height / 2
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.margins: parent.width * 0.05
                        onClicked: menu()
                    }
                }
            }
        }
    }

    // 名
    TikoTextTitle {
        id: textLine
        anchors.bottom: timeLine.top
        anchors.left: coverItem.left
        width: parent.width
        text: name
    }

    // 时长
    TikoTextSubtitle {
        id: timeLine
        anchors.bottom: mouseArea.bottom
        anchors.left: coverItem.left
        width: parent.width
        text: subtitle
    }
}
