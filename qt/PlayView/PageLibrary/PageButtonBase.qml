import QtQuick
import QtQuick.Layouts
import Tiko
import PlayView

Item {
    property Component delegateItem
    property string currentKey: ""
    property alias loader: loaderItem

    signal initKeyList()

    // 跳转按钮列表
    ListView {
        id: keyListView
        orientation: ListView.Horizontal
        width: parent.width
        currentIndex: 0
        height: 40
        anchors.left: parent.left
        anchors.margins: 30
        highlightRangeMode: ListView.ApplyRange
        preferredHighlightBegin: width / 4
        preferredHighlightEnd: width / 4
        snapMode: ListView.SnapToItem

        model: ListModel {
            id: keyModel
        }

        delegate: TikoButtonDefault {
            textLine.text: model.key
            height: 40
            opacity: ListView.isCurrentItem ? 1 : 0.3
            onClicked: {
                keyListView.currentIndex = model.id
                currentKey = model.key
            }
        }
    }

    Loader {
        id: loaderItem
        anchors.top: keyListView.bottom
        anchors.topMargin: TikoSeit.emphasizeMargins
        anchors.bottom: parent.bottom
        width: parent.width
        sourceComponent: delegateItem
    }

    function init() {
        keyModel.clear()
        initKeyList()
    }

    function listToKeyModel(list){
        if (list.length <= 0) return

        for (let i = 0; i < list.length; i++) {
            keyModel.append({key: list[i], id:i})
        }

        currentKey = list[0]
    }

    Component.onCompleted: init()

    onCurrentKeyChanged: {
        loader.item.reset()
    }
}
