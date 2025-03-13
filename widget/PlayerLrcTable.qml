import QtQuick
import TikoAPI
import Tiko

Item {
    id:playerLrcTable
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

        TikoMenu{
            id: menu

            TikoMenuItem{text: qsTr("字体加大"); icon.source: "qrc:/image/size+.png"; onClicked: Setting.mainLrcFont.pixelSize++}
            TikoMenuItem{text: qsTr("字体减小"); icon.source: "qrc:/image/size+.png"; onClicked: Setting.mainLrcFont.pixelSize--}
        }
    }

    ListView{
        id:lrcList
        anchors.fill:parent

        delegate: CoreLrcLine{
            width: lrcList.width
            core: MediaPlayer.lrcList[lrc]
        }

        model:ListModel{
            id:lrcDataList
        }
    }

    Component.onCompleted: buildLrcList()

    //关联
    Connections{
        target:MediaPlayer
        function onCppLrcLoaded(size){
            playerLrcTable.buildLrcList()
        }

        function onCppPlayingLrc(){
            if(!show && MediaPlayer.lrcList.length <= 1 && !mouseHover.containsMouse){
                return
            }

            var playLine = MediaPlayer.playingLrc.id
            lrcList.currentIndex = playLine
            lrcList.positionViewAtIndex(playLine, ListView.Beginning)
        }
    }

    //插入歌词
    function buildLrcList(){
        lrcDataList.clear()
        for(var i=0; i<MediaPlayer.lrcList.length; i++){
            lrcDataList.append({lrc:i})
        }
    }
}
