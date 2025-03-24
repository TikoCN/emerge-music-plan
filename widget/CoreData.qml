pragma Singleton
import QtQuick
import Widget

QtObject {
    property double timeWidth: 0.0
    property double editTimeWidth: 0.0
    property double playNumberWidth: 0.0

    function editMusic(parent, core){
        var component = Qt.createComponent("toolEditMusicPage/ToolEditMusic.qml")

        console.log(component.status === Component.Ready)
        if (component.status === Component.Ready) {
            var item = component.createObject(parent)
            item.build(core, core.getLrcData())
            item.show()
        }
    }

    function openMenuMusic(parent){
        var component = Qt.createComponent("menu/MenuMusic.qml")

        if (component.status === Component.Ready) {
            var item = component.createObject(parent, {musicLine: parent, music: parent.music})
            item.open()
        }
    }

    // 打开列表排序菜单
    function openMenuTableSort(parent, table){
        var component = Qt.createComponent("menu/MenuTableSort.qml")

        if (component.status === Component.Ready) {
            var item = component.createObject(parent, {table: table})
            item.open()
        }
    }
}
