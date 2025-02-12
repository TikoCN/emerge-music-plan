import QtQuick
import MyAPI

Item {
    id:playerLrcTable
    property bool show: false
    property bool have: false

    MouseArea{
        id:mouseHover
        anchors.fill:parent
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        hoverEnabled:true
        onClicked: (mouse)=>{
                       if (mouse.button === Qt.RightButton){
                           menu.open()
                       }
                   }

        MyMenu{
            id: menu

            MyMenuItem{text: qsTr("字体加大"); icon.source: "qrc:/image/size+.png"; onClicked: Setting.mainLrcFont.pixelSize++}
            MyMenuItem{text: qsTr("字体减小"); icon.source: "qrc:/image/size+.png"; onClicked: Setting.mainLrcFont.pixelSize--}
        }
    }

    ListView{
        id:lrcList
        anchors.fill:parent
        clip: true

        delegate: CoreLrcLine{
            width: lrcList.width
            height: 70
            lrcFont: Setting.mainLrcFont
            lrcData: MediaPlayer.lrcList[lrc]
        }

        model:ListModel{
            id:lrcDataList
        }
    }

    //关联
    Connections{
        target:MediaPlayer
        function onCppLrcLoaded(size){
            playerLrcTable.buildLrcList(size)
        }

        function onCppPlayingLrc(){
            if(!show && MediaPlayer.lrcList.length <= 1 && !mouseHover.containsMouse){
                return
            }

            var playLine = MediaPlayer.playingLrc.id
            lrcList.currentIndex = playLine
            lrcList.positionViewAtIndex(playLine, ListView.Center)
        }
    }

    //插入歌词
    function buildLrcList(size){
        lrcDataList.clear()
        for(var i=0; i<size; i++){
            lrcDataList.append({lrc:i})
        }
    }
}
