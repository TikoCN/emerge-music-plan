import QtQuick
import Tiko

TikoButtonNormal {
    id: tikoComboxButton
    text: tikoComboxButton.helpText + tikoComboxButton.showText
    onClick: tikoPopup.open()

    // 属性定义
    property var data: ["文本1", "文本2"]  // 下拉选项数据
    property int show: 0                  // 当前选中项的索引
    property string helpText: ""          // 帮助文本（前缀）
    property string showText: tikoComboxButton.data[show]  // 当前显示文本

    // 背景装饰
    Rectangle {
        anchors.fill: parent
        color: TikoSeit.backdropColor
        border.color: TikoSeit.transparentColor
        border.width: 3
        radius: 10
        z: -1  // 确保背景在按钮下方
    }

    // 下拉弹出框
    TikoPopup {
        id: tikoPopup
        width: tikoComboxButton.width
        height: Math.min(200, listModel.count * 40)  // 动态高度
        padding: 0
        clip: true

        // 选项列表
        ListView {
            id: column
            spacing: 0
            anchors.fill: parent
            boundsBehavior: Flickable.StopAtBounds

            model: ListModel {
                id: listModel
            }

            delegate: TikoButtonNormal {
                radius: 0
                text: tikoComboxButton.data[index]
                width: tikoComboxButton.width
                height: 40

                // 添加选中状态指示
                Rectangle {
                    visible: index === tikoComboxButton.show
                    anchors.fill: parent
                    color: TikoSeit.textInformationColor
                    opacity: 0.3
                    z: -1
                }

                onClick: {
                    tikoComboxButton.show = index
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
        // 确保show索引在有效范围内
        if (show >= data.length) {
            show = Math.max(0, data.length - 1)
        }
    }

    // 当show属性变化时更新显示文本
    onShowChanged: {
        // 确保索引有效
        if (show >= 0 && show < data.length) {
            showText = data[show]
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
