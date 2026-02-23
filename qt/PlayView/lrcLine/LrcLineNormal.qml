import QtQuick
import MediaerAPI
import Tiko

LrcLineBase {
    playingColor: TikoSeit.theme.baseTheme.foregroundNormal
    normalColor: TikoSeit.theme.baseTheme.foregroundTransition
    lrcFont: Setting.lrcFont
    isPlay: MediaPlayer.playingLrcId === lrcId
    aloneLine: false

    Connections {
        target: MediaPlayer
        function onPlayingLrcIdChanged(playingLrcId) {
            if (isPlay != (playingLrcId === lrcId)) {
                isPlay = (playingLrcId === lrcId)
                lrcCanvas.requestPaint()
            }
        }
    }
}
