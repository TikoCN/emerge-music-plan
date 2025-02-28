import QtQuick
import QtQuick.Controls
import "../"

TikoPopup {
    id: littleMessage
    width: icon.width + messageShow.width + 60
    height: messageShow.height
    padding: 0
    y: 0
    backColor: TikoSeit.themeColor
    borderColor: TikoSeit.themeColor

    property string message: qsTr("消息送达")
    property int type: 0

    Image {
        id: icon
        source: {
            switch(littleMessage.type){
            case 0:
                return "qrc:/icon/success.png"
            case 1:
                return "qrc:/icon/error.png"
            }
        }

        cache: false
        height: messageShow.height * 0.8
        width: messageShow.height * 0.8
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.verticalCenter: parent.verticalCenter
    }

    //文本展示
    TikoAutoText{
        id: messageShow
        text: littleMessage.message
        width: mathWidth.advanceWidth
        height: 30
        anchors.right: parent.right
        anchors.rightMargin: 30
    }

    //计算文本长度，用于设置宽度
    TextMetrics{
        id: mathWidth
        text: littleMessage.message
        font: messageShow.font
    }

    //进入动画
    ParallelAnimation{
        id: showAnimation

        NumberAnimation{
            target: littleMessage
            property: "y"
            to: littleMessage.parent.height / 6 - littleMessage.height / 2
            duration: 100
        }
    }

    //关闭动画
    ParallelAnimation{
        id: closeAnimation
        onStopped: littleMessage.destroy()


        NumberAnimation{
            target: littleMessage
            property: "y"
            to: - littleMessage.height
            duration: 100
        }
    }

    //自动关闭计时器
    Timer{
        id: timer
        interval: 3000
        onTriggered: closeAnimation.start()
    }

    function show(){
        littleMessage.open()
        showAnimation.start()
        timer.start()
    }
}
