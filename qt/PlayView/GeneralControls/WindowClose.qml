import QtQuick
import Tiko
import PlayView

//关闭
TikoButtonIconLittle {
    id: close
    //text: qsTr("关闭")
    icon.source: "qrc:/image/close.png"
    onClicked: CoreData.windowClose()
}

