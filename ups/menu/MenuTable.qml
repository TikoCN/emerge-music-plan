import QtQuick
import Tiko

TikoMenu{
    id: tableMenu
    TikoMenuItem{
        text: qsTr("打开本地文件夹")
    }
    TikoMenuItem{
        text: qsTr("修改该列表图片")
    }
    TikoMenuItem{
        text: qsTr("重命名")
    }
    TikoMenuItem{
        text: qsTr("拷贝")
    }
}
