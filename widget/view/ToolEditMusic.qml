import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Widget
import TikoAPI
import Tiko

TikoFrameless {
    id: toolEditMusic
    title: qsTr("编辑歌词")
    width: 800
    height: 600

    Rectangle{
        anchors.fill: parent
        color: TikoSeit.backdropColor
        topLeftRadius: 10
        topRightRadius: 10
    }

    Item{
        id: tool
        width: parent.width
        height: 30
        anchors.margins: 10

        TikoButtonNormal{
            id: editLrcButton
            anchors.left: tool.left
            text: qsTr("编辑歌词")
            onClickLeft: toolEditMusic.stackEditLrcPage()
            iconSource: "qrc:/image/editLrc.png"
        }

        TikoButtonNormal{
            anchors.left: editLrcButton.right
            anchors.leftMargin: 10
            text: qsTr("编辑音乐")
            onClickLeft: toolEditMusic.stackEditMusicPage()
            iconSource: "qrc:/image/editMusic.png"
        }

        TikoButtonIcon{
            anchors.right: parent.right
            icon.source: "qrc:/image/close.png"
            text: qsTr("关闭")
            onClicked: toolEditMusic.close()
        }
    }

    StackView{
        id: stackView
        anchors.top: tool.bottom
        anchors.left: parent.left
        anchors.margins: 10
        width: parent.width - 20
        height: parent.height - tool.height
        initialItem: editMusicPage

        Rectangle{
            anchors.fill: stackView
            color: TikoSeit.transparentColor
            opacity: 0.05
            topLeftRadius: 10
            topRightRadius: 10
        }
    }

    EditMusicPage{
        id: editMusicPage
        visible: false
    }

    EditLrcPage{
        id: editLrcPage
        visible: false
    }


    // 切换到编辑音乐页
    function stackEditMusicPage(){
        if(stackView.currentItem != editMusicPage){
            stackView.replace(editMusicPage)
        }
    }

    // 切换到编辑歌词页
    function stackEditLrcPage(){
        if(stackView.currentItem != editLrcPage){
            stackView.replace(editLrcPage)
        }
    }

    function build(music, lrc){
        editMusicPage.init(music)
        editLrcPage.init(lrc)
    }
}
