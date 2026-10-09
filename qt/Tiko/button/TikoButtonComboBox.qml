import QtQuick
import Tiko

TikoButtonDefault {
    id: tikoComboxButton
    textLine.text: tikoComboxButton.helpText + tikoComboxButton.currentText
    onClicked: tikoPopup.open()
    width: 100
    height: 50

    // 属性定义
    property var data: ["文本1", "文本2"]  // 下拉选项数据
    property int currentIndex: 0                  // 当前选中项的索引
    property string helpText: ""          // 帮助文本（前缀）
    property string currentText: tikoComboxButton.data[currentIndex]  // 当前显示文本

    // 下拉弹出框
    TikoPopup {
        id: tikoPopup
        width: tikoComboxButton.width
        height: Math.min(200, listModel.count * 40)  // 动态高度
        radius: 10
        padding: 0
        clip: true
        y: tikoComboxButton.height

        // 选项列表
        ListView {
            id: column
            spacing: 0
            anchors.fill: parent
            boundsBehavior: Flickable.StopAtBounds

            model: ListModel {
                id: listModel
            }

            delegate: TikoButtonDefault {
                textLine.text: tikoComboxButton.data[index]
                width: tikoComboxButton.width
                height: 40

                onClicked: {
                    tikoComboxButton.currentIndex = index
                    tikoPopup.close()
                }
            }
        }

        // 组件加载完成后初始化数据
        Component.onCompleted: {
            updateModel()
        }
    }

    // 当data属性变化时更新模型
    onDataChanged: {
        updateModel()
        if (currentIndex >= data.length) {
            currentIndex = Math.max(0, data.length - 1)
        }
    }

    // 当show属性变化时更新显示文本
    onCurrentIndexChanged: {
        if (currentIndex >= 0 && currentIndex < data.length) {
            currentText = data[currentIndex]
        }
        if (column.currentIndex !== currentIndex) {
            column.currentIndex = currentIndex
        }
    }

    // 更新列表模型的函数
    function updateModel() {
        listModel.clear()
        if (data && data.length > 0) {
            for(let i = 0; i < data.length; i++) {
                listModel.append({index: i})
            }
        }
    }
}
