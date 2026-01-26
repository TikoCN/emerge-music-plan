import QtQuick
import Tiko
import DataType
import MediaerAPI
import PlayView

TikoPopupInput {
    id: albumNameInput
    text: qsTr("请输入新的专辑")
    property bool isCheck: false
    property bool isDClick: false
    property int albumId: -1
    property int newAlbumId: -1
    property var musicList: []

    onFinishInput: {
        newAlbumId = SQLData.checkAlbumName(inputText)
        if (newAlbumId === -2)
            isCheck = true
        isDClick = false
    }
    onInput: isCheck = false

    onAccept: {
        if (!isCheck) {
            const checkMsg = checkMsgCom.createObject(albumNameInput);
            checkMsg.open()
        }
        else if (newAlbumId === -1) {
            album.name = inputText
        }
        else if (isDClick) {
            SQLData.updateAlbumMusic(musicList, newAlbumId, albumId)
        }
        else {
            isDClick = true
            const errorMsg = checkMsgCom.createObject(albumNameInput);
            errorMsg.open()
        }
    }

    Component {
        id: msgCom
        TikoMessageLittle {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
        }
    }

    function sendMsg(){
        if (msgCom.status === Component.Ready){
            const msg = msgCom.createObject(albumNameInput);
            msg.open()
        }
    }
}
