import QtQuick
import QtQuick.Controls.Basic
import MyAPI

Item {
    id: button

    implicitHeight: 36

    property bool check: false
    property string text: qsTr("文本")

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: check = !check
    }

    Rectangle{
        id: box
        width: height
        color: "#00000000"
        border.color: mouseArea.containsMouse ? Setting.themeColor : Setting.transparentColor
        border.width: 1
        height: parent.height * 0.6
        anchors.verticalCenter: parent.verticalCenter
        opacity: 0.6
        radius: height * 0.3

        Rectangle{
            anchors.centerIn: parent
            width: 0.8 * parent.width
            height: width
            color: Setting.themeColor
            opacity: button.check ? 0.5 : 0
            radius: height * 0.3
        }
    }

    MyAutoText{
        text: button.text
        anchors.left: box.right
        anchors.leftMargin: 10
        height: parent.height
        width: parent.width - box.width - 20
        color: mouseArea.containsMouse ? Setting.themeColor : Setting.transparentColor
    }
}
