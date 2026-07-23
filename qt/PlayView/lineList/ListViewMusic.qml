import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import MediaerAPI
import Tiko
import PlayView

ListView{
    id: musicListView
    interactive: true
    spacing: 15
    clip: true
    reuseItems: true
    ScrollBar.vertical: TikoBarV{}

    property bool isLittle: false
    property bool onlyLove: false
    property var customLoadCallback: null
    property int loadPos: 0
    signal play(int musicId, int listId)
    signal createMenu()
    signal loadData(int index)

    model: MusicLibrary.model()

    delegate: CoreMusicLine{
        width: musicListView.width
        listId: index
        isLittle: musicListView.isLittle
        visible: !onlyLove || model.isLove
        onPlayMusic: (musicId, listId) => {musicListView.play(musicId, listId)}
    }

    function loadByKey(key) {
        MusicLibrary.loader().setCurrentKey(key)
        MusicLibrary.loader().reset()
    }

    function loadAlbumMusic(albumId, size, start, sort) {
        model.loadAlbumMusic(albumId, size, start, sort)
    }

    function loadArtistMusic(artistId, size, start, sort) {
        model.loadArtistMusic(artistId, size, start, sort)
    }

    function loadPlayListMusic(playlistId, size, start, sort) {
        model.loadPlayListMusic(playlistId, size, start, sort)
    }

    function loadMore(list) {
        model.appendMusicList(list)
    }

    function loadMoreByCustom() {
        if (customLoadCallback) {
            let list = customLoadCallback(loadPos)
            if (list && list.length > 0) {
                model.appendMusicList(list)
            }
            loadPos += CoreData.pageSize
        } else {
            MusicLibrary.loader().loadMore()
        }
    }

    function reset() {
        model.clear()
        loadPos = 0
        if (customLoadCallback) {
            let list = customLoadCallback(0)
            if (list && list.length > 0) {
                model.appendMusicList(list)
            }
            loadPos += CoreData.pageSize
        } else {
            MusicLibrary.loader().reset()
        }
    }
}