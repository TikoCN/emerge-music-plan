import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import MediaerAPI
import Tiko
import PlayView

ScrollView{
    id: seitPage
    ScrollBar.horizontal.visible: false
    ScrollBar.vertical: TikoBarV{}

    property double modeTextWidth: 150
    
    ColumnLayout{
        id: column
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: TikoSeit.normalMargins
        spacing: TikoSeit.normalMargins

        //资源文件管理
        SourceSeit {
            width: parent.width * 0.75
            titleButton.width: seitPage.modeTextWidth
        }

        //颜色管理
        ColorSeit {
            width: parent.width * 0.75
            titleButton.width: seitPage.modeTextWidth
        }

        //字体管理
        FontSeit {
            width: parent.width * 0.75
            titleButton.width: seitPage.modeTextWidth
        }

        //参数管理
        ParameterSeit {
            width: parent.width * 0.75
            titleButton.width: seitPage.modeTextWidth
        }

        //选项管理
        SwitchSeit {
            width: parent.width * 0.75
            titleButton.width: seitPage.modeTextWidth
        }

        UpdateDataSeit {
            width: parent.width * 0.75
            titleButton.width: seitPage.modeTextWidth
        }
    }
}
