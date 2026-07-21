import QtQuick.Controls.Basic
import QtQuick
import Tiko

ScrollBar {
    id: root
    height: parent.height
    size: 1
    width: 20
    anchors.right: parent.right
    //snapMode: ScrollBar.SnapAlways
    policy: ScrollBar.AsNeeded
    minimumSize: 0.2
    visible: Math.abs(height - handleItem.height) > 20

    background: Rectangle {  // 背景
        id: background
        color: root.active ? TikoSeit.theme.colorBgDefault : "#00000000"
        radius: 4
    }


    contentItem: Rectangle {  // 滑块
        color: Qt.rgba(0,0,0,0)
        Rectangle {
            id: handleItem
            color: TikoSeit.theme.colorFgDefault
            radius: 3
            width: 6
            height: parent.height
            opacity: 0.6
            anchors.horizontalCenter: parent.horizontalCenter

            states: [
                State {
                    name: "active"
                    when: root.active
                    PropertyChanges {
                        target: handleItem
                        width: 8
                        opacity: 1.0
                    }
                },
                State {
                    name: "inactive"
                    when: !root.active
                    PropertyChanges {
                        target: handleItem
                        width: 6
                        opacity: 0.6
                    }
                }
            ]

            transitions: Transition {
                // 同时动画 width 和 opacity
                ParallelAnimation {
                    NumberAnimation {
                        properties: "width,opacity"
                        duration: 250
                        easing.type: Easing.OutQuad
                    }
                }
            }
        }
    }
}
