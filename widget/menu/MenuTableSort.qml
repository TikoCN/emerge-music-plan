import Tiko
import TikoAPI
import QtQuick

TikoMenu{
    id: menuTableSort
    width: 150
    y: parent.height
    onClosed: menuTableSort.destroy()
    property int table: 0
    property int forword: Core.tableList[menuTableSort.table].getSort() % 2
    property int type: {
        var sort = Core.tableList[menuTableSort.table].getSort()
        if ( sort % 2 === 0 ) {
            return sort
        }
        else {
            return sort - 1
        }
    }

    Repeater {
        model: ListModel {
            ListElement{typeText: qsTr("标题"); typeData: 0}
            ListElement{typeText: qsTr("歌手"); typeData: 2}
            ListElement{typeText: qsTr("专辑"); typeData: 4}
            ListElement{typeText: qsTr("时长"); typeData: 6}
            ListElement{typeText: qsTr("编辑时间"); typeData: 8}
            ListElement{typeText: qsTr("评级"); typeData: 10}
        }

        delegate: TikoMenuItem{
            text: typeText
            onTriggered: menuTableSort.sortMusic(typeData, menuTableSort.forword)
            icon.source: typeData === menuTableSort.type ? "qrc:/image/sort.png" : ""
            property int type: typeData
        }
    }

    TikoMenuSpeacer{}
    TikoMenuItem{
        text: qsTr("升序");
        onTriggered: menuTableSort.sortMusic(menuTableSort.type, 0)
        icon.source: 0 === menuTableSort.forword ? "qrc:/image/sort.png" : ""
    }
    TikoMenuItem{
        text: qsTr("降序");
        onTriggered: menuTableSort.sortMusic(menuTableSort.type, 1)
        icon.source: 1 === menuTableSort.forword ? "qrc:/image/sort.png" : ""
    }

    function sortMusic(key, forword){
        var sort = key + forword
        Core.tableList[menuTableSort.table].sortMusic(sort);
    }
}
