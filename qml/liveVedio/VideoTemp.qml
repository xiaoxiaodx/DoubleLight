import QtQuick 2.0
import XVideoTemp 1.0
import QtQuick 2.0
import Qt.labs.settings 1.0
import QtQuick.Controls 1.4
import "../simpleControl"
Rectangle {

    id:mPlayRect
    signal doubleClick(bool isFullScreen);
    signal click();
    signal s_showToastMsg(string str)
    signal s_sendList(var vmap)
    signal s_dxdy(var dx,var dy)

    property bool mIsSelected: false
    border.color: mIsSelected?"#98C5FF":"#252525"
    border.width: 3


    MouseArea{
        anchors.fill: parent
        //hoverEnabled: true
        propagateComposedEvents:true
        onClicked: {
            click()
            // mouse.accepted = false
        }
        onDoubleClicked:doubleClick(true);

    }

    Text {
        id: pos1
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 50
        anchors.bottom:parent.bottom
        color: "red"
        text: qsTr("text")
    }



        LineEdit {
            id: inputdx
            width: 100
            height: 20
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 10
            anchors.bottom:parent.bottom
            border.width: 0
            inputLimite:Qt.ImhDigitsOnly
            placeholderText: ""
            isNeedDoubleClickEdit: false
            textLeftPadding:0
            txtColor: Qt.rgba(0,0,0,0.65)
            text: "0"
            color: "#F8FAFD"
            onTextChanged: {

            }
        }

        LineEdit {
            id: inputdy
            width: 100
            height: 20

            anchors.left: inputdx.right
            anchors.leftMargin: 5
            anchors.verticalCenter: inputdx.verticalCenter
            border.width: 0
            inputLimite:Qt.ImhDigitsOnly
            placeholderText: ""
            isNeedDoubleClickEdit: false
            textLeftPadding:0
            txtColor: Qt.rgba(0,0,0,0.65)
            text: "0"
            color: "#F8FAFD"
            onTextChanged: {

            }
        }

        Button{
            id:btnEnsure
            width: 50
            height: 20
            anchors.left: inputdy.right
            anchors.leftMargin: 5
            anchors.verticalCenter: inputdx.verticalCenter
            text: "确认"
            onClicked: {
                var kx = 1;
                var ky = 1;
                var x1 = inputdx.text / kx;
                var y1 = inputdy.text / ky;

                console.debug()
                s_dxdy(x1,y1)

            }

        }

    XVideoTemp{
        id:video

        property real whradia: /*0.5625*/ 0.75
        property real hwradia: /*1.778 */ 1.333


        width:(mPlayRect.width*whradia>mPlayRect.height?mPlayRect.height*hwradia:mPlayRect.width) -6
        height: (mPlayRect.width*whradia>mPlayRect.height?mPlayRect.height:mPlayRect.width*whradia) -6
        // width: 206
        // height: 156
        anchors.horizontalCenter: mPlayRect.horizontalCenter
        anchors.verticalCenter: mPlayRect.verticalCenter
        //Component.onCompleted:video.startTemperatureVideo(deviceconfig.getWarnTem(),"D04");
        onSignal_loginStatus: main.showToast(msg);
        onSignal_areaMaxtemp:tempParCallback(map);
        onSignal_sendListRect:videoNormal.funsetlistRect(map)
        onSignal_initRedFrame:videoNormal.funinitRedFrame(mw,mh)

                MouseArea{
                    id:mouse22
                    anchors.fill: parent
                    cursorShape: Qt.CrossCursor
                    onClicked: {
                        var kx = video.width / 206;
                        var ky = video.height / 156;
                        var x1 = mouse.x / kx;
                        var y1 = mouse.y / ky;
                        pos1.text ="pos:"+ x1 +"    "+y1
                    }
                }
    }



    Connections{
        target: videoNormal
        onS_tempmodelSelect:{

            video.startTemperatureVideo(deviceconfig.getWarnTem(),mtype,deviceconfig.getTcpip())

        }
        onS_testRect: video.fun_recTestRect(x0,y0,w0,h0,x1,y1,w1,h1,x2,y2,w2,h2);

    }

    Connections{
        target: deviceconfig
        onS_timeSwith:video.fun_timeSwitch(mchecked);
        onS_temSet:video.fun_temSet( mvalue);
        onS_temMax:video.fun_temMax(mvalue);
        onS_temMin:video.fun_temMin(mvalue);
        onS_temOffset:video.fun_temOffset(mvalue);
        onS_temImage:video.fun_colorShowType(mvalue);
    }

    function httpParCallback(smap){

        console.debug("smap:"+smap)
        var strcmd = smap.cmd;
        console.debug("strcmd:"+strcmd)

        console.debug(" " + strcmd);
        if(strcmd === "getosdparam"){
            var enable = map.enable;

        }else if(strcmd === "getrecordparam"){

        }
    }
    function tempParCallback(smap){

        var cmdType = smap.parType;

        if(cmdType === "temp"){
            var num = parseFloat(deviceconfig.getWarnTem()).toFixed(2)
            var tempV = smap.tempValue;
            if( Number(tempV) >= Number(num)){
                vedioLayout.startWarn(tempV);
            }else{
                //vedioLayout.endWarn();
            }
        }else if("getiradrect" === cmdType){
            video.fun_recTestRect(smap.x,smap.y,smap.w,smap.h);
        }
    }


    function updateDate(){
        video.fun_updateDate();
    }

}



