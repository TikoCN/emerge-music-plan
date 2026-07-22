import QtQuick
import Tiko
import PlayView

//最大化
TikoButtonIcon {
    id: max
    //text: qsTr("最大化")
    icon.source: "qrc:/image/max.png"
    onClicked: CoreData.windowShowMax()
    level: 0
}
