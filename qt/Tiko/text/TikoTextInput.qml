import QtQuick
import QtQuick.Controls.Basic
import Tiko

TextField{
    property int addFontSize: 0

    id: inputItem
    color: TikoSeit.theme.colorFgDefault
    font.family: TikoSeit.theme.fontDefault.family
    font.pointSize: TikoSeit.theme.fontDefault.pointSize + addFontSize
    placeholderText: qsTr("请输入")
    placeholderTextColor: TikoSeit.theme.colorFgHint

    // 编辑完成 onEditingFinished:
    // 编辑文本 onTextEdited:
}
