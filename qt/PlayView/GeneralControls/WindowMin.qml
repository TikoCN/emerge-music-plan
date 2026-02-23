import QtQuick
import Tiko
import PlayView

//最小化
TikoButtonIconLittle {
    id: min
    //text: qsTr("最小化")
    icon.source: "qrc:/image/min.png"
    onClicked: CoreData.windowShowMin()
}
