import QtQuick
import QtQuick.Controls
import Tiko

Rectangle {
    id: root
    property font selectedFont: Qt.font
    property string text
    color: TikoSeit.theme.baseTheme.backgroundTransition
    border.color: TikoSeit.theme.baseTheme.borderTransition
    border.width: 3
    radius: 10
    implicitHeight: 300
    implicitWidth: 500

    TikoTextTitle {
        id: titleLine
        text: root.text
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: TikoSeit.normalMargins
    }


    TikoButtonCombox {
        id: fontFamiliesCombox
        data: Qt.fontFamilies()
        anchors.left: titleLine.left
        anchors.top: titleLine.bottom
        anchors.topMargin: TikoSeit.subitemSpace
        helpText: qsTr("字体：")
        onCurrentIndexChanged: {
            selectedFont.family = currentText
        }
    }


    TikoButtonCombox {
        data: Array.from({length: 14}, (_, index) => index + 1)
        anchors.right: parent.right
        anchors.rightMargin: TikoSeit.normalMargins
        anchors.top: fontFamiliesCombox.top
        helpText: qsTr("字号：")
        onCurrentIndexChanged: {
            selectedFont.pointSize = currentText
        }
    }

    Rectangle {
        height: 55 * 2
        anchors.top: fontFamiliesCombox.bottom
        anchors.topMargin: TikoSeit.subitemSpace
        anchors.left: titleLine.left
        anchors.right: parent.right
        anchors.rightMargin: TikoSeit.normalMargins
        anchors.bottom: parent.bottom
        anchors.bottomMargin: TikoSeit.normalMargins
        color: TikoSeit.theme.baseTheme.backgroundTransition
        radius: 25

        TikoTextLine {
            font: selectedFont
            anchors.fill: parent
            text: root.text + qsTr("预览文本")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
