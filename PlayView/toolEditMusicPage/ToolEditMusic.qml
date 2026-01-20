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
        color: TikoSeit.backdropColor
        topLeftRadius: 10
        topRightRadius: 10
    }

    Item{
        id: tool
        width: parent.width - 20
        height: 50
        anchors.margins: 10
        anchors.top: parent.top
        anchors.left: parent.left

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

    Rectangle{
        anchors.centerIn: stackView
        width: parent.width - 20
        height: parent.height - tool.height - 20
        color: TikoSeit.transparentColor
        opacity: 0.05
        topLeftRadius: 10
        topRightRadius: 10
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

    function build(musicId, lrc){
        musicEditPage.init(musicId)
        lrcEditPage.init(lrc, FileMan.getMusicLrcUrl(musicId))
        stackEditMusicPage()
    }
}
