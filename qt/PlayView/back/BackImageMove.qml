import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Effects
import MediaerAPI
import Tiko
import PlayView

//纯色底片
Rectangle{
    color: TikoSeit.theme.baseTheme.backgroundNormal
    property string iconUrl: ""
    property Timer moveTimer: moveTimerItem
    clip: true

    //模糊背景
    AutoCoverImage {
        id: backCover
        width: 100
        height: 100
        loadMsTime: 1
        visible: false
        loadFlag: true
        baseUrl: iconUrl
    }

    MultiEffect {
        id: effectCover
        autoPaddingEnabled: true
        source: backCover
        width: parent.width * 3
        height: parent.height * 3
        blurEnabled: true
        blurMax: 50
        blur: 1.0

        property int stepX: getRand()
        property int stepY: getRand()
        property bool addX: true
        property bool addY: true

        function getRand(){
            return Math.floor(Math.random() * 5 + 2)
        }

        function move(){
            const minX = pageMain.width - effectCover.width;
            const minY = pageMain.height - effectCover.height;
            const maxX = 0;
            const maxY = 0;

            let moveX = addX ? effectCover.stepX : -effectCover.stepX;
            let moveY = addY ? effectCover.stepY : -effectCover.stepY;

            // 计算x方向
            if (effectCover.x === minX){
                effectCover.addX = true
                effectCover.stepX = getRand()
                moveX = effectCover.stepX
            }
            else if (effectCover.x === maxX){
                effectCover.addX = false
                effectCover.stepX = getRand()
                moveX = -effectCover.stepX
            }

            // 限制位置
            if (moveX > 0 && effectCover.x + moveX > maxX){
                moveX = maxX - effectCover.x
            }
            else if (moveX < 0 && effectCover.x + moveX < minX){
                moveX = minX - effectCover.x
            }

            // 计算y方向
            if (effectCover.y === minY){
                effectCover.addY = true
                effectCover.stepY = getRand()
                moveY = effectCover.stepY
            }
            else if (effectCover.y === maxY){
                effectCover.addY = false
                effectCover.stepY = getRand()
                moveY = -effectCover.stepY
            }

            // 限制位置
            if (moveY > 0 && effectCover.y + moveY > maxY){
                moveY = maxY - effectCover.y
            }
            else if (moveY < 0 && effectCover.y + moveY < minY){
                moveY = minY - effectCover.y
            }

            effectCover.x += moveX
            effectCover.y += moveY
        }

        Timer {
            id: moveTimerItem
            interval: 33
            repeat: true
            onTriggered: effectCover.move()
        }
    }

    Rectangle{
        anchors.fill: parent
        color: TikoSeit.theme.baseTheme.transparentAntiEmphasize
    }
}
