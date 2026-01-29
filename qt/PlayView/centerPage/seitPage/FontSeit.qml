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
            {text:qsTr("ui字体"), prop:"mainFont"},
            {text:qsTr("桌面滚动歌词字体"), prop:"deskFont"},
            {text:qsTr("主页滚动歌词字体"), prop:"mainLrcFont"}
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
