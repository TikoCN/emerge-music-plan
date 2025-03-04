import QtQuick.Controls.Basic

TikoPopup {
    id: root
    implicitWidth: 600
    implicitHeight: 400

    property string text
    property string inputText: inputLine.input.text
    property string orgText: ""
    property var onAccept: ()=>{}
    property var onCancel: ()=>{}

    TikoInputText{
        id: inputLine
        show.text: text
        input.text: orgText
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: - height / 2 - parent.width / 20
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 20
    }

    TikoBorderButton{
        id: yes
        text: qsTr("确定")
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: height / 2 + parent.width / 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: - width / 2 - parent.width / 20
        onClicked: {
            onAccept()
            root.close()
        }
    }

    TikoBorderButton{
        id: no
        text: qsTr("取消")
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: height / 2 + parent.width / 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: width / 2 + parent.width / 20
        onClicked: {
            onCancel()
            root.close()
        }
    }
}
