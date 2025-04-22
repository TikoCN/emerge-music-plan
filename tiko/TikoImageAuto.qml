import QtQuick

Image{
    id: image
    sourceSize.width: imgWidth
    sourceSize.height: imgHeight
    asynchronous: true
    cache: false

    property int imgWidth: width
    property int imgHeight: height
    property int loadMsTime: 500
    property int freeMsTime: 60000
    property string normalUrl: ""
    property string loadUrl: ""
    property bool loadFlag: image.visible

    onLoadFlagChanged: {
        if (loadFlag) {
            deleteImag.stop()
            if (image.source !== loadUrl)
                loadImag.start()
        }
        else {
            loadImag.stop()
            if (image.source !== normalUrl)
                deleteImag.start()
        }
    }

    Timer {
        id: deleteImag
        interval: freeMsTime
        onTriggered: {
            if (!loadFlag) {
                image.source = normalUrl
            }
        }
    }

    Timer {
        id: loadImag
        interval: loadMsTime
        onTriggered: {
            if (loadFlag) {
                image.source = loadUrl
            }
        }
    }

    Component.onCompleted: {
        loadImag.start()
    }

    onLoadUrlChanged: {
        if (loadFlag)
            image.source = loadUrl
    }
}

