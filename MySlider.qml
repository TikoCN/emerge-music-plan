import QtQuick
import QtQuick.Controls.Basic
import MyAPI

// Horizontal 水平
// Vertical 竖直
Slider {
    id: root

    property double radius: 0

    //背景条
    background: Item{
        width: root.width
        height: root.height
        Rectangle{
            width: root.width
            height: root.height
            color: Setting.transparentColor
            opacity: 0.05
            radius: root.radius
        }
        Rectangle{
            radius: root.radius
            width: {
                if(root.orientation === Qt.Horizontal){
                    return root.visualPosition * root.width
                }
                else{
                    return root.width
                }
            }

            height: {
                if(root.orientation === Qt.Vertical){
                    return (1 - root.visualPosition) * root.height
                }
                else{
                    return root.height
                }
            }

            y:{
                if(root.orientation === Qt.Vertical){
                    return root.height - height
                }
                else{
                    return 0
                }
            }

            color: Setting.themeColor
            opacity: 0.4
        }
    }

    handle: Rectangle{
        x: {
            if(root.orientation === Qt.Horizontal){
                return root.visualPosition * root.width - width / 2
            }
            else{
                return root.width / 2 - width / 2
            }
        }

        y: {
            if(root.orientation === Qt.Vertical){
                return root.visualPosition * root.height - height / 2
            }
            else{
                return root.height / 2 - height / 2
            }
        }

        color: Setting.themeColor
        property var min: {
            if(root.orientation === Qt.Vertical){
                return root.width
            }
            else{
                return root.height
            }
        }
        width: min
        height: min
        radius: min
    }
}
