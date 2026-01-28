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
        x: 10
        y: 10
        width: parent.width - 20
        spacing: 10

        //资源文件管理
        SourceSeit {
            show.width: seitPage.modeTextWidth
        }

        //颜色管理
        ColorSeit {
            show.width: seitPage.modeTextWidth
        }

        //字体管理
        FontSeit {
            show.width: seitPage.modeTextWidth
        }

        //参数管理
        ParameterSeit {
            show.width: seitPage.modeTextWidth
        }

        //选项管理
        SwitchSeit {
            show.width: seitPage.modeTextWidth
        }

        UpdateDataSeit {
            show.width: seitPage.modeTextWidth
        }
    }
}
