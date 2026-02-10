import QtQuick
import Tiko

TikoTextLine {
    id: dynamicTextLine
    property TikoDynamicColor dynamicColor: TikoSeit.TextLineDynamicColor
    property TikoDynamicState dynamicState: null

    property bool enableUnifiedColor: false
    property color unifiedColor: Qt.black

    states: [
        State {
            name: "normal"
            when: dynamicState.isNormal
            PropertyChanges {
                target: dynamicTextLine
                color: dynamicColor.normalColor
            }
        },
        State {
            name: "hover"
            when: dynamicState.isHover
            PropertyChanges {
                target: dynamicTextLine
                color: dynamicColor.hoverColor
            }
        },
        State {
            name: "pressed"
            when: dynamicState.isPressed
            PropertyChanges {
                target: dynamicTextLine
                color: dynamicColor.normalColor
            }
        },
        State {
            name: "disabled"
            when: dynamicState.isDisabled
            PropertyChanges {
                target: dynamicTextLine
                color: dynamicColor.normalColor
            }
        }
    ]
}
