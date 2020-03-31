import QtQuick 2.0
import QtQuick.Layouts 1.12
Rectangle {

    id:root

    property int rowNum: 1
    property int columnNum: 2
    //property alias myModel: repeater.model

    property int currentIndex: -1

    property int mouseAdjustWidth1: 4

    signal s_doubleClick(var index,var ismax);
    signal s_click(var clickIndex);
    signal s_showToastMsg(string str)
    signal s_deleteObject()
    signal s1_authenticationFailue(string str)



    property int    wTOP:1
    property int    wBOTTOM:2
    property int    wLEFT:3
    property int    wRIGHT:4
    property int    wLEFTTOP:5
    property int    wRIGHTTOP:6
    property int    wLEFTBOTTOM:7
    property int    wRIGHTBOTTOM:8
    property int    wCenter:9
    property point mousePressPt1: "0,0"
    color: "#252525"

    //    Repeater{
    //        id:repeater

    RowLayout{
        anchors.fill: parent
        id: rowlayout
        spacing: 0
        VideoLivePlay{
            id:videoNormal
            color: "#252525"
            Layout.fillHeight: true;
            Layout.fillWidth: true
            videoType:1
            mIsSelected: 1 === currentIndex

            onClick: {

                currentIndex = 1

            }

            onDoubleClick: {
                if(videoTemp.visible){
                    videoTemp.visible = false;
                }else{
                    videoTemp.visible = true;
                }
            }

            Rectangle{
                id:rectadmjt
                x:100
                y:100
                width: 200
                height: 200
                color: "#505D9CFF"

                MouseArea{
                    id:areaTop
                    x:mouseAdjustWidth1
                    y:0
                    width: parent.width - mouseAdjustWidth1*2
                    height: mouseAdjustWidth1
                    hoverEnabled: true

                    onEntered: cursorShape = Qt.SizeVerCursor
                    onExited: cursorShape = Qt.ArrowCursor
                    onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                    onPositionChanged: {

                        if(pressed){
                            var offsetX = mouse.x - mousePressPt1.x
                            var offsetY = mouse.y - mousePressPt1.y
                            adjustWindow(wTOP,0,offsetY);
                        }
                    }

                }

                MouseArea{
                    id:areaLeftTop
                    x:0
                    y:0
                    width: mouseAdjustWidth1
                    height: mouseAdjustWidth1
                    hoverEnabled: true

                    onEntered: cursorShape = Qt.SizeFDiagCursor
                    onExited: cursorShape = Qt.ArrowCursor
                    onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                    onPositionChanged: {

                        if(pressed){
                            var offsetX = mouse.x - mousePressPt1.x
                            var offsetY = mouse.y - mousePressPt1.y
                            adjustWindow(wLEFTTOP,offsetX,offsetY);
                        }
                    }

                }

                MouseArea{
                    id:areaRightTop
                    x:parent.width - mouseAdjustWidth1
                    y:0
                    width: mouseAdjustWidth1
                    height: mouseAdjustWidth1
                    hoverEnabled: true

                    onEntered: cursorShape = Qt.SizeBDiagCursor
                    onExited: cursorShape = Qt.ArrowCursor
                    onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                    onPositionChanged: {

                        if(pressed){
                            var offsetX = mouse.x - mousePressPt1.x
                            var offsetY = mouse.y - mousePressPt1.y
                            adjustWindow(wRIGHTTOP,offsetX,offsetY);
                        }
                    }

                }

                MouseArea{
                    id:areaLeft
                    x:0
                    y:mouseAdjustWidth1
                    width: mouseAdjustWidth1
                    height: parent.height - mouseAdjustWidth1
                    hoverEnabled: true

                    onEntered: cursorShape = Qt.SizeHorCursor
                    onExited: cursorShape = Qt.ArrowCursor
                    onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                    onPositionChanged: {

                        if(pressed){
                            var offsetX = mouse.x - mousePressPt1.x
                            var offsetY = mouse.y - mousePressPt1.y
                            adjustWindow(wLEFT,offsetX,0);
                        }
                    }

                }

                MouseArea{
                    id:areaRight
                    x:parent.width - mouseAdjustWidth1
                    y:0
                    width: mouseAdjustWidth1
                    height: parent.height - mouseAdjustWidth1
                    hoverEnabled: true
                    onEntered: cursorShape = Qt.SizeHorCursor
                    onExited: cursorShape = Qt.ArrowCursor
                    onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                    onPositionChanged: {


                        if(pressed){
                            var offsetX = mouse.x + mousePressPt1.x
                            var offsetY = mouse.y + mousePressPt1.y
                            adjustWindow(wRIGHT,offsetX,0);
                        }

                    }
                }

                MouseArea{
                    id:areaLeftBottom
                    x:0
                    y:parent.height - mouseAdjustWidth1
                    width: mouseAdjustWidth1
                    height: mouseAdjustWidth1
                    hoverEnabled: true

                    onEntered: cursorShape = Qt.SizeBDiagCursor
                    onExited: cursorShape = Qt.ArrowCursor
                    onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                    onPositionChanged: {

                        if(pressed){
                            var offsetX = mouse.x - mousePressPt1.x
                            var offsetY = mouse.y - mousePressPt1.y
                            adjustWindow(wLEFTBOTTOM,offsetX,offsetY);
                        }
                        //setDlgPoint(offset.x, 0)

                    }
                }

                MouseArea{
                    id:areaRightBottom
                    x:rectadmjt.width - mouseAdjustWidth1
                    y:rectadmjt.height - mouseAdjustWidth1
                    width: mouseAdjustWidth1
                    height: mouseAdjustWidth1
                    hoverEnabled: true

                    onEntered: cursorShape = Qt.SizeFDiagCursor
                    onExited: cursorShape = Qt.ArrowCursor
                    onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                    onPositionChanged: {

                        if(pressed){
                            var offsetX = mouse.x - mousePressPt1.x
                            var offsetY = mouse.y - mousePressPt1.y
                            adjustWindow(wRIGHTBOTTOM,offsetX,offsetY);
                        }
                        //setDlgPoint(offset.x, 0)
                    }
                }

                MouseArea{
                    id:areaBottom
                    x:mouseAdjustWidth1
                    y:parent.height - mouseAdjustWidth1
                    width: parent.width - mouseAdjustWidth1*2
                    height: mouseAdjustWidth1
                    hoverEnabled: true

                    onEntered: cursorShape = Qt.SizeVerCursor
                    onExited: cursorShape = Qt.ArrowCursor
                    onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                    onPositionChanged: {

                        if(pressed){
                            var offsetX = mouse.x - mousePressPt1.x
                            var offsetY = mouse.y - mousePressPt1.y
                            adjustWindow(wBOTTOM,0,offsetY);
                        }
                        //setDlgPoint(offset.x, 0)

                    }
                }

                MouseArea{
                    id:areaCenter
                    x:mouseAdjustWidth1
                    y:mouseAdjustWidth1
                    width: parent.width - mouseAdjustWidth1*2
                    height: parent.height - mouseAdjustWidth1*2
                    hoverEnabled: true
                    onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                    onPositionChanged: {

                        if(pressed){
                            var offsetX = mouse.x - mousePressPt1.x
                            var offsetY = mouse.y - mousePressPt1.y
                            adjustWindow(wCenter,offsetX,offsetY);
                        }
                        //setDlgPoint(offset.x, 0)

                    }
                }
            }
        }

        VideoLivePlay{
            id:videoTemp
            videoType:2
            Layout.fillHeight: true;
            Layout.fillWidth: true
            color: "#252525"
            mIsSelected: 2 === currentIndex
            onClick: {
                currentIndex = 2
            }

            onDoubleClick: {

                if(videoNormal.visible){
                    videoNormal.visible = false;
                }else{
                    videoNormal.visible = true;
                }
            }




        }
    }


    function adjustWindow(adjustw,dX,dY)
    {
        var dx = 0;
        var dy = 0;
        var dw = 0;
        var dh = 0;
        if(adjustw === wLEFT){
            rectadmjt.x = rectadmjt.x + dX;
            rectadmjt.width = rectadmjt.width - dX;
        }else if(adjustw === wRIGHT){

            rectadmjt.width = rectadmjt.width + dX;

        }else if(adjustw === wTOP){
            rectadmjt.y = rectadmjt.y + dY;
            rectadmjt.height = rectadmjt.height - dY;

        }else if(adjustw === wBOTTOM){
            console.debug( "*****   " + dY)
            rectadmjt.height = rectadmjt.height + dY;
        }else if(adjustw === wRIGHTTOP){

            rectadmjt.x = rectadmjt.x - dX;
            rectadmjt.width = rectadmjt.width + dX;

            rectadmjt.y = rectadmjt.y + dY;
            rectadmjt.height = rectadmjt.height - dY;

        }else if(adjustw === wLEFTTOP){

            rectadmjt.x = rectadmjt.x + dX;
            rectadmjt.width = rectadmjt.width - dX;

            rectadmjt.y = rectadmjt.y + dY;
            rectadmjt.height = rectadmjt.height - dY;

        }else if(adjustw === wLEFTBOTTOM){
            rectadmjt.x = rectadmjt.x + dX;
            rectadmjt.width = rectadmjt.width - dX;

            rectadmjt.height = rectadmjt.height + dY;

        }else if(adjustw === wRIGHTBOTTOM){

            rectadmjt.width = rectadmjt.width + dX;

            rectadmjt.height = rectadmjt.height + dY;
        }else if(wCenter === adjustw){

            rectadmjt.x = rectadmjt.x + dX;
            rectadmjt.y = rectadmjt.y + dY;
        }









//                if(adjustw === wLEFT){
//                    dx =  dX;
//                    dw = - dX;
//                }else if(adjustw === wRIGHT){

//                    dw =  dX;

//                }else if(adjustw === wTOP){

//                }else if(adjustw === wBOTTOM){
//                    dh = dY;
//                }else if(adjustw === wRIGHTTOP){

//                }else if(adjustw === wLEFTTOP){

//                }else if(adjustw === wLEFTBOTTOM){
//                    dx =  dX;
//                    dw =  - dX;
//                    dh =  dY;
//                }else if(adjustw === wRIGHTBOTTOM){

//                    dw = dX;
//                    dh = dY;
//                }

//                if(rectadmjt.width + dw <= minW)
//                    rectadmjt.width = minW;
//                else{
//                    rectadmjt.width = rectadmjt.width + dw;
//                    rectadmjt.x = rectadmjt.x + dx;
//                }

//                if(rectadmjt.height + dh <= minH)
//                    rectadmjt.height = minH
//                else{
//                    rectadmjt.height = rectadmjt.height + dh;
//                    rectadmjt.y = rectadmjt.y + dy;
//                }









    }

}
