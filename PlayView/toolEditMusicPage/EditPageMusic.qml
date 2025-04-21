import QtQuick
import QtQuick.Layouts
import Tiko

Item{
    id: editMusicPage

    Column{
        id: musicTool
        width: 150
    }

    Column{
        id: musicData
        anchors.left: musicTool.right
        width: editMusicPage.width - musicTool.width
        spacing: 10

        property int showW: 200

        Repeater{
            model: ListModel{id: keyModel}

            delegate: TikoTextInput{
                width: musicData.width
                show.text: key
                show.width: musicData.showW
                input.text: value
            }
        }
    }

    function init(music){
        var dict = music.getAllKey()
        for(var i=0; i<dict.length; i++){
            keyModel.append({key: dict[i].key, value: dict[i].value})
        }
    }
}
