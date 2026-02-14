import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import QtQml
import MediaerAPI
import Tiko
import PlayView

Item {
    id: libraryPage

    RowLayout {
        id: toolRow
        anchors.top: libraryPage.top
        anchors.right: libraryPage.right
        anchors.left: libraryPage.left
        anchors.topMargin: TikoSeit.emphasizeMargins

        Repeater {
            model: [
                {name:"专辑", click: stackAlbum},
                {name:"歌手", click: stackArtist},
                {name:"音乐", click: stackMusic}
            ]

            delegate: TikoButtonNormal {
                textLine.text: modelData.name
                onClicked: modelData.click()
            }
        }
    }

    StackView {
        id: stackView
        anchors.top: toolRow.bottom
        anchors.bottom: libraryPage.bottom
        anchors.right: libraryPage.right
        anchors.left: libraryPage.left
        anchors.margins: TikoSeit.emphasizeMargins
        initialItem: musicPage

        PageAlbum {id: albumPage; visible: false}
        PageArtist {id: artistPage; visible: false}
        PageMusic {id: musicPage; visible: false}
    }

    function stackAlbum() {
        if(stackView.currentItem !== albumPage)
            stackView.replace(albumPage)
    }

    function stackArtist() {
        if(stackView.currentItem !== artistPage)
            stackView.replace(artistPage)
    }

    function stackMusic() {
        if(stackView.currentItem !== musicPage)
            stackView.replace(musicPage)
    }
}
