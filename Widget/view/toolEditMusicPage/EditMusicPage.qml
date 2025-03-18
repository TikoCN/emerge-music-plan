import QtQuick
import QtQuick.Layouts
import Tiko

Item{
    id: editMusicPage
    visible: false

    ColumnLayout{
        id: musicTool
        width: 150
    }

    ColumnLayout{
        id: musicData
        anchors.left: musicTool.right
        width: editMusicPage.width - musicTool.width
        spacing: 10

        property int showW: 30

        TikoTextInput{
            id: title
            show.text: qsTr("标题")
            show.width: musicData.showW
        }

        TikoTextInput{
            id: artist
            show.text: qsTr("歌手")
            show.width: musicData.showW
        }

        TikoTextInput{
            id: alumb
            show.text: qsTr("专辑")
            show.width: musicData.showW
        }
    }


    function init(music){
        title.input.text = music.title
        artist.input.text = music.artist
        alumb.input.text = music.alumb
    }
}
