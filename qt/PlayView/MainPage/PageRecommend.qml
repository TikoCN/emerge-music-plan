import QtQuick.Controls
import QtQuick
import PlayView
import Tiko
import MediaerAPI

ScrollView {
    id: mainPage
    contentHeight: Math.max(availableHeight, showColumn.implicitHeight + 120)
    ScrollBar.horizontal.visible: false
    ScrollBar.vertical: TikoBarV {
    }

    property bool hasRecommendations: recomMusic.visible
                                        || recomAlbum.visible
                                        || recomMusicNew.visible
                                        || recomArtist.visible
                                        || recomMusicPlay.visible

    Column {
        id: showColumn
        width: mainPage.width - 80
        spacing: 20
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 20

        RecomGridButtonMusic {
            id: recomMusic
            width: showColumn.width
            height: childrenRect.height
        }

        // 随机推荐专辑
        RecomGridButtonAlbum {
            id: recomAlbum
            width: showColumn.width
            height: childrenRect.height
        }

        RecomMusicNew {
            id: recomMusicNew
            width: showColumn.width
            height: childrenRect.height
        }

        RecomGridButtonArtist {
            id: recomArtist
            width: showColumn.width
            height: childrenRect.height
        }

        RecomMusicPlay {
            id: recomMusicPlay
            width: showColumn.width
            height: childrenRect.height
        }
    }

    Column {
        id: emptyState
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -mainPage.height * 0.1
        spacing: TikoSeit.emphasizeMargins
        visible: !mainPage.hasRecommendations

        TikoButtonIcon {
            anchors.horizontalCenter: parent.horizontalCenter
            icon.source: "qrc:/image/recommend.svg"
            cell: mainPage.width * 0.2
            imgCell: cell
            enabled: false
        }

        Text {
            width: mainPage.width * 0.35
            height: mainPage.height * 0.08
            text: qsTr("请设置数据来源")
            color: TikoSeit.theme.colorTextDefault
            font.pixelSize: Math.max(18, mainPage.width * 0.025)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
