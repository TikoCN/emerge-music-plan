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

    vessel: GridLayout {
        columns: width / 320
        property var colorDataMode: [
            {text:qsTr("背景颜色"), prop:"backdropColor"},
            {text:qsTr("透明层颜色"), prop:"transparentColor"},
            {text:qsTr("桌面歌词颜色"), prop:"deskLrcColor"},
            {text:qsTr("主题颜色"), prop:"themeColor"},
            {text:qsTr("文本常规颜色"), prop:"textNormalColor"},
            {text:qsTr("按钮常规颜色"), prop:"buttonNormalColor"},
            {text:qsTr("按钮悬停颜色"), prop:"buttonHoverColor"},
            {text:qsTr("按钮按下颜色"), prop:"buttonPressedColor"}
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
