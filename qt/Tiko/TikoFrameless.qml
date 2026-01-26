import QtQuick

Window {
    id: win

    property int acttion: 0
    property bool sizeChange: false

    color: "#00000000"
    flags: Qt.Window | Qt.FramelessWindowHint // 尝试隐藏边框和标题栏

    MouseArea {
        id: dropSize

        function resizeWindow(mouseX, mouseY) {
            const marginSpace = 10;

            const marginRight = width - mouseX;
            const marginBottom = height - mouseY;
            win.acttion = 0;
            if (marginRight < marginSpace) {//右
                win.acttion |= Qt.RightEdge;
            }
            if (marginBottom < marginSpace) {//下
                win.acttion |= Qt.BottomEdge;
            }
            if (mouseX < marginSpace) {//左
                win.acttion |= Qt.LeftEdge;
            }
            if (mouseY < marginSpace) {//上
                win.acttion |= Qt.TopEdge;
            }
        }
        function setCursorShape() {
            if (win.acttion === Qt.LeftEdge || win.acttion === Qt.RightEdge) {
                return Qt.SizeHorCursor;
            } else if (win.acttion === Qt.TopEdge || win.acttion === Qt.BottomEdge) {
                return Qt.SizeVerCursor;
            } else if (win.acttion === (Qt.LeftEdge | Qt.TopEdge) || win.acttion === (Qt.RightEdge | Qt.BottomEdge)) {
                return Qt.SizeFDiagCursor;
            } else if (win.acttion === (Qt.RightEdge | Qt.TopEdge) || win.acttion === (Qt.LeftEdge | Qt.BottomEdge)) {
                return Qt.SizeBDiagCursor;
            } else {
                return Qt.ArrowCursor;
            }
        }

        anchors.fill: parent
        cursorShape: dropSize.setCursorShape()
        hoverEnabled: true

        onPositionChanged: {
            resizeWindow(mouseX, mouseY);
        }
        onPressed: {
            resizeWindow(mouseX, mouseY);
            if (win.acttion !== 0) {
                win.startSystemResize(win.acttion);
                win.sizeChange = true;
            } else {
                win.startSystemMove();
            }
        }
        onReleased: {
            win.acttion = 0;
            win.sizeChange = false;
        }
    }
}
