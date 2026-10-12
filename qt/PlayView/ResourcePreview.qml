import QtQuick
import QtQuick.Window
import QtQuick.Controls.Basic as Basic
import Tiko
import MediaerAPI

TikoFrameless {
    id: resourcePreview
    width: 900
    height: 600
    minimumWidth: 640
    minimumHeight: 420
    visible: false
    title: qsTr("资源预览")
    flags: Qt.Window | Qt.FramelessWindowHint

    property string oldResourceUrl: ""
    property string newResourceData: ""
    property string filePath: ""
    property bool isImage: true
    signal dismissed()

    function closePreview() {
        dismissed()
        destroy()
    }

    onClosing: {
        dismissed()
        Qt.callLater(function() { resourcePreview.destroy() })
    }

    Item {
        id: windowSurface
        anchors.fill: parent
        anchors.margins: 10

        Rectangle {
            anchors.fill: parent
            color: TikoSeit.theme.colorBgView
            radius: 10
        }

        TikoWindow {
            id: titleBar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            title: qsTr("资源预览")
            windowTarget: resourcePreview
            onCloseRequested: resourcePreview.closePreview()
        }

        Column {
            anchors.top: titleBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: TikoSeit.emphasizeMargins
            spacing: 12

            Row {
                width: parent.width
                height: parent.height - 62
                spacing: 16

                Rectangle {
                    width: (parent.width - 16) / 2
                    height: parent.height
                    color: TikoSeit.theme.colorMaxBottom
                    radius: 6
                    border.color: TikoSeit.theme.colorBgHint
                    border.width: 1

                    Column {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 8

                        TikoTextLine { text: qsTr("当前资源") }

                        Image {
                            visible: resourcePreview.isImage
                            width: parent.width
                            height: parent.height - 32
                            source: resourcePreview.isImage ? resourcePreview.oldResourceUrl : ""
                            fillMode: Image.PreserveAspectFit
                        }

                        Basic.ScrollView {
                            visible: !resourcePreview.isImage
                            width: parent.width
                            height: parent.height - 32

                            Basic.TextArea {
                                text: resourcePreview.oldResourceUrl
                                readOnly: true
                                wrapMode: Basic.TextArea.Wrap
                                color: TikoSeit.theme.colorFgDefault
                                background: Rectangle { color: TikoSeit.theme.colorMaxBottom; radius: 6 }
                            }
                        }
                    }
                }

                Rectangle {
                    width: (parent.width - 16) / 2
                    height: parent.height
                    color: TikoSeit.theme.colorMaxBottom
                    radius: 6
                    border.color: TikoSeit.theme.colorBgHint
                    border.width: 1

                    Column {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 8

                        TikoTextLine { text: qsTr("网络资源") }

                        Image {
                            visible: resourcePreview.isImage
                            width: parent.width
                            height: parent.height - 32
                            source: resourcePreview.isImage
                                    ? "data:image/jpeg;base64," + resourcePreview.newResourceData : ""
                            fillMode: Image.PreserveAspectFit
                        }

                        Basic.ScrollView {
                            visible: !resourcePreview.isImage
                            width: parent.width
                            height: parent.height - 32

                            Basic.TextArea {
                                text: resourcePreview.newResourceData
                                readOnly: true
                                wrapMode: Basic.TextArea.Wrap
                                color: TikoSeit.theme.colorFgDefault
                                background: Rectangle { color: TikoSeit.theme.colorMaxBottom; radius: 6 }
                            }
                        }
                    }
                }
            }

            Row {
                width: parent.width
                height: 50
                spacing: 10
                layoutDirection: Qt.RightToLeft

                TikoButtonDefault {
                    textLine.text: qsTr("确定")
                    onLeftClicked: {
                        OnLine.savePreviewResource(resourcePreview.filePath,
                                                   resourcePreview.newResourceData,
                                                   resourcePreview.isImage)
                        resourcePreview.closePreview()
                    }
                }

                TikoButtonDefault {
                    textLine.text: qsTr("取消")
                    onLeftClicked: resourcePreview.closePreview()
                }
            }
        }
    }
}
