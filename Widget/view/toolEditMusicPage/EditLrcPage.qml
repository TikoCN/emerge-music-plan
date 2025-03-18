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

    Column {
        id: editLrctool
        spacing: 10
        width: 150

        TikoButtonNormal{
            width: 150
            iconSource: "qrc:/image/lineStart.png"
            text: qsTr("行开始")
            onClickLeft: editLrcPage.lineStart = MediaPlayer.player.position
        }

        TikoButtonNormal{
            width: 150
            iconSource: "qrc:/image/lineEnd.png"
            text: qsTr("行结束")
        }

        TikoButtonNormal{
            width: 150
            iconSource: "qrc:/image/wordStart.png"
            text: qsTr("词开始")
            onClickLeft: editLrcPage.wordStart = MediaPlayer.player.position
        }

        TikoButtonNormal{
            width: 150
            iconSource: "qrc:/image/wordEnd.png"
            text: qsTr("词结束")
        }

        TikoButtonNormal{
            width: 150
            iconSource:"qrc:/image/lineReplaceTime.png"
            text:qsTr("修正时间戳")
            onClickLeft: {
                editLrcPage.timeWork(0)
                editLrcPage.cursorNext()
            }
        }

        TikoButtonNormal{
            width: 150
            iconSource: "qrc:/image/lineAddTime.png"
            text: qsTr("添加时间戳")
            onClickLeft: editLrcPage.timeWork(1)
        }

        TikoButtonNormal{
            width: 150
            iconSource: "qrc:/image/lineDeleteTime.png"
            text: qsTr("删除时间戳")
            onClickLeft: editLrcPage.timeWork(2)
        }

        TikoButtonNormal{
            width: 150
            iconSource: "qrc:/image/yes.png"
            text: qsTr("保存")
            onClickLeft: core.writeDataToFile(lrcShow.text)
        }
    }

    ScrollView {
        id: lrcShowSpace
        anchors.left: editLrctool.right
        width: editLrcPage.width - editLrctool.width
        anchors.margins: 10

        ScrollBar.horizontal.visible: false
        ScrollBar.vertical: TikoBar {
            parent: lrcShowSpace
        }

        background: Rectangle{
            color: Setting.transparentColor
            opacity: 0.05
            radius: 10
        }

        TextEdit{
            id: lrcShow
            anchors.fill: parent
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

    function init(lrc){
        lrcShow.text = lrc
    }

    function insertString(input){
        var pos = lrcShow.cursorPosition
        var lrc = lrcShow.text.split("\n") //行分割
        var text = ""
        for(var i=0; i<lrc.length; i++){
            text += lrc[i]

            if(i === pos){
                text += input
            }
        }
        lrcShow.text = text
        lrcShow.cursorPosition = pos + input.length
    }

    function hlrcInsertLineTime(){

    }

    function hlrcInsertWordTime(){

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
