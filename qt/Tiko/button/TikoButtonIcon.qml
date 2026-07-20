import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Effects
import Tiko

TikoButtonBase {
    id: iconButton
    property TikoImage icon: iconItem
    property int level: 1
    property double cell: 32
    property double imgCell: 32
    bgOpacity: 0
    onLevelChanged: {
        switch (level) {
        case 1:
            cell = 32
            imgCell = 32
            break;
        case 0:
            cell = 20
            imgCell = 20
            break;
        }
    }

    width: cell
    height: cell

    TikoImage {
        id: iconItem
        anchors.centerIn: parent
        width: imgCell
        height: imgCell
    }

    SequentialAnimation {
        id: hoverAnim
        property int r: 20

        RotationAnimation{
            target: iconItem
            from: 0
            to: hoverAnim.r
            duration: 100
        }


        RotationAnimation{
            target: iconItem
            from: hoverAnim.r
            to: -hoverAnim.r
            duration: 200
        }

        RotationAnimation{
            target: iconItem
            from: -hoverAnim.r
            to: 0
            duration: 100
        }
    }
}
