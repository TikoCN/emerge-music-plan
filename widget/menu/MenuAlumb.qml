import QtQuick
import Tiko
import DataCore

TikoMenu {
    id: artistMenu

    property AlumbData alumb

    TikoMenuItem {
        text: qsTr("播放")
    }

    TikoMenuItem {
        text: qsTr("添加到播放队列")
    }

    TikoMenuSpeacer{}

    TikoMenuItem {
        text: qsTr("添加到")
    }

    TikoMenuItem {
        text: qsTr("编辑信息")
    }

    TikoMenuSpeacer{}

    TikoMenuItem {
        text: qsTr("显示专辑")
    }

    TikoMenuItem {
        text: qsTr("显示艺术家")
    }
}
