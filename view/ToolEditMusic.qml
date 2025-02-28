import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import TikoAPI
import Tiko

TikoFrameless {
    id: toolEditMusic
    title: qsTr("编辑歌词")

    property var core

    TikoUiButton{
        id: noEdit
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        icon.source: "qrc:/image/close.png"
        text: qsTr("关闭")
        onClicked: toolEditMusic.close()
    }

    RowLayout{
        id: tool
        x: 10
        y: 10
        spacing: 10
        height: 30

        TikoButton{
            id:downMark
            icon.source:"qrc:/image/downY.png"
            text:qsTr("重新添加时间戳并跳转下一行")
            onClicked:{
                toolEditMusic.timeWork(0)
                toolEditMusic.cursorNext()
            }
        }

        TikoUiButton{
            id: editMark
            icon.source: "qrc:/image/addR.png"
            text: qsTr("添加时间戳")
            onClicked: toolEditMusic.timeWork(1)
        }

        TikoUiButton{
            id: deleteMark
            icon.source: "qrc:/image/minR.png"
            text: qsTr("删除时间戳")
            onClicked: toolEditMusic.timeWork(2)
        }

        TikoUiButton{
            id: yesEdit
            icon.source: "qrc:/image/yesR.png"
            text: qsTr("确定编辑并保存")
            onClicked:ToolHelper.finishEdit(core.coreId, lrcShow.text, title.input.text, artist.input.text, alumb.input.text, genre.input.text, year.input.text)
        }
    }

    RowLayout{
        id: musicData
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: tool.bottom
        anchors.topMargin: 10
        width: parent.width - 20
        spacing: 10

        property int showW: 30
        property int itemW: (musicData.width - 50) / 5

        TikoInputText{
            id: title
            show.text: qsTr("标题")
            show.width: musicData.showW
            implicitWidth: musicData.itemW
        }

        TikoInputText{
            id: artist
            show.text: qsTr("歌手")
            show.width: musicData.showW
            implicitWidth: musicData.itemW
        }

        TikoInputText{
            id: alumb
            show.text: qsTr("专辑")
            show.width: musicData.showW
            implicitWidth: musicData.itemW
        }

        TikoInputText{
            id: genre
            show.text: qsTr("流派")
            show.width: musicData.showW
            implicitWidth: musicData.itemW
        }

        TikoInputText{
            id: year
            show.text: qsTr("年份")
            show.width: musicData.showW
            implicitWidth: musicData.itemW
        }
    }

    ScrollView{
        width: parent.width
        anchors.top: musicData.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10

        ScrollBar.horizontal.visible: false
        ScrollBar.vertical: TikoBar{}

        background:Rectangle{
            color: Setting.transparentColor
            opacity: 0.05
            radius: 10
        }

        TextEdit{
            id: lrcShow
            anchors.fill: parent
            cursorVisible: true
            inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhNoPredictiveText
            onCursorVisibleChanged: cursorVisible = true

            cursorDelegate: Rectangle{
                height: 10
                width: 3
                color: Setting.themeColor
                opacity: 0.3
            }
        }
    }

    function buildLrc(lrc, coreId){
        core = MediaPlayer.coreList[coreId]
        lrcShow.text = lrc

        show()

        artist.input.text = core.artist
        title.input.text = core.title
        alumb.input.text = core.alumb
        genre.input.text = core.genre
        year.input.text = core.year
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
