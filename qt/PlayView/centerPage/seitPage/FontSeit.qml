import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import MediaerAPI
import Tiko
import PlayView

TikoRightVessel{
    Layout.fillWidth: true
    Layout.preferredHeight: this.height
    show.text: qsTr("字体管理")

    vessel: GridLayout {

        columns: width / 500
        property var fontDataMode: [
            {text:qsTr("主要字体"), prop:"mainFont"},
            {text:qsTr("桌面字体"), prop:"deskFont"},
            {text:qsTr("主歌词字体"), prop:"mainLrcFont"},
            {text:qsTr("桌面歌词字体"), prop:"deskLrcFont"},
            {text:qsTr("文本正常字体"), prop:"textNormalFont"},
            {text:qsTr("标题字体"), prop:"titleFont"},
            {text:qsTr("辅助字体"), prop:"assistanceFont"}
        ]
        Repeater {
            model: fontDataMode
            delegate: TikoSelectFont {
                width: 400
                height: 70
                text: modelData.text
                selectedFont: Setting[modelData.prop]

                onSelectedFontChanged:{
                    Setting[modelData.prop] = selectedFont
                }
            }
        }
    }
}
