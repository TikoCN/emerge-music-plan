import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import MyAPI

ScrollView{
    id: seitPage
    ScrollBar.horizontal.visible: false
    ScrollBar.vertical: MyBar{}

    property double modeTextWidth: 150
    
    Column{
        id: column
        x: 10
        y: 10
        width: parent.width
        spacing: 10
        property double vesselWidth: parent.width * 0.8

        Item{
            width: column.parent.width
            height: 1
        }
        
        //资源文件管理
        MyRightVessel{
            width: column.vesselWidth
            show.text: qsTr("资源文件管理")
            show.width: seitPage.modeTextWidth

            vessel: Item{
                height: childrenRect.height

                MyBorderButton{
                    id: selectDirButton
                    width: 150
                    height: 50
                    text: qsTr("选择音乐文件地址")
                    onClicked: selectMusicDir.open()
                }

                MyBorderButton{
                    anchors.left: selectDirButton.right
                    anchors.leftMargin: 10
                    width: selectDirButton.width
                    height: 50
                    text: qsTr("加载音乐")
                    onClicked: Setting.loadMusicCores()
                }

                Rectangle{
                    color: Setting.transparentColor
                    anchors.fill: sourceList
                    opacity: 0.05
                }

                Column{
                    id: sourceList
                    anchors.top: selectDirButton.bottom
                    anchors.topMargin: 10
                    width: 600

                    Repeater{
                        model: ListModel{
                            id:musicSourceModel
                        }

                        delegate: MyAutoText {
                            text: url
                            background.opacity: {
                                if(i % 2 === 0){
                                    return 0.05
                                }
                                else{
                                    return 0
                                }
                            }
                            height: 40
                            width: sourceList.width
                            padding: 10
                        }
                    }

                    FolderDialog{
                        id: selectMusicDir
                        onAccepted: {
                            var url = selectMusicDir.selectedFolder
                            Setting.sourceList.push(url)
                            sourceList.bulidSoure()
                        }
                    }

                    //插入新条目
                    function bulidSoure(){
                        musicSourceModel.clear()
                        for(var i=0; i<Setting.sourceList.length; i++){
                            musicSourceModel.append({url:Setting.sourceList[i], i:i})
                        }
                    }

                    //初始化
                    Component.onCompleted: {
                        Setting.loadMusicCores()
                        bulidSoure()
                    }
                }
            }
        }

        //颜色管理
        MyRightVessel{
            width: column.vesselWidth
            show.text: qsTr("颜色管理")
            show.width: seitPage.modeTextWidth

            vessel: Column{
                spacing: 10

                SelectColor{
                    width: 200
                    text: qsTr("主题颜色")
                    selectedColor: Setting.themeColor
                    onSelectedColorChanged: {
                        Setting.themeColor = selectedColor
                    }
                }

                SelectColor{
                    width: 200
                    text: qsTr("背景颜色")
                    selectedColor: Setting.backdropColor
                    onSelectedColorChanged: {
                        Setting.backdropColor = selectedColor
                    }
                }

                SelectColor{
                    width: 200
                    text: qsTr("透明层颜色")
                    selectedColor: Setting.transparentColor
                    onSelectedColorChanged: {
                        Setting.transparentColor = selectedColor
                    }
                }

                SelectColor{
                    width: 200
                    text: qsTr("正在播放颜色")
                    selectedColor: Setting.playingLrcColor
                    onSelectedColorChanged: {
                        Setting.playingLrcColor = selectedColor
                    }
                }

                SelectColor{
                    width: 200
                    text: qsTr("完成播放颜色")
                    selectedColor: Setting.playedLrcColor
                    onSelectedColorChanged: {
                        Setting.playedLrcColor = selectedColor
                    }
                }
            }
        }

        //字体管理
        MyRightVessel{
            width: column.vesselWidth
            show.text: qsTr("字体管理")
            show.width: seitPage.modeTextWidth

            vessel: Column{
                spacing: 10

                SelectFont{
                    width: 200
                    height: 30
                    text: qsTr("ui字体")
                    selectedFont: Setting.mainFont

                    onSelectedFontChanged:{
                        Setting.mainFont = selectedFont
                    }
                }

                SelectFont{
                    width: 200
                    height: 30
                    text: qsTr("桌面滚动歌词字体")
                    selectedFont: Setting.deskFont

                    onSelectedFontChanged:{
                        Setting.deskFont = selectedFont
                    }
                }

                SelectFont{
                    width: 200
                    height: 30
                    text: qsTr("主页滚动歌词字体")
                    selectedFont: Setting.mainLrcFont

                    onSelectedFontChanged:{
                        Setting.mainLrcFont = selectedFont
                    }
                }
            }
        }

        //参数管理
        MyRightVessel{
            width: column.vesselWidth
            show.text: qsTr("参数管理")
            show.width: seitPage.modeTextWidth
            vessel: Column{
                function inputNumber(input, number){
                    var rx = /[^0-9]/
                    if(input.text.match(rx) !== null ){
                        popupMessage.show(qsTr("请输入数字"))
                        input.text = number.toString()
                    }
                    else if(Number(input.text) !== number){
                        return Number(input.text)
                    }
                    return number
                }

                MyInputText{
                    width: parent.width
                    show.text: qsTr("最大加载线程数")
                    show.width: 200
                    input.text: Setting.maxThreadNumber.toString()
                    input.onEditingFinished: Setting.maxThreadNumber = inputNumber(input, Setting.maxThreadNumber)
                }
            }
        }

        //选项管理
        MyRightVessel{
            width: column.vesselWidth
            show.text: qsTr("选项管理")
            show.width: seitPage.modeTextWidth
            vessel: Column{

                MyCheckButton{
                    width: parent.width * 0.6
                    text: qsTr("在线模块")
                    check: Setting.isOnLine
                    onCheckChanged: Setting.isOnLine = check
                }

                MyCheckButton{
                    width: parent.width * 0.6
                    text: qsTr("从网易获得封面")
                    check: Setting.isGetCoverFromNetEase
                    onCheckChanged: Setting.isGetCoverFromNetEase = check
                }

                MyCheckButton{
                    width: parent.width * 0.6
                    text: qsTr("从qq音乐获得歌词")
                    check: Setting.isGetCoverFromQQMusic
                    onCheckChanged: Setting.isGetCoverFromQQMusic = check
                }

                MyCheckButton{
                    width: parent.width * 0.6
                    text: qsTr("从baidu获得封面")
                    check: Setting.isGetCoverFromBaidu
                    onCheckChanged: Setting.isGetCoverFromBaidu = check
                }

                MyCheckButton{
                    width: parent.width * 0.6
                    text: qsTr("从bing获得封面")
                    check: Setting.isGetCoverFromBing
                    onCheckChanged: Setting.isGetCoverFromBing = check
                }

                MyCheckButton{
                    width: parent.width * 0.6
                    text: qsTr("从qq音乐获得歌词")
                    check: Setting.isGetLrcFromNetEase
                    onCheckChanged: Setting.isGetLrcFromNetEase = check
                }

                MyCheckButton{
                    width: parent.width * 0.6
                    text: qsTr("从网易获得歌词")
                    check: Setting.isGetLrcFromQQMusic
                    onCheckChanged: Setting.isGetLrcFromQQMusic = check
                }
            }
        }
    }
}
