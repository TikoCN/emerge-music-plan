import QtQuick
import Tiko
import DataType
import MediaerAPI

TikoPopupInput {
    id: artistNameInput
    text: qsTr("请输入新的作曲家")
    property bool isCheck: false
    property bool isDClick: false
    property int artistId: -1
    property int newArtistId: -1
    property var musicList: []

    onFinishInput: {
        newArtistId = SQLData.checkArtistName(inputText)
        if (newArtistId === -2)
            isCheck = true
        isDClick = false
    }
    onInput: isCheck = false
    onAccept: {
        if (!isCheck) {
            var checkMsg = checkMsgCom.createObject(artistNameInput)
            checkMsg.open()
        }
        else if (newArtistId === -1) {
            // todo 更新名字
        }
        else if (isDClick) {
            SQLData.updateArtistMusic(musicList, newArtistId, artistId)
        }
        else {
            isDClick = true
            var errorMsg = checkMsgCom.createObject(artistNameInput)
            errorMsg.open()
        }
    }
    onClosed: {
        Core.decrementArtist(artistId)
        destroy()
    }

    Component {
        id: errorMsgCom
        TikoMessageLittle {
            id: errorMsg
            message: qsTr("该作曲家已存在，再次点击将歌曲迁移至目标作曲家")
            anchors.centerIn: artistNameInput
        }
    }

    Component {
        id: checkMsgCom
        TikoMessageLittle {
            id: checkMsg
            message: qsTr("数据库错误，未能检验文本")
            anchors.centerIn: artistNameInput
        }
    }
}

