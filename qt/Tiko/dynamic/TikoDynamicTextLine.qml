import QtQuick
import Tiko

TikoTextLine {
    id: dynamicTextLine
    property TikoDynamicColor dynamicColor: TikoSeit.theme.foregroundDynamicColor
    property TikoDynamicState dynamicState: null
    property TikoDynamicState compulsion: compulsionState

    property bool enableUnifiedColor: false
    property color unifiedColor: Qt.black

    states: [
        State {
            name: "hover"
            when: compulsionState.isHover || (dynamicState != null && dynamicState.isHover)
            PropertyChanges {
                target: dynamicTextLine
                color: dynamicColor.hoverColor
            }
        },
        State {
            name: "pressed"
            when: compulsionState.isPressed || (dynamicState != null && dynamicState.isPressed)
            PropertyChanges {
                target: dynamicTextLine
                color: dynamicColor.normalColor
            }
        },
        State {
            name: "disabled"
            when: compulsionState.isDisabled || (dynamicState != null && dynamicState.isDisabled)
            PropertyChanges {
                target: dynamicTextLine
                color: dynamicColor.normalColor
            }
        },
        State {
            name: "normal"
            when: compulsionState.isNormal || (dynamicState != null && dynamicState.isNormal)
            PropertyChanges {
                target: dynamicTextLine
                color: dynamicColor.normalColor
            }
        }
    ]

    TikoDynamicState {
        id: compulsionState
    }
}
