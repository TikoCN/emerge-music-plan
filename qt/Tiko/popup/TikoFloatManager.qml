pragma Singleton
import QtQuick

QtObject {
    property var activeFloat: null

    function show(floatItem) {
        if (activeFloat !== null && activeFloat !== floatItem) {
            activeFloat.close()
        }
        activeFloat = floatItem
    }

    function hide(floatItem) {
        if (activeFloat === floatItem) {
            activeFloat = null
        }
    }
}
