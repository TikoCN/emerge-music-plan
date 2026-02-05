import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import MediaerAPI
import Tiko
import PlayView

TikoRightVessel{
    Layout.preferredHeight: this.height
    titleButton.textLine.text: qsTr("字体管理")
    id: fontSeit

    vessel: ColumnLayout {

        property var fontDataMode: [
            {text: qsTr("歌词字体"), prop: "lrcFont"},
            {text: qsTr("桌面歌词字体"), prop: "deskLrcFont"},
            {text: qsTr("常规文本字体"), prop: "textNormalFont"},
            {text: qsTr("标题字体"), prop: "titleFont"},
            {text: qsTr("副标题字体"), prop: "subtitleFont"},
            {text: qsTr("信息字体"), prop: "informationFont"},
            {text: qsTr("辅助字体"), prop: "assistanceFont"}
        ]
        Repeater {
            model: fontDataMode
            delegate: TikoSelectFont {
                width: parent.width
                text: modelData.text
                selectedFont: Setting[modelData.prop]

                onSelectedFontChanged:{
                    Setting[modelData.prop] = selectedFont
                }
            }
        }
    }
}
