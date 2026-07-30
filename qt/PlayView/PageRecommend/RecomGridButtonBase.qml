import QtQuick
import Tiko
import PlayView
import MediaerAPI

Item {
    id: coreButtonGird

    property Component delegateItem
    property string text: ""

    TikoTextLine {
        id: gridText
        width: coreButtonGird.width
        text: coreButtonGird.text
        level: 2
    }

    Loader {
        id: gridItem
        anchors.top: gridText.bottom
        anchors.topMargin: 6
        width: coreButtonGird.width
        sourceComponent: delegateItem
    }
}
