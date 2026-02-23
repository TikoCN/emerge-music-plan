import QtQuick
import Tiko

Rectangle {
    id: dynamicbackground
    property TikoDynamicColor backgroundDynamicColor: TikoSeit.theme.backgroundDynamicColor
    property TikoDynamicColor borderDynamicColor: TikoSeit.theme.borderDynamicColor
    property TikoDynamicState dynamicState: null
    property TikoDynamicState compulsion: compulsionState

    color: Qt.rgba(0, 0, 0, 0)
    border.color: Qt.rgba(0, 0, 0, 0)
    border.width: 1
    radius: 3

    states: [
        State {
            name: "hover"
            when: compulsionState.isHover || (dynamicState != null && dynamicState.isHover)
            PropertyChanges {
                target: dynamicbackground
                color: backgroundDynamicColor.hoverColor
                border.color: borderDynamicColor.hoverColor
            }
        },
        State {
            name: "pressed"
            when: compulsionState.isPressed || (dynamicState != null && dynamicState.isPressed)
            PropertyChanges {
                target: dynamicbackground
                color: backgroundDynamicColor.pressedColor
                border.color: borderDynamicColor.pressedColor
            }
        },
        State {
            name: "disabled"
            when: compulsionState.isDisabled || (dynamicState != null && dynamicState.isDisabled)
            PropertyChanges {
                target: dynamicbackground
                color: backgroundDynamicColor.disabledColor
                border.color: borderDynamicColor.disabledColor
            }
        },
        State {
            name: "normal"
            when: compulsionState.isNormal || (dynamicState != null && dynamicState.isNormal)
            PropertyChanges {
                target: dynamicbackground
                color: backgroundDynamicColor.normalColor
                border.color: borderDynamicColor.normalColor
            }
        }
    ]

    TikoDynamicState {
        id: compulsionState
    }
}
