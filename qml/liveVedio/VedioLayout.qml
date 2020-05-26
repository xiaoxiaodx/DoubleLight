import QtQuick 2.0
import QtQuick.Layouts 1.12
import "../"
Rectangle {

    id:root

    property int currentIndex: -1

    property alias isShowScreen: mhomeStateBar.visible

    color: "#252525"

    property int lockPreIndex: -1

    RowLayout{

        id: rowlayout
        width: parent.width
        height: parent.height - mhomeStateBar.height
        spacing: 0

        VideoNormal{
            id:videoNormal
            color: "#252525"
            Layout.fillHeight: true;
            Layout.fillWidth: true
            mIsSelected: 1 === currentIndex
            onClick: currentIndex = 1

            onDoubleClick: {
                if(videoTemp.visible){
                    videoTemp.visible = false;
                }else{
                    videoTemp.visible = true;
                }
            }
        }

        VideoTemp{
            id:videoTemp

            Layout.fillHeight: true;
            Layout.fillWidth: true
            color: "#252525"
            mIsSelected: 2 === currentIndex
            onClick: currentIndex = 2

            onDoubleClick: {
                if(videoNormal.visible){
                    videoNormal.visible = false;
                }else{
                    videoNormal.visible = true;
                }
            }
        }
    }

    Image{
        id:imgWar
        width: 58
        height: 58
        anchors.right: parent.right
        anchors.rightMargin: 13
        anchors.top: parent.top
        anchors.topMargin: 68
        source: "qrc:/images/warn_ico.png"
        z:2
        opacity: 0
        SequentialAnimation {
            id:animationWarnOpacity
            //loops: Animation.Infinite
            alwaysRunToEnd: true
            NumberAnimation { target: imgWar; property: "opacity"; to: 1; duration: 200 }
            NumberAnimation { target: imgWar; property: "opacity"; to: 1; duration: 300 }
            NumberAnimation { target: imgWar; property: "opacity"; to: 0; duration: 200 }
        }
        function startAnimation(){

            animationWarnOpacity.start();
        }
        function stopAnimation(){

            animationWarnOpacity.stop();
            imgWar.opacity = 0;
        }
    }


    Image{
        id:imgRecord
        width: 57
        height: 24
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
        source: "qrc:/images/lablerecord.png"
        z:2
        opacity: 0
        SequentialAnimation {
            id:animationRecordOpacity
            loops: Animation.Infinite
            // alwaysRunToEnd: true
            NumberAnimation { target: imgRecord; property: "opacity"; to: 1; duration: 300 }
            NumberAnimation { target: imgRecord; property: "opacity"; to: 1; duration: 300 }
            NumberAnimation { target: imgRecord; property: "opacity"; to: 0; duration: 300 }
        }
        function startAnimation(){

            animationRecordOpacity.start();
        }
        function stopAnimation(){

            animationRecordOpacity.stop();
            imgRecord.opacity = 0;
        }
    }

    function startWarn(warntemp){

        if(!deviceconfig.getSwitchWarn())
            return;

     //  if(warnTimer.isDelay1000){
//            warnTimer.isDelay1000 = false;
//            warnTimer.start();
            //开启动画
            imgWar.startAnimation();
            //开启截图
            if(deviceconfig.getSwitchScreenShot())
                warnmanger.screenShot(deviceconfig.getScrennShotPath(),main,0 ,68,main.width,main.height-68,warntemp)

            //开启声音
            if(deviceconfig.getSwitchBeer())
                playWarn.play()
        //}

    }


    function startRecordLable(){
        imgRecord.startAnimation();
    }
    function stopRecordLable(){
        imgRecord.stopAnimation();
    }

    Connections{
        target: main
        onLockerCHange:{

            if(lockchange){
                lockPreIndex = currentIndex
                currentIndex = -1
            }else
                currentIndex = lockPreIndex
        }
    }

    HomeStates{
        id:mhomeStateBar
        width: parent.width
        height:50
        anchors.bottom: parent.bottom
    }



    Timer{
        id:updateTimer
        repeat:true
        interval: 1800000;
        Component.onCompleted: updateTimer.start();
        onTriggered: {
            videoTemp.updateDate();
            videoNormal.updateDate();
        }
    }



}
