import QtQuick
import Tiko
import PlayView
import MediaerAPI

Item {
    id: coreButtonGird

    property Component delegateItem
    property string text: ""
    property bool hideWhenEmpty: true

    height: visible ? gridItem.y + gridItem.height : 0
    visible: !hideWhenEmpty || (gridItem.item !== null && gridItem.item.count > 0)

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
