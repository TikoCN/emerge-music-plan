import QtQuick
import QtQuick.Controls.Basic

Button {
    id: button
    implicitWidth: textRect.boundingRect.width * 4
    implicitHeight: 32

    property int radius: 3
    property double hover: 0.3
    property double normal: 0

    background: Rectangle{
        border.color:{
            return button.hovered ? BaseSeit.themeColor : BaseSeit.transparentColor
        }
        border.width: 1.5
        radius: button.radius
        color: "#00000000"
        opacity: 0.3
    }

    contentItem: MyAutoText{
        id: textShow
        anchors.fill: parent
        text: button.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    TextMetrics{
        id: textRect
        font: textShow.font
        text: textShow.text
    }
}
