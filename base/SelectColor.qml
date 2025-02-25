import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    property color selectedColor: "red"
    property string text
    height: 50

    Rectangle{
        id: colorShow
        color: root.selectedColor
        y: textShow.height * 0.2
        width: textShow.height * 0.6
        height: width
        border.color: BaseSeit.transparentColor
        border.width: 1
        radius: height * 0.2
        smooth: true
    }

    MyBarButton{
        id: textShow
        anchors.left: colorShow.right
        anchors.leftMargin: 3
        width: parent.width - colorShow.width - 20
        text: root.text
        onClicked: colorSelect.open()
    }

    MyPopup{
        id: colorSelect
        width: 200
        height: 150

        contentItem: Item{
            //红色
            Rectangle{
                id: redR
                color: "red"
                width: 15
                height: 15
            }

            MySlider{
                anchors.left: redR.right
                anchors.leftMargin: 15
                anchors.top: redR.top
                id: redSlider
                width: parent.width - 50
                height: 15
                radius: 6
                from: 0
                to: 255
                value: root.selectedColor.r * 255
                onMoved: root.selectedColor.r = redSlider.value / 255
            }

            //绿色
            Rectangle{
                y: redR.y + redR.height + 10
                id: greenR
                color: "green"
                width: 15
                height: 15
            }

            MySlider{
                anchors.left: greenR.right
                anchors.leftMargin: 15
                anchors.top: greenR.top
                id: greenSlider
                width: parent.width - 50
                height: 15
                radius: 6
                from: 0
                to: 255
                value: root.selectedColor.g * 255
                onMoved: root.selectedColor.g = greenSlider.value / 255
            }

            //蓝色
            Rectangle{
                y: greenR.y + greenR.height + 10
                id: blueR
                color: "blue"
                width: 15
                height: 15
            }

            MySlider{
                anchors.left: blueR.right
                anchors.leftMargin: 15
                anchors.top: blueR.top
                id: blueSlider
                width: parent.width - 50
                height: 15
                radius: 6
                from: 0
                to: 255
                value: root.selectedColor.b * 255
                onMoved: root.selectedColor.b = blueSlider.value / 255
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
                                onClicked: root.selectedColor = showColor
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
