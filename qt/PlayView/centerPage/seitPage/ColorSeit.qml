import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import MediaerAPI
import Tiko
import PlayView

TikoRightVessel {
    Layout.minimumHeight: height
    titleButton.textLine.text: qsTr("颜色管理")

    vessel: GridLayout {
        columns: width / 200
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
            {text: qsTr("文本颜色"), prop: "textBenchmarkColor"},
            {text: qsTr("按钮颜色"), prop: "buttonBenchmarkColor"},
        ]
        Repeater {
            model: colorDataMode
            delegate: TikoSelectColor{
                width: 200
                text: modelData.text
                selectedColor: Setting[modelData.prop]
                onSelectedColorChanged: {
                    Setting[modelData.prop] = selectedColor
                }
            }
        }
    }
}
