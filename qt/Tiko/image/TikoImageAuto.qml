import QtQuick.Effects
import QtQuick

TikoImage{
    id: imageAutoView
    source: normalUrl
    enableUnifiedColor: String(source) === normalUrl

    property int loadMsTime: 300
    property int freeMsTime: 300
    property string normalUrl: ""
    property string loadUrl: ""
    property bool extraLoadFlag: true
    property bool keepNormal: false
    // 暂停发起新请求，保留已显示的封面，避免拖动期间反复解码和上传图片。
    property bool loadingPaused: false
    property bool loadFlag: extraLoadFlag &&
                            imageAutoView.visible

    onLoadFlagChanged: loadImage()
    onLoadUrlChanged: {
        // 复用卡片后清除旧封面及失败状态，暂停加载时也不能沿用上一条数据的图片。
        keepNormal = false
        if (String(source) !== loadUrl && String(source) !== normalUrl)
            source = normalUrl
        loadImage()
    }
    onKeepNormalChanged: loadImage()
    onLoadingPausedChanged: loadImage()

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

    /**
     * @brief 根据加载状态安排封面请求，暂停期间保留当前图片。
     */
    function loadImage() {
        loadImageTimer.stop();
        deleteImageTimer.stop();

        if (loadingPaused) return;

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

    /**
     * @brief 在允许加载时切换到当前数据对应的封面。
     */
    function switchToLoadImage() {
        if (loadingPaused || !loadFlag || loadUrl === "") return;

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
}

