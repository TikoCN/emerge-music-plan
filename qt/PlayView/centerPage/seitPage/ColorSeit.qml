import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import MediaerAPI
import Tiko
import PlayView

TikoRightVessel {
    Layout.minimumHeight: height
    show.text: qsTr("颜色管理")

    vessel: GridLayout {
        columns: width / 320
        property var colorDataMode: [
            // 基础颜色
            {text: qsTr("主题颜色"), prop: "themeColor"},
            {text: qsTr("透明层颜色"), prop: "transparentColor"},
            {text: qsTr("背景颜色"), prop: "backdropColor"},

            // 歌词颜色
            {text: qsTr("歌词常规颜色"), prop: "lrcNormalColor"},
            {text: qsTr("歌词播放颜色"), prop: "lrcPlayingColor"},
            {text: qsTr("桌面歌词颜色"), prop: "deskLrcColor"},

            // 文本颜色
            {text: qsTr("文本常规颜色"), prop: "textNormalColor"},
            {text: qsTr("标题文本颜色"), prop: "textTitleColor"},
            {text: qsTr("副标题文本颜色"), prop: "textSubtitleColor"},
            {text: qsTr("信息文本颜色"), prop: "textInformationColor"},
            {text: qsTr("辅助文本颜色"), prop: "textAssistanceColor"},
            {text: qsTr("禁用文本颜色"), prop: "textDisabledColor"},

            // 按钮颜色
            {text: qsTr("按钮常规颜色"), prop: "buttonNormalColor"},
            {text: qsTr("按钮悬停颜色"), prop: "buttonHoverColor"},
            {text: qsTr("按钮按下颜色"), prop: "buttonPressedColor"},
            {text: qsTr("按钮禁用颜色"), prop: "buttonDisabledColor"}
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
