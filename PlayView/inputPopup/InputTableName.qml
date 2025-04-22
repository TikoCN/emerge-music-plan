import QtQuick
import Tiko
import DataType
import MediaerAPI

TikoPopupInput {
    id: tableNameInput
    text: qsTr("请输入新的作曲家")
    property bool isCheck: false
    property bool isDClick: false
    property int tableId: -1
    property int newTableId: -1
    property TableData table: Core.getTable(tableId)
    onFinish: {
        newTableId = SQLData.checkTableName(name)
        if (newTableId === -2)
            isCheck = true
        isDClick = false
    }
    onInput: isCheck = false
    onAccept: {
        if (!isCheck) {
            var checkMsg = checkMsgCom.createObject(tableNameInput)
            checkMsg.open()
        }
        else if (newTableId === -1) {
            table.name = inputText
        }
        else if (isDClick) {
            SQLData.updateTableMusic(table.musicList, newTableId, tableId)
        }
        else {
            isDClick = true
            var errorMsg = checkMsgCom.createObject(tableNameInput)
            errorMsg.open()
        }
    }
    onCancel: close()
    onClosed: {
        Core.releaseTable(tableId)
        destroy()
    }

    Component {
        id: errorMsgCom
        TikoMessageLittle {
            id: errorMsg
            message: qsTr("该作曲家已存在，再次点击将歌曲迁移至目标作曲家")
            anchors.centerIn: tableNameInput
        }
    }

    Component {
        id: checkMsgCom
        TikoMessageLittle {
            id: checkMsg
            message: qsTr("数据库错误，未能检验文本")
            anchors.centerIn: tableNameInput
        }
    }
}

