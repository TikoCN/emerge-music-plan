import QtQuick
import Tiko

TikoTextLine {
    id: dynamicTextLine
    property TikoDynamicColor dynamicColor: TikoSeit.theme.foregroundDynamicColor
    property TikoDynamicState dynamicState: null

    property bool enableUnifiedColor: false
    property color unifiedColor: Qt.black

    states: [
        State {
            name: "normal"
            when: dynamicState != null && dynamicState.isNormal
            PropertyChanges {
                target: dynamicTextLine
                color: dynamicColor.normalColor
            }
        },
        State {
            name: "hover"
            when: dynamicState != null && dynamicState.isHover
            PropertyChanges {
                target: dynamicTextLine
                color: dynamicColor.hoverColor
            }
        },
        State {
            name: "pressed"
            when: dynamicState != null && dynamicState.isPressed
            PropertyChanges {
                target: dynamicTextLine
                color: dynamicColor.normalColor
            }
        },
        State {
            name: "disabled"
            when: dynamicState != null && dynamicState.isDisabled
            PropertyChanges {
                target: dynamicTextLine
                color: dynamicColor.normalColor
            }
        }
    ]
}
