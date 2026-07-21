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

    Row {
        id: toolRow
        anchors.top: libraryPage.top
        anchors.right: libraryPage.right
        anchors.left: libraryPage.left
        anchors.margins: TikoSeit.emphasizeMargins
        spacing: TikoSeit.emphasizeMargins
        property string selectText: "qrc:/image/cover.png"

        Repeater {
            model: [
                {name:"专辑", icon:"qrc:/image/album.png", click: stackAlbum},
                {name:"歌手", icon:"qrc:/image/artist.png", click: stackArtist},
                {name:"音乐", icon:"qrc:/image/cover.png", click: stackMusic}
            ]

            delegate: TikoButtonDefault {
                icon.source: modelData.icon
                textLine.text: modelData.name
                //icon.dynamicState.isHighlight: (toolRow.selectText === textLine.text)
                //textLine.dynamicState.isHighlight: (toolRow.selectText === textLine.text)
                onClicked: {
                    if (toolRow.selectText === textLine.text) return

                    modelData.click()
                    toolRow.selectText = textLine.text
                }
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
