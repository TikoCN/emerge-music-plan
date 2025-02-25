import QtQuick
import QtQuick.Controls

Item{
    id: root
    property font selectedFont: Qt.font
    property string text

    MyBorderButton{
        text: root.text
        anchors.fill: parent
        onClicked: editFont.open()
    }

    MyPopup{
        id: editFont
        width: 200
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

                delegate: MyBarButton{
                    width: fontList.width
                    height: 30
                    text: name
                    font.family: name
                    onClicked: {
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
                color: BaseSeit.transparentColor
                opacity: 0.03
            }

            MyAutoText{
                id: fontSizeShow
                y: 160
                width: 50
                height: fontSize.height
                text: fontSize.value.toString()
            }

            MySlider{
                id: fontSize
                anchors.left: fontSizeShow.right
                anchors.leftMargin: 10
                from: 10
                to: 30
                y: 160
                width: parent.width - fontSizeShow.width - 10
                radius: 6
                value: root.selectedFont.pixelSize
                onMoved: root.selectedFont.pixelSize = value
            }
        }
    }
}
