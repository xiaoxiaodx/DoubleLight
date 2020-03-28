import QtQuick 2.0
import XVideo 1.0
import QtQuick.Controls 1.4
import "../simpleControl"
Rectangle {

    signal doubleClick(bool isFullScreen);
    signal click();
    signal s_showToastMsg(string str)


    property string shotScrennFilePath: ""
    property string recordingFilePath: ""
    property bool mIsSelected: false
    property int videoType : 0
    border.color: mIsSelected?"#ff6815":"#252525"
    border.width: 2


    XVideo{
        id:video
        anchors.fill: parent
        anchors.margins: 2

        Component.onCompleted: {
            if(videoType === 1){
                video.startNormalVideo();

            }else if(videoType === 2){
                video.startTemperatureVideo();
            }
        }
        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            propagateComposedEvents:true

            onClicked: click();

            onDoubleClicked:doubleClick(true);

        }

//        Rectangle{
//            id:screenBlack
//            anchors.fill: parent
//            visible:true
//            color: "#252525"
//        }



        onSignal_loginStatus: main.showToast(msg);
    }

    function screenShotBtnClick(){

        screenShotMask.startAnimation();
        video.funScreenShot();
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
        onS_temOffset:video.fun_temOffset(mvalue);

    }
}



