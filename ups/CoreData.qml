pragma Singleton
import QtQuick
import Ups
import MediaerAPI
import Tiko
import DataCore

QtObject {
    property double timeWidth: 0.0
    property double editTimeWidth: 0.0
    property double playNumberWidth: 0.0
    signal mainTurnSeit()
    signal mainTurnMain()
    signal mainTurnAlbumPage()
    signal mainTurnAlbumPlayer(AlbumData album)
    signal mainTurnArtistPage()
    signal mainTurnArtistPlayer(ArtistData artist)

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

    function openMenuMusic(parent, music, type){
        var component = Qt.createComponent("menu/MenuMusic.qml")

        if (component.status === Component.Ready) {
            var item = component.createObject(parent, {music: music, type: type})
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
    function openMenuTableSort(parent, table){
        var component = Qt.createComponent("menu/MenuTableSort.qml")

        if (component.status === Component.Ready) {
            var item = component.createObject(parent, {table: table})
            item.open()
        }
        else {
            TikoSeit.sendMessage(parent, component.errorString(), 1)
            console.log(component.errorString())
        }
    }
}
