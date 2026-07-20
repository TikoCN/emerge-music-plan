import QtQuick
import MediaerAPI
import Tiko

LrcLineBase {
    playingColor: TikoSeit.theme.colorTextHighlight
    normalColor: TikoSeit.theme.colorTextHint
    lrcFont: Setting.lrcFont
    isPlay: MediaPlayer.playingLrcId === lrcId
    onIsPlayChanged: lrcCanvas.requestPaint()
    aloneLine: false

    Connections {
        target: MediaPlayer
        function onPlayingLrcIdChanged(playingLrcId) {
            if (isPlay != (playingLrcId === lrcId)) {
                isPlay = (playingLrcId === lrcId)
            }
        }
    }
}
