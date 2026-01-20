import QtQuick
import Tiko
import DataType
import MediaerAPI

TikoPopupInput {
    id: playlistNameInput
    text: qsTr("请输入新的播放列表")
    property bool isCheck: false
    property bool isDClick: false
    property int playlistId: -1
    property int newPlayListId: -1

    onFinishInput: {
        newPlayListId = SQLData.checkPlayListName(inputText)
        if (newPlayListId === -2)
            isCheck = true
        isDClick = false
    }
    onInput: isCheck = false
    onAccept: {
        if (!isCheck) {
            var checkMsg = checkMsgCom.createObject(playlistNameInput)
            checkMsg.open()
        }
        else if (newPlayListId === -1) {
            playlist.name = inputText
        }
        else if (isDClick) {
            SQLData.updatePlayListMusic(playlist.musicList, newPlayListId, playlistId)
        }
        else {
            isDClick = true
            var errorMsg = checkMsgCom.createObject(playlistNameInput)
            errorMsg.open()
        }
    }
    onClosed: {
        DataActive.decrementPlayList(playlistId)
        destroy()
    }

    Component {
        id: errorMsgCom
        TikoMessageLittle {
            id: errorMsg
            message: qsTr("该播放列表已存在，再次点击将歌曲迁移至目标播放列表")
            anchors.centerIn: playlistNameInput
        }
    }

    Component {
        id: checkMsgCom
        TikoMessageLittle {
            id: checkMsg
            message: qsTr("数据库错误，未能检验文本")
            anchors.centerIn: playlistNameInput
        }
    }
}

