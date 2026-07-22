import QtQuick
import Tiko
import MediaerAPI

TikoImageAuto {
    property string baseUrl: ""
    // 附加大小区分，只用作区分请求链，不用于实际大小
    property string sizeUrl: "&width="+width+"&height="+height
    loadUrl: baseUrl + sizeUrl

    onStatusChanged: {
        if (status === Image.Ready &&
                String(source) === loadUrl &&
                ImageControl.getUrlNullFlag(loadUrl)) {
            keepNormal = true
        }
    }
}
