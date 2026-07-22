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

    property LoadBase dataLoader: LoadBase{}
    property bool isLittle: false
    property bool onlyLove: false
    signal play(int musicId, int listId)
    signal createMenu()

    model: MusicLibrary.model()

    delegate: CoreMusicLine{
        width: musicListView.width
        listId: index
        isLittle: musicListView.isLittle
        visible: !onlyLove || model.isLove
        onPlayMusic: (musicId, listId) => {musicListView.play(musicId, listId)}
    }

    function loadByKey(key, size, start) {
        model.loadByKey(key, size, start)
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

    function reset() {
        model.clear()
        dataLoader.reset()
    }
}