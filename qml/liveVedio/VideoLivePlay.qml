import QtQuick 2.0
import XVideo 1.0
import QtQuick.Controls 1.4
import "../simpleControl"
Rectangle {

    id:mPlayRect
    signal doubleClick(bool isFullScreen);
    signal click();
    signal s_showToastMsg(string str)


    property string shotScrennFilePath: ""
    property string recordingFilePath: ""
    property bool mIsSelected: false
    property int videoType : 0
    border.color: mIsSelected?"#EF9D36":"#252525"
    border.width: 2

    signal s_startTemWarn();
    signal s_endTemWarn();


    XVideo{
        id:video

        //anchors.fill: parent

        width:(mPlayRect.width*3/4>mPlayRect.height?mPlayRect.height*4/3:mPlayRect.width) - 4
        height: (mPlayRect.width*3/4>mPlayRect.height?mPlayRect.height:mPlayRect.width*3/4) - 4

        anchors.horizontalCenter: mPlayRect.horizontalCenter
        anchors.verticalCenter: mPlayRect.verticalCenter


        Component.onCompleted: {

            if(videoType === 1){
                video.startNormalVideo();

            }else if(videoType === 2){
                video.startTemperatureVideo();
            }
        }

        onSignal_loginStatus: main.showToast(msg);

//        onSignal_temp:{
//            var num = parseFloat(deviceconfig.getWarnTem()).toFixed(2)
//            //console.debug("temp ***** "+tempV+"   "+num)
//            if( Number(tempV) >= Number(num)){
//                vedioLayout.startWarn();
//            }else{
//                vedioLayout.endWarn();
//            }
//        }
        onSignal_tempPar:tempParCallback(map);

        onSignal_httpUiParSet:httpParCallback(map);


    }

    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents:true

        onClicked: click();

        onDoubleClicked:doubleClick(true);

    }
    Connections{
        target: deviceconfig
        onS_timeSwith:video.fun_timeSwitch(mchecked);
        onS_warnSwith:video.fun_warnSwith(mchecked);
        onS_screenShotSwith:video.fun_warnSwith(mchecked);
        onS_beerSwith:video.fun_beerSwith(mchecked);
        onS_recordSwith:video.fun_recordSwith(mchecked);
        onS_temSet:video.fun_temSet( mvalue);
        onS_screenShotPathSet:video.fun_screenShotPathSet( mvalue);
        onS_recordPathSet:video.fun_recordPathSet(mvalue);
        onS_temDrift:video.fun_temDrift(mvalue);
        //onS_getInitPar:video.fun_getInitPar();

        onS_temMax:video.fun_temMax(mvalue);
        onS_temMin:video.fun_temMin(mvalue);
        onS_temOffset:video.fun_temOffset(mvalue);
    }

    function httpParCallback(smap){
        console.debug("sss");
        console.debug("smap:"+smap)
        var strcmd = smap.cmd;
console.debug("strcmd:"+strcmd)

        console.debug(" " + strcmd);
        if(strcmd === "getrecordparam"){
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
                vedioLayout.startWarn();
            }else{
                vedioLayout.endWarn();
            }
        }
    }
}



