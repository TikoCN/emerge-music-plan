import QtQuick
import Tiko


TikoImage {
    id: dynamicIcon
    property TikoDynamicColor dynamicColor: TikoSeit.theme.foregroundDynamicColor
    property TikoDynamicState dynamicState: null

    states: [
        State {
            name: "normal"
            when: dynamicState != null && dynamicState.isNormal
            PropertyChanges {
                target: dynamicIcon
                unifiedColor: dynamicColor.normalColor
            }
        },
        State {
            name: "hover"
            when: dynamicState != null && dynamicState.isHover
            PropertyChanges {
                target: dynamicIcon
                unifiedColor: dynamicColor.hoverColor
            }
        },
        State {
            name: "pressed"
            when: dynamicState != null && dynamicState.isPressed
            PropertyChanges {
                target: dynamicIcon
                unifiedColor: dynamicColor.pressedColor
            }
        },
        State {
            name: "disabled"
            when: dynamicState != null && dynamicState.isDisabled
            PropertyChanges {
                target: dynamicIcon
                unifiedColor: dynamicColor.disabledColor
            }
        }
    ]
}
