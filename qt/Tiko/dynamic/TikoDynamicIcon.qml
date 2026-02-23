import QtQuick
import Tiko


TikoImage {
    id: dynamicIcon
    property TikoDynamicColor dynamicColor: TikoSeit.theme.foregroundDynamicColor
    property TikoDynamicState dynamicState: null
    property alias compulsion: compulsionState

    states: [
        State {
            name: "hover"
            when: compulsionState.isHover || (dynamicState != null && dynamicState.isHover)
            PropertyChanges {
                target: dynamicIcon
                unifiedColor: dynamicColor.hoverColor
            }
        },
        State {
            name: "pressed"
            when: compulsionState.isPressed || (dynamicState != null && dynamicState.isPressed)
            PropertyChanges {
                target: dynamicIcon
                unifiedColor: dynamicColor.pressedColor
            }
        },
        State {
            name: "disabled"
            when: compulsionState.isDisabled || (dynamicState != null && dynamicState.isDisabled)
            PropertyChanges {
                target: dynamicIcon
                unifiedColor: dynamicColor.disabledColor
            }
        },
        State {
            name: "normal"
            when: compulsionState.isNormal || (dynamicState != null && dynamicState.isNormal)
            PropertyChanges {
                target: dynamicIcon
                unifiedColor: dynamicColor.normalColor
            }
        }
    ]

    TikoDynamicState {
        id: compulsionState
    }
}
