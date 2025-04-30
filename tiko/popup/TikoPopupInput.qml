import QtQuick
import Tiko

TikoPopup {
    id: popupInput
    implicitWidth: 600
    implicitHeight: 400

    property string text: qsTr("请输入")
    property string inputText: inputLine.inputItem.text
    property string orgText: ""
    property int textWidth: 200
    signal accept()
    signal cancel()
    signal finish()
    signal input()

    TikoButtonBorder{
        id: yes
        text: qsTr("确定")
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: height / 2 + parent.width / 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: - width / 2 - parent.width / 20
        onClick: accept()
    }

    TikoButtonBorder{
        id: no
        text: qsTr("取消")
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: height / 2 + parent.width / 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: width / 2 + parent.width / 20
        onClick: cancel()
    }

    TikoTextInput{
        id: inputLine
        show.text: text
        show.width: popupInput.textWidth
        inputItem.text: orgText
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: - height / 2 - parent.width / 20
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 20
        onFinish: popupInput.finish()
        onInput: popupInput.input()
    }


    function setNormalText(){
        inputLine.input.text = orgText
    }
}
