import QtQuick
import Tiko

Rectangle {
    id: dynamicbackground
    property color backgroundHoverColor: TikoSeit.backgroundHoverColor
    property color backgroundNormalColor: TikoSeit.backgroundNormalColor
    property color backgroundPressedColor: TikoSeit.backgroundPressedColor
    property color backgroundDisabledColor: TikoSeit.backgroundDisabledColor

    property color borderHoverColor: TikoSeit.borderHoverColor
    property color borderNormalColor: TikoSeit.borderNormalColor
    property color borderPressedColor: TikoSeit.borderPressedColor
    property color borderDisabledColor: TikoSeit.borderDisabledColor

    property bool isNormal: false
    property bool isHover: false
    property bool isPressed: false
    property bool isDisabled: false

    states: [
        State {
            name: "normal"
            when: isNormal
            PropertyChanges {
                target: dynamicbackground
                color: backgroundNormalColor
                border.color: borderNormalColor
            }
        },
        State {
            name: "hover"
            when: isHover
            PropertyChanges {
                target: dynamicbackground
                color: backgroundHoverColor
                border.color: borderHoverColor
            }
        },
        State {
            name: "pressed"
            when: isPressed
            PropertyChanges {
                target: dynamicbackground
                color: backgroundPressedColor
                border.color: borderPressedColor
            }
        },
        State {
            name: "disabled"
            when: isDisabled
            PropertyChanges {
                target: dynamicbackground
                color: backgroundDisabledColor
                border.color: borderDisabledColor
            }
        }
    ]
}
