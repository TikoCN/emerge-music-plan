import QtQuick
import Tiko

TikoPopup {
    id: popupInput
    implicitWidth: 600
    implicitHeight: 400

    property string title: qsTr("请输入")
    property string inputText: inputLine.text
    property string orgText: ""
    property int textWidth: 200
    signal accept()
    signal cancel()
    signal finishInput()
    signal input()

    TikoButtonDefault{
        id: yes
        textLine.text: qsTr("确定")
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: height / 2 + parent.width / 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: - width / 2 - parent.width / 20
        onClicked: {
            popupInput.close()
            accept()
        }
    }

    TikoButtonDefault{
        id: no
        textLine.text: qsTr("取消")
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: height / 2 + parent.width / 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: width / 2 + parent.width / 20
        onClicked: {
            cancel()
            popupInput.close()
        }
    }

    TikoTextLine {
        id: helpItem
        text: title
        width: inputLine.width
        horizontalAlignment: Text.AlignHCenter
        level: 2
        anchors.bottom: inputLine.top
        anchors.margins: TikoSeit.emphasizeMargins
        anchors.horizontalCenter: parent.horizontalCenter
    }

    TikoTextInput{
        id: inputLine
        placeholderText: orgText
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: - height / 2 - parent.width / 20
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 20
        onEditingFinished: popupInput.finishInput()
        onTextEdited: popupInput.input()
        horizontalAlignment: Text.AlignHCenter
    }


    function setNormalText(){
        inputLine.input.text = orgText
    }
}
