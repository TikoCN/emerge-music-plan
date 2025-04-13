import QtQuick
Window{
    id:win
    color:"#00000000"
    flags: Qt.Window | Qt.FramelessWindowHint // 尝试隐藏边框和标题栏
    property int acttion : 0
    property bool sizeChange: false

    MouseArea{
        id:dropSize
        anchors.fill:parent
        hoverEnabled:true

        cursorShape:setCursorShape()

        function setCursorShape(){
            if(acttion === Qt.LeftEdge || acttion === Qt.RightEdge){
                return Qt.SizeHorCursor
            }
            else if(acttion === Qt.TopEdge || acttion === Qt.BottomEdge){
                return Qt.SizeVerCursor
            }
            else if(acttion === (Qt.LeftEdge | Qt.TopEdge) || acttion === (Qt.RightEdge | Qt.BottomEdge)){
                return Qt.SizeFDiagCursor
            }
            else if(acttion === (Qt.RightEdge | Qt.TopEdge) || acttion === (Qt.LeftEdge | Qt.BottomEdge)){
                return Qt.SizeBDiagCursor
            }
            else{
                return Qt.ArrowCursor
            }
        }

        onPositionChanged:{
            resizeWindow(mouseX, mouseY)
        }

        onPressed:{
            resizeWindow(mouseX, mouseY)
            if(acttion != 0){
                win.startSystemResize(acttion)
                sizeChange = true
            }
            else{
                win.startSystemMove()
            }
        }

        function resizeWindow(mouseX, mouseY){
            var marginSpace = 10

            var marginRight = width - mouseX
            var marginBottom = height - mouseY
            acttion = 0
            if(marginRight < marginSpace){//右
                acttion |= Qt.RightEdge
            }
            if(marginBottom < marginSpace){//下
                acttion |= Qt.BottomEdge
            }
            if(mouseX < marginSpace){//左
                acttion |= Qt.LeftEdge
            }
            if(mouseY < marginSpace){//上
                acttion |= Qt.TopEdge
            }
        }

        onReleased:{
            acttion = 0
            sizeChange = false
        }
    }
}
