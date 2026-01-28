import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import MediaerAPI
import Tiko
import PlayView

TikoRightVessel{
    Layout.fillWidth: true
    Layout.preferredHeight: this.height
    show.text: qsTr("字体管理")

    vessel: Column{
        spacing: 10
        property double itemWidth: 300

        TikoSelectFont{
            width: parent.itemWidth
            height: 30
            text: qsTr("ui字体")
            selectedFont: Setting.mainFont

            onSelectedFontChanged:{
                Setting.mainFont = selectedFont
                CoreData.setCellSize(this)
            }
        }

        TikoSelectFont{
            width: parent.itemWidth
            height: 30
            text: qsTr("桌面滚动歌词字体")
            selectedFont: Setting.deskFont

            onSelectedFontChanged:{
                Setting.deskFont = selectedFont
            }
        }

        TikoSelectFont{
            width: parent.itemWidth
            height: 30
            text: qsTr("主页滚动歌词字体")
            selectedFont: Setting.mainLrcFont

            onSelectedFontChanged:{
                Setting.mainLrcFont = selectedFont
            }
        }
    }
}
