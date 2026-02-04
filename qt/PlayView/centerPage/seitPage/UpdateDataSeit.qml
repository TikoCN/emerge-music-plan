import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import MediaerAPI
import Tiko
import PlayView

TikoRightVessel {
    Layout.preferredHeight: this.height
    show.text: qsTr("更新数据")

    vessel: ColumnLayout {
        property var updateDataMode: [
            {text:qsTr("更新专辑关键词"), doFunction: DataActive.updateALLNameKey}
        ]

        Repeater {
            model: updateDataMode
            delegate: TikoButtonBorder {
                Layout.preferredWidth: 300
                text: modelData.text
                onClick: modelData.doFunction()
            }
        }
    }
}
