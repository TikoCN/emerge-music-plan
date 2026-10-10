import QtQuick
import Tiko
import PlayView

//最小化
TikoButtonIcon {
    id: min
    //text: qsTr("最小化")
    icon.source: "qrc:/image/min.svg"
    onClicked: CoreData.windowShowMin()
    level: 0
}
