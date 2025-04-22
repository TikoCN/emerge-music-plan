import QtQuick
import Tiko
import DataType
import MediaerAPI

TikoPopupInput {
    id: albumNameInput
    text: qsTr("请输入新的作曲家")
    property bool isCheck: false
    property bool isDClick: false
    property int albumId: -1
    property int newAlbumId: -1
    property AlbumData album: Core.getAlbum(albumId)
    onFinish: {
        newAlbumId = SQLData.checkAlbumName(name)
        if (newAlbumId === -2)
            isCheck = true
        isDClick = false
    }
    onInput: isCheck = false
    onAccept: {
        if (!isCheck) {
            var checkMsg = checkMsgCom.createObject(albumNameInput)
            checkMsg.open()
        }
        else if (newAlbumId === -1) {
            album.name = inputText
        }
        else if (isDClick) {
            SQLData.updateAlbumMusic(album.musicList, newAlbumId, albumId)
        }
        else {
            isDClick = true
            var errorMsg = checkMsgCom.createObject(albumNameInput)
            errorMsg.open()
        }
    }
    onCancel: close()
    onClosed: {
        Core.releaseAlbum(AlbumId)
        destroy()
    }

    Component {
        id: errorMsgCom
        TikoMessageLittle {
            id: errorMsg
            message: qsTr("该作曲家已存在，再次点击将歌曲迁移至目标作曲家")
            anchors.centerIn: AlbumNameInput
        }
    }

    Component {
        id: checkMsgCom
        TikoMessageLittle {
            id: checkMsg
            message: qsTr("数据库错误，未能检验文本")
            anchors.centerIn: AlbumNameInput
        }
    }
}
