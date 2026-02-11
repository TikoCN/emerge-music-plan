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
        data: [12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 24, 26 ,28, 30]
        anchors.right: parent.right
        anchors.rightMargin: TikoSeit.normalMargins
        anchors.top: fontFamiliesCombox.top
        helpText: qsTr("字号：")
        onCurrentIndexChanged: {
            selectedFont.pointSize = currentText
        }
    }

    TikoTextLine {
        font: selectedFont
        height: 55 * 2
        anchors.top: fontFamiliesCombox.bottom
        anchors.topMargin: TikoSeit.subitemSpace
        anchors.left: titleLine.left
        anchors.right: parent.right
        anchors.rightMargin: TikoSeit.normalMargins
        anchors.bottom: parent.bottom
        anchors.bottomMargin: TikoSeit.normalMargins

        text: root.text + qsTr("预览文本")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter


        Rectangle {
            anchors.fill: parent
            color: TikoSeit.theme.baseTheme.backgroundTransition
            radius: 25
        }
    }
}
