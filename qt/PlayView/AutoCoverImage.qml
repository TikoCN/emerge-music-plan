import QtQuick
import Tiko
import MediaerAPI

TikoImageAuto {
    property string baseUrl: ""
    // 请求链与实际采样尺寸同步，避免两个定时器分别更新造成尺寸错位。
    property string sizeUrl: "&width=" + sourceSize.width + "&height=" + sourceSize.height
    loadUrl: baseUrl + sizeUrl

    onStatusChanged: {
        if (status === Image.Ready &&
                String(source) === loadUrl &&
                ImageControl.getUrlNullFlag(loadUrl)) {
            keepNormal = true
        }
    }
}
