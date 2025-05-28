import QtQuick
import MediaerAPI   
import Tiko
import PlayView

Item {
    id: playerLrcTable
    property bool have: false

    MouseArea{
        id: mouseHover
        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        hoverEnabled: true
        onClicked: (mouse)=>{
                       if (mouse.button === Qt.RightButton){
                           var menu = comMenu.createObject(mouseHover)
                           menu.open()
                       }
                   }

        Component{
            id: comMenu

            TikoMenu{
                id: menu
                onClosed: menu.destroy()

                TikoMenuItem{
                    text: qsTr("字体加大");
                    icon.source: "qrc:/image/size+.png";
                    onClicked: Setting.mainLrcFont.pixelSize++
                }
                TikoMenuItem{
                    text: qsTr("字体减小");
                    icon.source: "qrc:/image/size+.png";
                    onClicked: Setting.mainLrcFont.pixelSize--
                }
                TikoMenuSpeacer{}
                TikoMenuItem{
                    text: qsTr("打开歌词")
                    onClicked: MediaPlayer.playingMusic.openMusicLrc()
                }
                TikoMenuItem{
                    text: qsTr("编辑歌词")
                    onClicked: CoreData.editMusic(playerLrcTable, MediaPlayer.playingMusic)
                }
            }
        }
    }

    ListView{
        id:lrcList
        anchors.fill:parent
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0

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
        target: MediaPlayer
        function onLrcLoaded(){
            playerLrcTable.buildLrcList()
        }

        function onPlayingLrcIdChange(){
            if(
                    !show &&
                    MediaPlayer.lrcList.length <= 1 &&
                    MediaPlayer.playingLrc === null &&
                    !mouseHover.containsMouse
                    ){
                return
            }

            var playLine = MediaPlayer.playingLrc.id

            lrcList.currentIndex = playLine
        }

        function onClearLrc(){
            lrcDataList.clear()
        }
    }

    //插入歌词
    function buildLrcList(){
        for(var i=0; i<MediaPlayer.lrcList.length; i++){
            lrcDataList.append({lrc:i})
        }
    }
}
