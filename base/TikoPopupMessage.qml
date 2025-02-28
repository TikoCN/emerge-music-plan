import QtQuick.Dialogs
import QtQuick

TikoPopup {
    id:dialog
    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2
    implicitHeight: parent.height / 4
    implicitWidth: parent.width / 4

    property string text

    TikoAutoText{
        anchors.centerIn: parent
        text: dialog.text
    }

    TikoBorderButton{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        text: qsTr("了解")
        onClicked: dialog.close()
    }

    function show(text){
        dialog.text = text
        dialog.open()
    }
}
