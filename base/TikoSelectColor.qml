import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: tikoColorSelcet
    implicitHeight: 40

    property color selectedColor: "red"
    property color borderColor: TikoSeit.transparentColor
    property string text: qsTr("颜色选择")

    MouseArea{
        onClicked: colorSelect.open()
        anchors.fill: parent
    }

    TikoAutoText{
        text: tikoColorSelcet.text
        anchors.left: tikoColorSelcet.left
        height: tikoColorSelcet.height
        width: tikoColorSelcet.width - tikoColorSelcet.height * 0.6
    }

    Rectangle{
        height: tikoColorSelcet.height * 0.6
        width: this.height
        anchors.right: tikoColorSelcet.right
        color: tikoColorSelcet.selectedColor
        border.color: tikoColorSelcet.borderColor
    }


    TikoPopup{
        id: colorSelect
        width: 600
        height: 350

        contentItem: Item{
            //红色
            Rectangle{
                id: redR
                color: "red"
                width: 15
                height: 15
            }

            TikoSlider{
                anchors.left: redR.right
                anchors.leftMargin: 15
                anchors.top: redR.top
                id: redSlider
                width: parent.width - 50
                height: 15
                radius: 6
                from: 0
                to: 255
                value: tikoColorSelcet.selectedColor.r * 255
                onMoved: tikoColorSelcet.selectedColor.r = redSlider.value / 255
            }

            //绿色
            Rectangle{
                y: redR.y + redR.height + 10
                id: greenR
                color: "green"
                width: 15
                height: 15
            }

            TikoSlider{
                anchors.left: greenR.right
                anchors.leftMargin: 15
                anchors.top: greenR.top
                id: greenSlider
                width: parent.width - 50
                height: 15
                radius: 6
                from: 0
                to: 255
                value: tikoColorSelcet.selectedColor.g * 255
                onMoved: tikoColorSelcet.selectedColor.g = greenSlider.value / 255
            }

            //蓝色
            Rectangle{
                y: greenR.y + greenR.height + 10
                id: blueR
                color: "blue"
                width: 15
                height: 15
            }

            TikoSlider{
                anchors.left: blueR.right
                anchors.leftMargin: 15
                anchors.top: blueR.top
                id: blueSlider
                width: parent.width - 50
                height: 15
                radius: 6
                from: 0
                to: 255
                value: tikoColorSelcet.selectedColor.b * 255
                onMoved: tikoColorSelcet.selectedColor.b = blueSlider.value / 255
            }

            //选择预定颜色
            GridLayout{
                y: blueR.y + blueR.height + 10
                width: parent.width
                height: 50
                rows: 2
                columns: 7
                Repeater{
                    delegate: Component{
                        Item{
                            height: 20
                            width: 20
                            id: select
                            property color showColor: inColor
                            Rectangle{
                                anchors.fill: parent
                                color: select.showColor
                                border.color: "black"
                                border.width: 1
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked: tikoColorSelcet.selectedColor = showColor
                            }
                        }
                    }

                    model: ListModel{
                        ListElement{inColor: "red"}
                        ListElement{inColor: "blue"}
                        ListElement{inColor: "green"}
                        ListElement{inColor: "aliceblue"}
                        ListElement{inColor: "black"}
                        ListElement{inColor: "gold"}
                        ListElement{inColor: "greenyellow"}
                        ListElement{inColor: "tan"}
                        ListElement{inColor: "yellow"}
                        ListElement{inColor: "white"}
                        ListElement{inColor: "teal"}
                        ListElement{inColor: "snow"}
                    }
                }
            }
        }
    }
}
