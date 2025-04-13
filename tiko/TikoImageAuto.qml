import QtQuick

Image{
    id: image
    sourceSize.width: imgWidth
    sourceSize.height: imgHeight
    asynchronous: true
    cache: false

    property int imgWidth: width
    property int imgHeight: height
    property string normalUrl: ""
    property string loadUrl: ""
    property bool loadFlag: image.visible

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
        interval: 10000
        onTriggered: {
            if (!loadFlag) {
                image.source = normalUrl
            }
        }
    }

    Timer {
        id: loadImag
        interval: 500
        onTriggered: {
            if (loadFlag) {
                image.source = loadUrl
            }
        }
    }

    Component.onCompleted: {loadImag.start()}
}

