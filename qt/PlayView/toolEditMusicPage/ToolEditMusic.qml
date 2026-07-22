import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import MediaerAPI
import Tiko
import PlayView

TikoFrameless {
    id: toolEditMusic
    title: qsTr("编辑歌词")
    width: 800
    height: 600

    Rectangle{
        anchors.fill: parent
        color: TikoSeit.theme.colorBgDefault
        topLeftRadius: 10
        topRightRadius: 10
        z: -1
    }

    Item{
        id: tool
        width: toolEditMusic.width - 20
        height: 50
        anchors.margins: 10
        anchors.top: parent.top
        anchors.left: parent.left

        TikoButtonDefault{
            id: editLrcButton
            anchors.left: tool.left
            textLine.text: qsTr("编辑歌词")
            onLeftClicked: toolEditMusic.stackEditLrcPage()
            icon.source: "qrc:/image/editLrc.png"
        }

        TikoButtonDefault{
            anchors.left: editLrcButton.right
            anchors.leftMargin: 10
            textLine.text: qsTr("编辑音乐")
            onLeftClicked: toolEditMusic.stackEditMusicPage()
            icon.source: "qrc:/image/editMusic.png"
        }

        TikoButtonIcon{
            anchors.right: parent.right
            icon.source: "qrc:/image/close.png"
            onClicked: toolEditMusic.destroy()
        }
    }

    StackView{
        id: stackView
        anchors.top: tool.bottom
        anchors.left: parent.left
        anchors.margins: 20
        width: parent.width - 40
        height: parent.height - tool.height - 40
    }

    EditPageMusic {
        id: musicEditPage
        visible: false
    }

    EditPageLrc {
        id: lrcEditPage
        visible: false
    }


    // 切换到编辑音乐页
    function stackEditMusicPage(){
        if(stackView.currentItem != musicEditPage){
            stackView.replace(musicEditPage)
        }
    }

    // 切换到编辑歌词页
    function stackEditLrcPage(){
        if(stackView.currentItem != lrcEditPage){
            stackView.replace(lrcEditPage)
        }
    }

    function build(musicId){
        musicEditPage.init(musicId)
        lrcEditPage.init(musicId)
        stackEditMusicPage()
    }
}
