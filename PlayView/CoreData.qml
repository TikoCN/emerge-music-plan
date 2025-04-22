pragma Singleton
import QtQuick
import PlayView
import MediaerAPI
import Tiko
import DataType

QtObject {
    property double timeWidth: 0.0
    property double editTimeWidth: 0.0
    property double playNumberWidth: 0.0
    property bool sizeChange: false
    property var table: []
    signal mainTurnSeit()
    signal mainTurnMain()
    signal mainTurnAlbumPage()
    signal mainTurnAlbumPlayer(int album)
    signal mainTurnArtistPage()
    signal mainTurnArtistPlayer(int artist)
    signal sendMsg(string msg, int type)

    function editMusic(parent, core){
        var component = Qt.createComponent("toolEditMusicPage/ToolEditMusic.qml")

        if (component.status === Component.Ready) {
            var item = component.createObject(parent)
            item.build(core, core.getLrcData())
            item.show()
        }
        else {
            TikoSeit.sendMessage(parent, component.errorString(), 1)
            console.log(component.errorString())
        }
    }

    function openMenuMusic(parent, musicId){
        var component = Qt.createComponent("menu/MenuMusic.qml")

        if (component.status === Component.Ready) {
            var item = component.createObject(parent, {musicId: musicId})
            item.open()
        }
        else {
            TikoSeit.sendMessage(parent, component.errorString(), 1)
            console.log(component.errorString())
        }
    }

    function openMenuMusicTable(parent, musicId, tableId){
        var component = Qt.createComponent("menu/MenuMusicTable.qml")

        if (component.status === Component.Ready) {
            var item = component.createObject(parent, {musicId: musicId, tableId: tableId})
            item.open()
        }
        else {
            TikoSeit.sendMessage(parent, component.errorString(), 1)
            console.log(component.errorString())
        }
    }

    function openMenuArtist(parent, artist){
        var component = Qt.createComponent("menu/MenuArtist.qml")

        if (component.status === Component.Ready) {
            var item = component.createObject(parent, {artist: artist})
            item.open()
        }
        else {
            TikoSeit.sendMessage(parent, component.errorString(), 1)
            console.log(component.errorString())
        }
    }

    function openMenuAlbum(parent, album){
        var component = Qt.createComponent("menu/MenuAlbum.qml")

        if (component.status === Component.Ready) {
            var item = component.createObject(parent, {album: album})
            item.open()
        }
        else {
            TikoSeit.sendMessage(parent, component.errorString(), 1)
            console.log(component.errorString())
        }
    }

    // 打开列表排序菜单
    function openMenuTableSort(parent, tableId){
        var component = Qt.createComponent("menu/MenuTableSort.qml")

        if (component.status === Component.Ready) {
            var item = component.createObject(parent, {tableId: tableId})
            item.open()
        }
        else {
            TikoSeit.sendMessage(parent, component.errorString(), 1)
            console.log(component.errorString())
        }
    }
}
