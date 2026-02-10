import QtQuick
import Tiko


TikoImage {
    property color hoverColor: TikoSeit.iconHoverColor
    property color normalColor: TikoSeit.iconNormalColor
    property color pressedColor: TikoSeit.iconPressedColor
    property color disabledColor: TikoSeit.iconDisabledColor

    property bool isNormal: false
    property bool isHover: false
    property bool isPressed: false
    property bool isDisabled: false

    states: [
        State {
            name: "normal"
            when: isNormal
            PropertyChanges {
                target: dynamicTextLine
                unifiedColor: normalColor
            }
        },
        State {
            name: "hover"
            when: isHover
            PropertyChanges {
                target: dynamicTextLine
                unifiedColor: hoverColor
            }
        },
        State {
            name: "pressed"
            when: isPressed
            PropertyChanges {
                target: dynamicTextLine
                unifiedColor: pressedColor
            }
        },
        State {
            name: "disabled"
            when: isDisabled
            PropertyChanges {
                target: dynamicTextLine
                unifiedColor: disabledColor
            }
        }
    ]
}
