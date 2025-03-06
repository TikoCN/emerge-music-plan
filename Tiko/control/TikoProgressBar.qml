import QtQuick
import QtQuick.Controls
import "../"

Item {
    id: progressBar

    property double precent: 0
    property color backColor: TikoSeit.backdropColor
    property color usedColor: TikoSeit.themeColor
    property color transparentColor: TikoSeit.transparentColor
    property double radius: progressBar.height

    Item{
        width: progressBar.width - precentShow.width - 20
        height: progressBar.height

        //背景
        Rectangle{
            anchors.fill: parent
            color: progressBar.transparentColor
            opacity: 0.05
            radius: progressBar.radius
        }

        //进度条
        Rectangle{
            height: parent.height
            width: parent.width * progressBar.precent
            color: progressBar.usedColor
            radius: progressBar.radius
        }
    }

    //文本显示
    Item{
        id: precentShow
        anchors.right: parent.right
        width: 70
        height: progressBar.height

        TikoAutoText {
            property int finish: progressBar.precent * 100
            anchors.fill: parent
            text: finish.toString()+ "%"
        }
    }
}
