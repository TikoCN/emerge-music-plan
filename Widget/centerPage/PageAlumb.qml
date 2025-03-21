import QtQuick
import QtQuick.Controls
import Tiko

Item {
    id: alumbPage

    Grid{
        id: alumbList
        visible: false

        Repeater{
            model: ListModel{
                id: alumbModel
            }

            delegate: Item {
                width: 100
                height: textLine.height + 110

                Image {
                    id: alumbCover
                    width: 100
                    height: 100
                    source: "image://cover/file:" +  musicId.toString()
                }

                TikoTextLine {
                    id: textLine
                    anchors.top: alumbCover.bottom
                    anchors.topMargin: 10
                    width: parent.width
                    text: alumbText
                }

                MouseArea {
                    anchors.fill: alumbCover
                    onClicked: alumbPage.openAlumbData(alumbId)
                }
            }
        }
    }

    Item{
        id: alumbDataShow
        visible: false

        Image {
            id: alumbDataCover
            width: 100
            height: 100
        }
    }

    StackView {
        initialItem: alumbList
        anchors.fill: parent
    }

    function openAlumbData (alumbId) {

    }

    function build () {

    }
}
