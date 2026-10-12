import QtQuick
import MediaerAPI
import Tiko
import PlayView

MenuMusic {
    id: menuMusicPlaylist
    property int playlistId: -1

    TikoMenu {
        title: qsTr("移动到")
        icon.source: "qrc:/image/move.svg"
        enabled: menuMusicPlaylist.playlistId !== -1

        Repeater {
            delegate: TikoMenuItem {
                text: model.name
                enabled: !model.isDir && model.id !== menuMusicPlaylist.playlistId
                onTriggered: {
                    if (PlaylistLibrary.moveMusicToPlaylist(menuMusicPlaylist.musicId,
                                                            menuMusicPlaylist.playlistId, model.id))
                        menuMusicPlaylist.playlistChanged()
                }
            }
            model: PlaylistModel {
                type: PlaylistModel.UserModel
            }
        }
    }

    TikoMenuItem {
        text: qsTr("从播放列表中移除")
        enabled: menuMusicPlaylist.playlistId !== -1
        onTriggered: {
            if (PlaylistLibrary.removeMusicFromPlaylist(menuMusicPlaylist.musicId,
                                                        menuMusicPlaylist.playlistId))
                menuMusicPlaylist.playlistChanged()
        }
    }
}
