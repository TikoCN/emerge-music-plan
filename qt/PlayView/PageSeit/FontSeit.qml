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
        width: bgLoader.width

        property var fontDataMode: [
            {text: qsTr("歌词字体"), prop: "lrcFont"},
            {text: qsTr("桌面歌词字体"), prop: "deskLrcFont"},
        ]
        Repeater {
            model: fontDataMode
            delegate: TikoSelectFont {
                selectedFont: Setting[modelData.prop]
                width: bgLoader.width

                fontSizeItem.text: "字体大小"
                fontShowItem.text: "字体预览"
                fontFaimleyItem.text: "字体家族"
                fontPreviewItem.text: modelData.text

                onSelectedFontChanged:{
                    Setting[modelData.prop] = selectedFont
                }
            }
        }
    }
}
