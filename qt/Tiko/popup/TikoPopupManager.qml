pragma Singleton
import QtQuick

QtObject {
    property var activePopup: null

    function show(popupItem) {
        if (activePopup !== null && activePopup !== popupItem) {
            activePopup.close()
        }
        activePopup = popupItem
    }

    function hide(popupItem) {
        if (activePopup === popupItem) {
            activePopup = null
        }
    }
}
