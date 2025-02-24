import QtQuick.Dialogs
import QtQuick
import MyAPI

MyPopup {
    id:dialog
    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2
    implicitHeight: parent.height / 4
    implicitWidth: parent.width / 4

    property string text

    MyAutoText{
        anchors.centerIn: parent
        text: dialog.text
    }

    MyBorderButton{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        text: qsTr("了解")
        onClicked: dialog.close()
    }

    //关联
    Connections{
        target:PopupData
        function onCppMessage(text){
            dialog.show(text)
        }
    }

    function show(text){
        dialog.text = text
        dialog.open()
    }
}
