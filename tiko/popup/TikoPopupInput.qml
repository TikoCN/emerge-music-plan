import QtQuick.Controls.Basic
import Tiko

TikoPopup {
    id: popupInput
    implicitWidth: 600
    implicitHeight: 400

    property string text
    property string inputText: inputLine.input.text
    property string orgText: ""
    property int textWidth: 200
    signal accept
    signal cancel

    TikoTextInput{
        id: inputLine
        show.text: text
        show.width: popupInput.textWidth
        input.text: orgText
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: - height / 2 - parent.width / 20
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 20
    }

    TikoButtonBorder{
        id: yes
        text: qsTr("确定")
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: height / 2 + parent.width / 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: - width / 2 - parent.width / 20
        onClicked: {
            popupInput.accept()
            popupInput.close()
        }
    }

    TikoButtonBorder{
        id: no
        text: qsTr("取消")
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: height / 2 + parent.width / 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: width / 2 + parent.width / 20
        onClicked: {
            popupInput.cancel()
            popupInput.close()
        }
    }

    function setNormalText(){
        inputLine.input.text = ""
    }
}
