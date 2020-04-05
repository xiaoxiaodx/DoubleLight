import QtQuick 2.0
import XVideo 1.0
import Qt.labs.settings 1.0
import QtQuick.Controls 1.4
import "../simpleControl"
Rectangle {

    id:mPlayRect
    signal doubleClick(bool isFullScreen);
    signal click();
    signal s_showToastMsg(string str)
    signal s_sendList(var vmap)

    property bool mIsSelected: false

    border.color: mIsSelected?"#98C5FF":"#252525"
    border.width: 3

    signal s_startTemWarn();
    signal s_endTemWarn();


    property int mouseAdjustWidth1: 4
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


    MouseArea{
        anchors.fill: parent
        //hoverEnabled: true
        propagateComposedEvents:true

        onClicked: {

            click()
            mouse.accepted = false
        }
        onDoubleClicked:doubleClick(true);

    }
    XVideo{
        id:video

        //anchors.fill: parent

        width:(mPlayRect.width*0.75>mPlayRect.height?mPlayRect.height*1.333:mPlayRect.width) -6
        height: (mPlayRect.width*0.75>mPlayRect.height?mPlayRect.height:mPlayRect.width*0.75) -6

        anchors.horizontalCenter: mPlayRect.horizontalCenter
        anchors.verticalCenter: mPlayRect.verticalCenter


        Component.onCompleted:video.startNormalVideo()



        onSignal_loginStatus: main.showToast(msg);

        onSignal_httpUiParSet:httpParCallback(map);


        Rectangle{
            id:rectadmjt
            x:100
            y:100
            width: 200
            height: 200
            color: "#505D9CFF"
            visible:false//videoType === 1
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



    Connections{
        target: deviceconfig
        onS_timeSwith:video.fun_timeSwitch(mchecked);//时间使能
        onS_temSet:video.fun_temSet( mvalue);//警报温度设置
    }
    Connections{
        target: videoTemp
        onS_sendList:video.fun_setListRect(vmap)

    }

    function httpParCallback(smap){
        console.debug("sss");
        console.debug("smap:"+smap)
        var strcmd = smap.cmd;
        console.debug("strcmd:"+strcmd)

        console.debug(" " + strcmd);
        if(strcmd === "getosdparam"){
            var enable = map.enable;

        }else if(strcmd === "getrecordparam"){

        }
    }

    function updateDate(){
        video.fun_updateDate();
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


        video.fun_setRectPar(rectadmjt.x,rectadmjt.y,rectadmjt.width,rectadmjt.height,video.width,video.height)
        console.debug("矩形位置："+rectadmjt.x+" "+rectadmjt.y+" "+rectadmjt.width+" "+rectadmjt.height+" "+video.width+" "+video.height)

    }

}



