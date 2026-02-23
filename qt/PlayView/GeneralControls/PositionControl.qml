import QtQuick
import Tiko
import MediaerAPI

TikoSliderH {
    id: playControl
    height: 10
    from: 0
    to: MediaPlayer.player.duration
    value: MediaPlayer.player.position
    size: 1
    showColor: TikoSeit.theme.baseTheme.foregroundNormal
    onMoved: {MediaPlayer.player.setPosition(value)}
}
