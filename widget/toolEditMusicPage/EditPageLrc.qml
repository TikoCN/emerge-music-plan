import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Tiko
import TikoAPI

Item{
    id: editLrcPage
    visible: false
    property real lineStart: 0
    property real wordStart: 0
    property string lrcUrl: ""
    property int mode: 0

    Column {
        id: editLrctool
        spacing: 6
        width: 150

        TikoButtonCombox{
            width: 150
            data: ["0.1", "0.3", "0.5", "1", "2", "3"]
            show: 3
            helpText: qsTr("播放速度：")
            onShowTextChanged: {
                var rate = Number(showText)
                MediaPlayer.player.setPlaybackRate(rate)
            }
        }

        TikoButtonCombox{
            width: 150
            data: [qsTr("逐字模式"), qsTr("逐词模式")]
            helpText: qsTr("工作模式：")
            onShowChanged: editLrcPage.mode = show
        }

        TikoButtonNormal{
            width: 150
            iconSource: "qrc:/image/lineStart.png"
            text: qsTr("行开始") + " : " + editLrcPage.lineStart.toString()
            onClick: editLrcPage.lineStart = MediaPlayer.player.position
        }

        TikoButtonNormal{
            width: 150
            iconSource: "qrc:/image/lineEnd.png"
            text: qsTr("行结束")
            onClick: editLrcPage.hlrcInsertLineTime()
        }

        TikoButtonNormal{
            width: 150
            iconSource: "qrc:/image/wordStart.png"
            text: qsTr("词开始") + " : " + editLrcPage.wordStart.toString()
            onClick: editLrcPage.wordStart = MediaPlayer.player.position
        }

        TikoButtonNormal{
            width: 150
            iconSource: "qrc:/image/wordEnd.png"
            text: qsTr("词结束")
            onClick: editLrcPage.hlrcInsertWordTime()
        }

        TikoButtonNormal{
            width: 150
            iconSource: "qrc:/image/wordEnd.png"
            text: qsTr("连续词结束")
            onClick: {
                editLrcPage.wordStart = MediaPlayer.player.position
                editLrcPage.hlrcInsertWordTime()
            }
        }

        TikoButtonNormal{
            width: 150
            iconSource:"qrc:/image/lineReplaceTime.png"
            text:qsTr("修正时间戳")
            onClick: {
                editLrcPage.timeWork(0)
                editLrcPage.cursorNext()
            }
        }

        TikoButtonNormal{
            width: 150
            iconSource: "qrc:/image/lineAddTime.png"
            text: qsTr("添加时间戳")
            onClick: editLrcPage.timeWork(1)
        }

        TikoButtonNormal{
            width: 150
            iconSource: "qrc:/image/lineDeleteTime.png"
            text: qsTr("删除时间戳")
            onClick: editLrcPage.timeWork(2)
        }

        TikoButtonNormal{
            width: 150
            iconSource: "qrc:/image/yes.png"
            text: qsTr("保存")
            onClick: Base.writeFileText(editLrcPage.lrcUrl, lrcShow.text)
        }
    }

    ScrollView {
        id: lrcShowSpace
        anchors.left: editLrctool.right
        width: parent.width - editLrctool.width
        height: parent.height
        anchors.margins: 10

        ScrollBar.horizontal.visible: false
        ScrollBar.vertical: TikoBarV {parent: lrcShowSpace}

        background: Rectangle{
            color: Setting.transparentColor
            opacity: 0.05
            radius: 10
        }

        Item{
            id: showItem
            implicitHeight: lrcShow.height
            width: lrcShowSpace.width
            TextEdit{
                id: lrcShow
                width: showItem.width
                cursorVisible: true
                inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhNoPredictiveText
                onCursorVisibleChanged: lrcShow.cursorVisible = true
                font.family: Setting.mainFont.family
                font.pixelSize: Setting.mainFont.pixelSize + lrcShow.exFontSize

                property int exFontSize: 5

                cursorDelegate: Rectangle{
                    height: 10
                    width: 3
                    color: Setting.themeColor
                    opacity: 0.3
                }
            }
        }
    }

    function init(lrc, lrcUrl){
        lrcShow.text = lrc
        editLrcPage.lrcUrl = lrcUrl
    }

    function insertString(pos, input){
        var lrc = lrcShow.text //行分割
        var text = ""
        for(var i=0; i<lrc.length; i++){
            if(i === pos){
                text += input
            }
            text += lrc[i]
        }
        lrcShow.text = text
        var aimPos = pos + input.length + 1

        switch (editLrcPage.mode) {
        case 0:
            // 字分割，跳过换换行符，和空格
            while(text.length > aimPos + 1 && (text[aimPos] === "\n" || text[aimPos] === " ")){
                aimPos++
            }
            break
        case 1:
            // 词分割，跳过词组，找到空格
            while(text.length > aimPos + 1 && text[aimPos] !== " "){
                aimPos++
            }
            if(text.length > aimPos + 1 && text[aimPos] === " "){
                aimPos++
            }
            break
        }

        if(text.length > aimPos){
            lrcShow.cursorPosition = aimPos
        }
        else{
            lrcShow.cursorPosition = aimPos - 1
        }
    }

    function hlrcInsertLineTime(){
        var time = "["+ editLrcPage.lineStart.toString() +","+ MediaPlayer.player.position.toString() +"]"
        insertString(lrcShow.cursorPosition, time)
    }

    function hlrcInsertWordTime(){
        var time = "("+ editLrcPage.wordStart.toString() +","+ MediaPlayer.player.position.toString() +")"
        insertString(lrcShow.cursorPosition, time)
    }

    //替换当前行的时间戳
    function timeWork(type){
        var size = lrcShow.cursorPosition
        //找到上一个 \n
        var lrcList = lrcShow.text.split("\n") //行分割
        var aimLine = 0
        for(let i =0; i<lrcList.length; i++){
            //长度超过本行，下一行
            if(size > lrcList[i].length){
                size -= lrcList[i].length + 1
            }
            else{//找到目标行退出
                aimLine = i
                break
            }
        }

        var line = lrcList[aimLine]//当前行内容
        var re = /\[([^\]]*)\]/;//正则表达式
        var time = "["+MediaPlayer.getTimeString()+"]"
        var newLine = line
        switch(type){
        case 0://替换时间戳
            newLine = line.replace(re, time)
            break
        case 1:
            newLine = time + line
            break

        case 2:
            newLine = line.replace(re, "")
            if(newLine === "") newLine = " "
            break
        }
        //替换到文本中
        lrcShow.text = lrcShow.text.replace(line, newLine)

        lrcList = lrcShow.text.split("\n") //行分割
        var aimPos = 0
        for(let j=0; j<aimLine; j++){
            aimPos += lrcList[j].length + 1
        }
        lrcShow.cursorPosition = aimPos//光标复位
    }


    //跳到下一行
    function cursorNext(){
        for(var i=lrcShow.cursorPosition; i<lrcShow.text.length; i++){
            if(lrcShow.text[i] === "\n"){
                lrcShow.cursorPosition = i+1
                break
            }
        }
    }

    //获得时间戳的文本表示
    function getDateString(n){
        var ms = n % 1000
        var s = n / 1000 % 60
        var m = n / 1000 / 60
        return "[" + m.toString() +":"+ s.toString() +"."+ ms.toString() + "]"
    }
}
