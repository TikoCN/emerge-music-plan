import QtQuick
import QtQuick.Layouts
import MediaerAPI
import Tiko

TikoRightVessel {
    Layout.preferredHeight: this.height
    titleButton.textLine.text: qsTr("通用设置")

    vessel: RowLayout {
        width: bgLoader.width
        anchors.margins: TikoSeit.subitemSpace

        TikoTextLine {
            text: qsTr("界面语言")
        }

        TikoButtonComboBox {
            Layout.preferredWidth: 180
            width: 180
            data: [qsTr("简体中文"), qsTr("English")]
            currentIndex: Setting.language === "en" ? 1 : 0
            onCurrentIndexChanged: Setting.setLanguage(currentIndex === 1 ? "en" : "zh_CN")
        }
    }
}
