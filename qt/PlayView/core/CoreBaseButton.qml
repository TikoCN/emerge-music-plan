import QtQuick
import Tiko
import DataType
import MediaerAPI
import PlayView

Item {
    id: coreButton
    height: childrenRect.height

    property string name: ""
    property string normalIcon: ""
    property string loadIcon: ""
    property string subtitle: ""
    property int space: 10

    signal menu()
    signal page()
    signal play()

    TikoButtonBase {
        id: mouseArea
        onLeftClicked: page()
        onRightClicked: menu()
        width: parent.width
        height: coverItem.height + textLine.height + timeLine.height + coreButton.space + TikoSeit.subitemSpace

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
            x: coreButton.space
            id: coverItem
            width: parent.width
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            normalUrl: normalIcon
            loadUrl: loadIcon

            PropertyAnimation on y {
                id: upAnim
                from: coreButton.space
                to: 0
                duration: 300
            }

            PropertyAnimation on y {
                id: downAnim
                from: 0
                to: coreButton.space
                duration: 300
            }

            // 播放按钮
            TikoButtonIconNormal {
                icon.source: "qrc:/image/play.png"
                icon.width: width / 2
                icon.height: height / 2
                anchors.bottom: coverItem.bottom
                anchors.left: coverItem.left
                anchors.margins: coverItem.width * 0.05
                visible: mouseArea.dynamicState.isHover
                onClicked: play()
            }

            // 菜单按钮
            TikoButtonIconNormal {
                icon.source: "qrc:/image/more.png"
                icon.width: width / 2
                icon.height: height / 2
                anchors.top: coverItem.top
                anchors.right: coverItem.right
                anchors.margins: coverItem.width * 0.05
                visible: mouseArea.dynamicState.isHover
                onClicked: menu()
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
}
