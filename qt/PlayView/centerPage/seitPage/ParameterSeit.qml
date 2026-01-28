import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import MediaerAPI
import Tiko
import PlayView

TikoRightVessel{
    Layout.fillWidth: true
    Layout.preferredHeight: this.height
    show.text: qsTr("参数管理")
    show.width: seitPage.modeTextWidth
    vessel: ColumnLayout {
        property var paraDataModel: [
            {text:qsTr("最大加载线程"), prop:"maxThreadNumber"}
        ]

        function inputNumber(input, number){
            var rx = /[^0-9]/
            if(input.text.match(rx) !== null ){
                popupMessage.show(qsTr("请输入数字"))
                input.text = number.toString()
            }
            else if(Number(input.text) !== number){
                return Number(input.text)
            }
            return number
        }

        Repeater {
            model: paraDataModel
            delegate: TikoTextInput{
                implicitWidth: 150
                show.text: modelData.text
                show.width: 100
                inputItem.text: Setting[modelData.prop].toString()
                inputItem.width: 50
                onFinish: Setting[modelData.prop] = inputNumber(inputItem, Setting[modelData.prop])
            }
        }
    }
}
