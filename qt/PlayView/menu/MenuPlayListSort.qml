import Tiko
import MediaerAPI
import QtQuick
import DataType

TikoMenu{
    id: menuPlaylistSort
    width: 150
    y: parent.height

    property int playlistId: 0
    property int sort: -1
    property int newSort: -1
    property int forword:{
        if ( sort % 2 === 0 ) {
            return 1
        }
        else {
            return 0
        }
    }
    property int type: {
        if ( sort % 2 === 0 ) {
            return sort
        }
        else {
            return sort - 1
        }
    }

    onClosed: menuPlaylistSort.destroy()

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
            onTriggered: menuPlaylistSort.sortMusic(typeData, menuPlaylistSort.forword)
            icon.source: typeData === menuPlaylistSort.type ? "qrc:/image/sort.png" : ""
            property int type: typeData
        }
    }

    TikoMenuSpeacer{}
    TikoMenuItem{
        text: qsTr("升序");
        onTriggered: menuPlaylistSort.sortMusic(menuPlaylistSort.type, 0)
        icon.source: 0 === menuPlaylistSort.forword ? "qrc:/image/sort.png" : ""
    }
    TikoMenuItem{
        text: qsTr("降序");
        onTriggered: menuPlaylistSort.sortMusic(menuPlaylistSort.type, 1)
        icon.source: 1 === menuPlaylistSort.forword ? "qrc:/image/sort.png" : ""
    }

    function sortMusic(key, forword){
        newSort = key + forword
    }
}
