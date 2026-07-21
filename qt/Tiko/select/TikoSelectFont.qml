import QtQuick
import QtQuick.Controls
import Tiko

Rectangle {
    id: root
    property font selectedFont: Qt.font
    radius: 10
    implicitHeight: 300
    implicitWidth: 400
    color: TikoSeit.theme.colorBgView
    property TikoTextLine fontSizeItem: fontSizeItem
    property TikoTextLine fontShowItem: fontShowItem
    property TikoTextLine fontFaimleyItem: fontFaimleyItem
    property TikoTextLine fontPreviewItem: fontPreviewItem
    property double lineHeight: 50

    Grid {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: TikoSeit.subitemSpace
        columns: 2

        TikoTextLine {
            id: fontSizeItem
            text: "Select Font Size"
            level: 0
            width: parent.width * 0.3
            height: root.lineHeight
            horizontalAlignment: Text.AlignHCenter
        }

        TikoSliderH {
            from: 1
            to: 20
            value: selectedFont.pointSize
            width: parent.width * 0.6
            height: lineHeight
            onMoved: selectedFont.pointSize = value
        }

        TikoTextLine {
            id: fontFaimleyItem
            text: "Select Font Faimley"
            level: 0
            width: parent.width * 0.3
            height: root.lineHeight
            horizontalAlignment: Text.AlignHCenter
        }

        TikoButtonCombox {
            data: Qt.fontFamilies()
            width: parent.width * 0.6
            height: lineHeight
            currentText: selectedFont.family
            onCurrentIndexChanged: {
                if (selectedFont.family !== currentText)
                selectedFont.family = currentText
            }
        }

        TikoTextLine {
            id: fontShowItem
            text: "Select Font Show"
            level: 0
            width: parent.width * 0.3
            height: root.lineHeight
            horizontalAlignment: Text.AlignHCenter
        }

        TikoTextLine {
            id: fontPreviewItem
            font: selectedFont
            width: parent.width * 0.6
            height: 200
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
