import QtQuick
import QtQuick.Layouts
import Tiko
import PlayView

Item {
    property Component delegateItem
    property string currentKey: ""
    property alias loader: loaderItem
    readonly property bool hasKeys: keyModel.count > 0
    readonly property bool hasItems: loaderItem.item !== null && loaderItem.item.count > 0

    signal initKeyList()

    // 跳转按钮列表
    ListView {
        id: keyListView
        visible: hasKeys
        orientation: ListView.Horizontal
        currentIndex: -1
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 30
        anchors.rightMargin: 30
        highlightRangeMode: ListView.ApplyRange
        preferredHighlightBegin: width / 4
        preferredHighlightEnd: width / 4
        snapMode: ListView.NoSnap

        model: ListModel {
            id: keyModel
        }

        delegate: TikoButtonDefault {
            textLine.text: model.key
            height: 40
            checked: ListView.isCurrentItem
            textLine.color: checked ? checkedColor : TikoSeit.theme.colorFgDefault
            opacity: checked ? 1 : 0.6
            onClicked: {
                keyListView.currentIndex = model.id
                currentKey = model.key
            }
        }
    }

    Loader {
        id: loaderItem
        visible: hasKeys && hasItems
        anchors.top: keyListView.bottom
        anchors.topMargin: TikoSeit.emphasizeMargins
        anchors.bottom: parent.bottom
        width: parent.width
        sourceComponent: delegateItem
    }

    Text {
        anchors.centerIn: parent
        visible: !hasKeys || !hasItems
        text: qsTr("暂无数据")
        color: TikoSeit.theme.colorTextDefault
        font.pixelSize: 18
    }

    /**
     * @brief 重新加载分类并清除原有选中状态。
     */
    function init() {
        keyModel.clear()
        keyListView.currentIndex = -1
        currentKey = ""
        initKeyList()
    }

    /**
     * @brief 填充分类列表，并选中首项以加载对应内容。
     * @param list 分类关键词列表。
     */
    function listToKeyModel(list){
        if (list.length <= 0) return

        for (let i = 0; i < list.length; i++) {
            keyModel.append({key: list[i], id:i})
        }

        keyListView.currentIndex = 0
        currentKey = list[0]
    }

    Component.onCompleted: init()
}
