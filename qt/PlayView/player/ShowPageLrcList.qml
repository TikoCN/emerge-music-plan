import QtQuick
import MediaerAPI   
import Tiko
import PlayView

Item {
    id: playerLrcPlayList
    property bool have: false
    Component.onCompleted: buildLrcList()

    MouseArea{
        id: mouseHover
        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        hoverEnabled: true
        onClicked: (mouse)=>{
                       if (mouse.button === Qt.RightButton){
                           var menu = comMenu.createObject(mouseHover)
                           menu.popup()
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
                    onClicked: CoreData.editMusic(playerLrcPlayList, MediaPlayer.playingMusic)
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
            lrcId: lrc
        }

        model:ListModel{
            id:lrcDataList
        }
    }

    //关联
    Connections{
        target: MediaPlayer
        function onLrcLoaded(){
            playerLrcPlayList.buildLrcList()
        }

        function onPlayingLrcIdChange(playingLrcId){
            if (!show &&
                    !mouseHover.containsMouse){
                return
            }
            lrcList.currentIndex = playingLrcId
        }
    }

    //插入歌词
    function buildLrcList(){
        var length = MediaPlayer.getLrcListLength()
        for(var i=0; i<length; i++){
            lrcDataList.append({lrc:i})
        }
    }
}
