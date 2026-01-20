import QtQuick

<<<<<<< Updated upstream:tiko/TikoImageAuto.qml
Image{
    id: image
    sourceSize.width: imgWidth
    sourceSize.height: imgHeight
    asynchronous: true
    cache: false

    property int imgWidth: width
    property int imgHeight: height
    property int loadMsTime: 500
    property int freeMsTime: 1000
    property string normalUrl: ""
    property string loadUrl: ""
    property bool loadFlag: image.visible

    onLoadFlagChanged: {
        if (loadFlag) {
            loadImag.start()
        }
        else {
            deleteImag.start()
        }
    }
=======
TikoImageColorAuto{
    id: imageAutoView
    source: normalUrl
    isAutoColor: String(source) === normalUrl

    property int loadMsTime: 100
    property int freeMsTime: 100
    property string normalUrl: ""
    property string loadUrl: ""
    property bool extraLoadFlag: true
    property bool keepNormal: false
    property bool loadFlag: extraLoadFlag &&
                            TikoSeit.isShowFramChange &&
                            imageAutoView.visible

    onLoadFlagChanged: loadImage()
    onLoadUrlChanged: loadImage()
    onKeepNormalChanged: loadImage()
>>>>>>> Stashed changes:qt/Tiko/TikoImageAuto.qml

    Timer {
        id: deleteImageTimer
        interval: freeMsTime
        onTriggered: switchToNormalImage()
        triggeredOnStart: false
        running: false
        repeat: false
    }

    Timer {
        id: loadImageTimer
        interval: loadMsTime
        onTriggered: switchToLoadImage()
        triggeredOnStart: false
        running: false
        repeat: false
    }

    Component.onDestruction: {
        loadImageTimer.stop();
        deleteImageTimer.stop();
        // 清理缓存，释放内存
        source = "";
    }

    function loadImage() {
        loadImageTimer.stop();
        deleteImageTimer.stop();

        if (keepNormal) {
            if (String(source) !== normalUrl) {
                switchToNormalImage()
            }
            return
        }

        if (loadFlag && String(source) !== loadUrl) {
            if (loadUrl !== "") loadImageTimer.start();
        } else if (String(source) !== normalUrl) {
            deleteImageTimer.start();
        }
    }

    function switchToLoadImage() {
        if (!loadFlag || loadUrl === "") return;

        if (String(source) !== loadUrl) {
            source = loadUrl;
        }
    }

    function switchToNormalImage() {
        if (!keepNormal && loadFlag) return;

        if (String(source) !== normalUrl) {
            source = normalUrl;
        }
    }

    Component.onCompleted: {loadImag.start()}

    onLoadUrlChanged: {
        if (loadFlag)
            image.source = loadUrl
    }
}

