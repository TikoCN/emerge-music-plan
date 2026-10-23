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
                {name: qsTr("专辑"), icon:"qrc:/image/album.svg", click: stackAlbum},
                {name: qsTr("歌手"), icon:"qrc:/image/artist.svg", click: stackArtist},
                {name: qsTr("音乐"), icon:"qrc:/image/cover.png", click: stackMusic}
            ]

            delegate: TikoButtonDefault {
                icon.source: modelData.icon
                textLine.text: modelData.name
                checked: toolRow.selectText === modelData.name
                icon.unifiedColor: checked ? checkedColor : TikoSeit.theme.colorFgDefault
                textLine.color: checked ? checkedColor : TikoSeit.theme.colorFgDefault
                onClicked: {
                    if (toolRow.selectText === modelData.name) return

                    modelData.click()
                    toolRow.selectText = modelData.name
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

        LibraryAlbum {id: albumPage; visible: false}
        LibraryArtist {id: artistPage; visible: false}
        LibraryMusic {id: musicPage; visible: false}
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
