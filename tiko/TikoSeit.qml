pragma Singleton
import QtQuick
import QtQuick.Controls

QtObject {
    property color themeColor: "lightBlue"
    property color transparentColor: "#000000"
    property color backdropColor: "#ffffff"
    property double fontPixelSize: 13
    property string fontFamily: "Times"

    //发送消息
    function sendMessage(parent, msg, type){
        var component = Qt.createComponent("./control/TikoLittleMessage.qml")

        if (component.status === Component.Ready) {
            var littleMessage = component.createObject(parent, {message: msg, type:type})
            littleMessage.show()
        }
    }
}
