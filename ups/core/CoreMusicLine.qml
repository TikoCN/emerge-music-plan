import QtQuick
import QtQuick.Layouts
import MediaerAPI
import Tiko
import DataCore
import Ups

Item {
    id: musicLine
    implicitHeight: 70
    implicitWidth: 100
    clip: true

    property int listId: 0
    property int musicId: -1
    property bool isLittle: false
    signal play()

    property MusicData music : visible ? Core.getMusic(musicId) : null

    MouseArea{
        id: mouseArea
        onClicked:(mouse)=>{
                      if(mouse.button === Qt.RightButton){
                          CoreData.openMenuMusic(musicLine, music, 0)
                      }
                      else{
                          play()
                      }
                  }
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        anchors.fill: parent
        hoverEnabled: true

        Row{
            x:10
            y:10
            width: parent.width - 20
            height: parent.height - 20
            spacing: 6

            //封面
            TikoImageAuto {
                id: cover
                width: 50
                height: 50
                normalUrl: "qrc:/image/music.png"
                loadUrl: "image://cover/musicFile:" + musicId.toString()
                loadFlag: visible && music !== null
            }

            Column{
                id: coreName
                width: isLittle ? (parent.width - cover.width - 30) / 2 :
                                  (parent.width - tool.width - cover.width - 30) / 2

                TikoTextLine{
                    text: music !== null ? music.title : qsTr("标题")
                    exSize: 3
                    font.bold: true
                    height: 30
                    width: parent.width
                }
                TikoTextLine{
                    text: music !== null ? music.artist : qsTr("作曲家")
                    height: 20
                    width: parent.width
                }
            }

            TikoTextLine{
                text: music !== null ? music.album : qsTr("专辑")
                width: coreName.width
                height: 50
            }

            Row{
                id: tool
                spacing: 3
                visible: !isLittle

                Row {
                    id: hoverItem
                    width: 250
                    height: 50
                    spacing: 10

                    TikoButtonIcon{
                        text: qsTr("喜爱")
                        visible: mouseArea.containsMouse
                        width: 50
                        height: 50
                        onClicked: {
                            if (music !== null)
                            music.isLove = !music.isLove
                        }
                        icon.source: music !== null &&  music.isLove ?
                                         "qrc:/image/love.png" : "qrc:/image/unlove.png"
                        useAutoColor: music !== null && !music.isLove
                        cache: true
                    }

                    // 评级
                    Row{
                        width: 100
                        height: 50
                        spacing: 1
                        visible: mouseArea.containsMouse

                        Repeater{
                            delegate: TikoButtonIcon{
                                width: 20
                                height: 50
                                hover: 0
                                cache: true
                                onClicked: {
                                    if (music !== null)
                                    music.level = level
                                }
                                icon.source: music !== null && music.level >= level ?
                                                 "qrc:/image/int.png" : "qrc:/image/unInt.png"
                                useAutoColor: music !== null && !(music.level >= level)
                            }
                            model: ListModel{
                                ListElement{level: 1}
                                ListElement{level: 2}
                                ListElement{level: 3}
                                ListElement{level: 4}
                                ListElement{level: 5}
                            }
                        }
                    }
                    TikoButtonIcon{
                        text: qsTr("更多")
                        visible: mouseArea.containsMouse
                        width: 50
                        height: 50
                        onClicked: CoreData.openMenuMusic(musicLine, music, 0)
                        icon.source: "qrc:/image/else.png"
                        cache: true
                    }
                }

                TikoTextLine{
                    text: music !== null ? music.getStringTime() : qsTr("00:00")
                    width: CoreData.timeWidth
                    height: 50
                }
                TikoTextLine{
                    text: music !== null ? music.lastEdit : qsTr("00:00")
                    width: CoreData.editTimeWidth
                    height: 50
                }
                TikoTextLine{
                    text: music !== null ? music.playNumber.toString() : qsTr("00:00")
                    width: CoreData.playNumberWidth
                    height: 50
                }
            }
        }
    }

    Rectangle{
        anchors.fill: parent
        radius: 5
        color: Setting.transparentColor//背景颜色
        opacity:{
            if(musicLine.listId % 2 === 1){
                opacity = 0.1
            }
            else{
                opacity = 0.05
            }
        }
    }
}

