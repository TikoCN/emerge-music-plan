pragma Singleton
import QtQuick

QtObject {
    property double timeWidth: 0.0
    property double editTimeWidth: 0.0
    property double playNumberWidth: 0.0

    function editMusic(parent, core){
        var component = Qt.createComponent("./view/ToolEditMusic.qml")

        if (component.status === Component.Ready) {
            var item = component.createObject(parent, {core:core})
            item.show()
        }
    }

    function openMenuMusic(parent){
        var component = Qt.createComponent("./view/MenuMusic.qml")

        if (component.status === Component.Ready) {
            var item = component.createObject(parent, {musicLine: parent, music: parent.music})
            item.open()
        }
    }
}
