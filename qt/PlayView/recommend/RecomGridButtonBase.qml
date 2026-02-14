import QtQuick
import Tiko
import PlayView
import MediaerAPI

Item {
    id: coreButtonGird

    property Component delegateItem
    property string text: ""

    TikoTextTitle {
        id: gridText
        width: coreButtonGird.width
        text: coreButtonGird.text
    }

    Loader {
        id: gridItem
        anchors.top: gridText.bottom
        anchors.topMargin: 6
        width: coreButtonGird.width
        sourceComponent: delegateItem
    }
}
