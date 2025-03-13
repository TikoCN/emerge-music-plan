import QtQuick
import QtQuick.Controls
import "../control"
import "../"

Item{
    id: root
    property font selectedFont: Qt.font
    property string text

    MouseArea{
        anchors.fill: parent
        onClicked: editFont.open()
    }

    TikoTextLine{
        font.family: selectedFont.family
        font.pixelSize: selectedFont.pixelSize
        anchors.fill: root
        text: root.text
    }

    TikoPopup{
        id: editFont
        width: 300
        height: 230
        contentItem: Item{
            ListView{
                id: fontList
                width: parent.width
                height: 150
                clip: true

                model: ListModel{
                    id: fontModel
                }

                delegate: TikoButtonNormal{
                    width: fontList.width
                    height: 30
                    text: name
                    autoText.font.family: name
                    onClickLeft: {
                        root.selectedFont.family = name
                        editFont.close()
                    }
                }

                Component.onCompleted:{
                    var list = Qt.fontFamilies()
                    var aim = 0
                    for(var i=0; i<list.length; i++){
                        fontModel.append({name:list[i]})
                        if(list[i] === root.selectedFont.family){
                            aim = i
                        }
                    }
                    fontList.currentIndex = aim
                    fontList.positionViewAtIndex(aim, ListView.Center)
                }
            }

            Rectangle{
                anchors.fill: fontList
                color: TikoSeit.transparentColor
                opacity: 0.03
            }

            TikoTextInput{
                id: fontSizeShow
                anchors.top: fontList.bottom
                anchors.topMargin: 10
                width: 150
                height: 30
                show.text: qsTr("文本大小")
                input.width: 50
                input.text: fontSize.value.toFixed(2).toString()
                input.onEditingFinished: {
                    root.selectedFont.pixelSize = inputNumber(input, root.selectedFont.pixelSize);
                }

                function inputNumber(input, number){
                    var rx = /[^0-9.]/
                    if(input.text.match(rx) !== null ){
                        popupMessage.show(qsTr("请输入数字"))
                        input.text = number.toFixed(2).toString()
                    }
                    else if(Number(input.text) !== number){
                        return Number(input.text)
                    }
                    return number
                }
            }

            TikoHSlider{
                id: fontSize
                anchors.left: fontSizeShow.right
                anchors.leftMargin: 10
                anchors.verticalCenter: fontSizeShow.verticalCenter
                from: 10
                to: 30
                width: parent.width - fontSizeShow.width - 10
                height: 15
                radius: 6
                value: root.selectedFont.pixelSize
                onMoved: root.selectedFont.pixelSize = value
            }
        }
    }
}
