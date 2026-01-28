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
    show.text: qsTr("选项管理")
    show.width: seitPage.modeTextWidth
    vessel: GridLayout {
        anchors.margins: CoreData.cellItemSpace
        columns: width/300
        clip: true

        property var switchDataMode: [
                // 在线模块
                {text: qsTr("在线模块"),prop: "isOnLine"},
                // 封面获取组
                {text: qsTr("从网易获得封面"),prop: "isGetCoverFromNetEase"},
                {text: qsTr("从QQ音乐获得封面"),prop: "isGetCoverFromQQMusic"},
                {text: qsTr("从百度获得封面"), prop: "isGetCoverFromBaidu"},
                {text: qsTr("从必应获得封面"),prop: "isGetCoverFromBing"},
                // 歌词获取组
                {text: qsTr("从QQ音乐获得歌词"), prop: "isGetLrcFromQQMusic"},
                {text: qsTr("从网易获得歌词"),prop: "isGetLrcFromNetEase"}

        ]

        Repeater {
            model: switchDataMode
            delegate: TikoButtonSwitch{
                width: 300
                text: modelData.text
                check: Setting[modelData.prop]
                onCheckChanged: Setting[modelData.prop] = check
            }
        }
    }
}
