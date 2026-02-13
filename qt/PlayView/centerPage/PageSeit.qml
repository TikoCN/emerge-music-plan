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

    readonly property double modeTextWidth: 200
    readonly property real itemWidth: seitPage.width * 0.9
    
    ColumnLayout{
        id: column
        anchors.left: parent.left
        width: parent.width
        anchors.margins: TikoSeit.normalMargins
        spacing: TikoSeit.normalMargins

        //资源文件管理
        SourceSeit {
            Layout.preferredWidth: seitPage.itemWidth
            titleButton.width: seitPage.modeTextWidth
        }

        //颜色管理
        ColorSeit {
            Layout.preferredWidth: seitPage.itemWidth
            titleButton.width: seitPage.modeTextWidth
        }

        //字体管理
        FontSeit {
            Layout.preferredWidth: seitPage.itemWidth
            titleButton.width: seitPage.modeTextWidth
        }

        //参数管理
        ParameterSeit {
            Layout.preferredWidth: seitPage.itemWidth
            titleButton.width: seitPage.modeTextWidth
        }

        //选项管理
        SwitchSeit {
            Layout.preferredWidth: seitPage.itemWidth
            titleButton.width: seitPage.modeTextWidth
        }

        UpdateDataSeit {
            Layout.preferredWidth: seitPage.itemWidth
            titleButton.width: seitPage.modeTextWidth
        }
    }
}
