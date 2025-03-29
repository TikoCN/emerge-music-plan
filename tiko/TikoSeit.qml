pragma Singleton
import QtQuick
import QtQuick.Controls
import Tiko

QtObject {
    property color themeColor: "lightBlue"
    property color transparentColor: "#000000"
    property color backdropColor: "#ffffff"
    property double fontPixelSize: 13
    property string fontFamily: "Times"
    property double messageY: 30


    //发送消息
    function sendMessage(parent, msg, type){
        var component = Qt.createComponent("message/TikoMessageLittle.qml")

        if (component.status === Component.Ready) {
            var littleMessage = component.createObject(parent, {message: msg, type:type})
            littleMessage.show()
        }
        else {
            console.log(component.errorString())
        }
    }
}
