import QtQuick
import Tiko

TikoButtonNormal {
    id: tikoComboxButton
    text: tikoComboxButton.helpText + tikoComboxButton.showText
    onClick: tikoPopup.open()
    property var data: ["文本1", "文本2"]
    property int show: 0
    property string helpText: ""
    property string showText: tikoComboxButton.data[show]

    TikoPopup{
        id: tikoPopup
        width: tikoComboxButton.width
        height: column.height
        padding: 0

        Column{
            id: column
            spacing: 0

            Repeater{
                model: ListModel{
                    id: listModel
                }

                delegate: TikoButtonNormal{
                    radius: 0
                    text: tikoComboxButton.data[index]
                    width: tikoComboxButton.width
                    onClick: {
                        tikoComboxButton.show = index
                        tikoPopup.close()
                    }
                }
            }
        }

        Component.onCompleted: {
            for(let i=0; i<tikoComboxButton.data.length; i++){
                listModel.append({index:i})
            }
        }
    }
}
