import QtQuick
import Tiko
import MediaerAPI

TikoImageAuto {

    onStatusChanged: {
        if (status === Image.Ready &&
                String(source) === loadUrl &&
                ImageControl.getUrlNullFlag(loadUrl)) {
            keepNormal = true
        }
    }
}
