import QtQuick

Image{
    id: image
    cache: true
    source: normalUrl
    sourceSize.width: imgWidth
    sourceSize.height: imgHeight
    asynchronous: true

    property int imgWidth: width
    property int imgHeight: height
    property string normalUrl: ""
    property string loadUrl: ""

    onVisibleChanged: {
        if (visible) {
            loadImag.start()
        }
        else {
            deleteImag.start()
        }
    }

    Timer {
        id: deleteImag
        interval: 30000
        onTriggered: {
            if (!image.visible) {
                image.cache = true
                image.source = normalUrl
            }
        }
    }

    Timer {
        id: loadImag
        interval: 500
        onTriggered: {
            if (image.visible) {
                image.cache = false
                image.source = loadUrl
            }
        }
    }

    Component.onCompleted: {loadImag.start()}
}

