import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import MediaerAPI
import Tiko
import PlayView

TikoRightVessel {
    Layout.fillWidth: true
    Layout.minimumHeight: height
    show.text: qsTr("颜色管理")

    vessel: ColumnLayout {
        property var colorDataMode: [
            {text:qsTr("背景颜色"), prop:"backdropColor"},
            {text:qsTr("透明层颜色"), prop:"transparentColor"},
            {text:qsTr("桌面歌词颜色"), prop:"deskLrcColor"}
        ]
        Repeater {
            model: colorDataMode
            delegate: TikoSelectColor{
                width: 300
                text: modelData.text
                selectedColor: Setting[modelData.prop]
                onSelectedColorChanged: {
                    Setting[modelData.prop] = selectedColor
                }
            }
        }
    }
}
