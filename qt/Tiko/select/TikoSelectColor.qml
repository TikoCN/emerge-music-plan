import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Tiko

Item {
    id: tikoColorSelcet
    implicitHeight: 40

    property color selectedColor: "red"
    property color borderColor: TikoSeit.theme.baseTheme.borderTransition
    property string text: qsTr("颜色选择")

    MouseArea{
        onClicked: {
            if (colorSelectCom.status === Component.Ready) {
                var popup = colorSelectCom.createObject(tikoColorSelcet)
                popup.open()
            }
        }

        anchors.fill: parent
    }

    TikoTextLine{
        text: tikoColorSelcet.text
        anchors.left: colorShow.right
        anchors.leftMargin: 10
        anchors.verticalCenter: tikoColorSelcet.verticalCenter
        height: tikoColorSelcet.height
        width: tikoColorSelcet.width - tikoColorSelcet.height * 0.6
    }

    Rectangle{
        id: colorShow
        height: tikoColorSelcet.height * 0.6
        width: this.height
        anchors.left: tikoColorSelcet.left
        anchors.verticalCenter: tikoColorSelcet.verticalCenter
        color: tikoColorSelcet.selectedColor
        border.color: tikoColorSelcet.borderColor
    }

    Component {
        id: colorSelectCom

        TikoPopup{
            width: 300
            height: showItem.height + 20
            y: tikoColorSelcet.height
            x: (tikoColorSelcet.width - width) / 2
            onHide: destroy()

            Item{
                id: showItem
                width: 300
                height: childrenRect.height

                TikoTextLine {
                    id: colorDrop
                    text: qsTr("颜色调整")
                }

                //红色
                Rectangle{
                    anchors.top: colorDrop.bottom
                    anchors.topMargin: 10
                    id: redR
                    color: "red"
                    width: 30
                    height: 30
                }

                TikoSliderH{
                    anchors.left: redR.right
                    anchors.leftMargin: 30
                    anchors.verticalCenter: redR.verticalCenter
                    id: redSlider
                    width: parent.width - 100
                    height: 30
                    radius: 6
                    from: 0
                    to: 255
                    value: tikoColorSelcet.selectedColor.r * 255
                    onMoved: tikoColorSelcet.selectedColor.r = redSlider.value / 255
                }

                //绿色
                Rectangle{
                    anchors.top: redR.bottom
                    anchors.topMargin: 10
                    id: greenR
                    color: "green"
                    width: redR.width
                    height: redR.height
                }

                TikoSliderH{
                    anchors.left: greenR.right
                    anchors.leftMargin: 30
                    anchors.verticalCenter: greenR.verticalCenter
                    id: greenSlider
                    width: parent.width - 100
                    height: 30
                    radius: 6
                    from: 0
                    to: 255
                    value: tikoColorSelcet.selectedColor.g * 255
                    onMoved: tikoColorSelcet.selectedColor.g = greenSlider.value / 255
                }

                //蓝色
                Rectangle{
                    anchors.top: greenR.bottom
                    anchors.topMargin: 10
                    id: blueR
                    color: "blue"
                    width: redR.width
                    height: redR.height
                }

                TikoSliderH{
                    anchors.left: blueR.right
                    anchors.leftMargin: 30
                    anchors.verticalCenter: blueR.verticalCenter
                    id: blueSlider
                    width: parent.width - 100
                    height: 25
                    radius: 6
                    from: 0
                    to: 255
                    value: tikoColorSelcet.selectedColor.b * 255
                    onMoved: tikoColorSelcet.selectedColor.b = blueSlider.value / 255
                }

                TikoTextLine {
                    id: textColorSelect
                    text: qsTr("颜色选择")
                    anchors.top: blueR.bottom
                    anchors.topMargin: 10
                }

                //选择预定颜色
                GridLayout{
                    anchors.top: textColorSelect.bottom
                    anchors.topMargin: 10
                    width: showItem.width - 20
                    height: 120
                    rows: 2
                    columns: 7
                    Repeater{
                        delegate: Component{
                            Item{
                                height: 30
                                width: 30
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
}
