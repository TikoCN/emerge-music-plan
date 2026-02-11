import QtQuick

QtObject {
    // Level 0: 最基础/最强烈的层次（对于背景是最深/最浅，对于前景是最浅/最深）
    // Level 1: 中间层次
    // Level 2: 最高/最突出层次

    // 背景色层次 - 后缀表示层级深度
    property color backgroundNormal
    property color backgroundTransition
    property color backgroundEmphasize

    // 边框色层次 - 后缀表示视觉强度
    property color borderNormal
    property color borderTransition
    property color borderEmphasize

    // 前景色层次 - 后缀表示重要性/可见性
    property color foregroundNormal
    property color foregroundTransition
    property color foregroundEmphasize

    property color themeNormal
    property color themeTransition
    property color themeEmphasize

    function setColor(isLightTheme){
        if (isLightTheme) {
            // 背景色层次（从白到浅灰）
            backgroundNormal = Qt.rgba(1.0, 1.0, 1.0, 1.0)
            backgroundTransition = Qt.rgba(0.98, 0.98, 0.98, 1.0)
            backgroundEmphasize = Qt.rgba(0.95, 0.95, 0.95, 1.0)

            // 边框色层次（从极浅灰到中灰）
            borderNormal = Qt.rgba(0.9, 0.9, 0.9, 1.0)
            borderTransition = Qt.rgba(0.8, 0.8, 0.8, 1.0)
            borderEmphasize = Qt.rgba(0.6, 0.6, 0.6, 1.0)

            // 前景色层次（从黑到中灰）
            foregroundNormal = Qt.rgba(0.0, 0.0, 0.0, 1.0)
            foregroundTransition = Qt.rgba(0.2, 0.2, 0.2, 1.0)
            foregroundEmphasize = Qt.rgba(0.4, 0.4, 0.4, 1.0)
        } else {
            // 背景色层次（从黑到深灰）
            backgroundNormal = Qt.rgba(0.0, 0.0, 0.0, 1.0)
            backgroundTransition = Qt.rgba(0.07, 0.07, 0.07, 1.0)
            backgroundEmphasize = Qt.rgba(0.12, 0.12, 0.12, 1.0)

            // 边框色层次（从暗灰到浅灰）
            borderNormal = Qt.rgba(0.18, 0.18, 0.18, 1.0)
            borderTransition = Qt.rgba(0.25, 0.25, 0.25, 1.0)
            borderEmphasize = Qt.rgba(0.38, 0.38, 0.38, 1.0)

            // 前景色层次（从白到中灰白）
            foregroundNormal = Qt.rgba(1.0, 1.0, 1.0, 1.0)
            foregroundTransition = Qt.rgba(0.9, 0.9, 0.9, 1.0)
            foregroundEmphasize = Qt.rgba(0.7, 0.7, 0.7, 1.0)
        }
    }
}
