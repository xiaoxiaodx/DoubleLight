import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Window 2.12
import QtQml 2.12
import ScreenVideo 1.0
import QtMultimedia 5.8
import "../qml/liveVedio"
import "../qml/playbackVideo"
import "simpleControl"
Rectangle {

    enum ADJUSTW {
        WTOP,
        WBOTTOM,
        WLEFT,
        WRIGHT,
        WLEFTTOP,
        WRIGHTTOP,
        WLEFTBOTTOM,
        WRIGHTBOTTOM
    }
    id: maincontent;

    property bool fullscreen: false
    property bool isPress: false

    property int mWindowStates: 4   // 2：正常 4：最大化

    property int preX:0
    property int preY:0
    property int preWidth:0
    property int preHeight:0

    property string toastStr: ""

    property string versionstr: "version: v1.1.2"

    property int curVindex: -1


    property int modelDataCurrentIndex: -1



    visible: false

    signal winMin();
    signal winMax();
    signal winClose();
    signal dragPosChange(var mx,var my);
    color: "#252525"
    HomeMenu{
        id:homeMenu
        property bool isDoubleClick: false
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: isLocker?0:68

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#5D9CFF"}
            GradientStop { position: 1.0; color: "#2D76E7"}
        }


        onSwinMin:  winMin()
        onSwinClose: winClose()
        onSwinMax: winMax()

        MouseArea{
            property point clickPoint: "0,0"

            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            propagateComposedEvents: true
            onPressed: {
                homeMenu.isDoubleClick = false;
                clickPoint  = Qt.point(mouse.x, mouse.y)
            }

            onDoubleClicked: {

                homeMenu.isDoubleClick = true
                winMax();


            }
            onPositionChanged: {

                if(!homeMenu.isDoubleClick){
                    var offset = Qt.point(mouse.x - clickPoint.x, mouse.y - clickPoint.y)

                    dragPosChange(offset.x, offset.y)
                }


            }
        }



    }


    ListModel{
        id:listDeviceDataModel
        Component.onCompleted: {

            listDeviceDataModel.append({videoType:1,isMax:0,deviceName:""});
            listDeviceDataModel.append({videoType:1,isMax:0,deviceName:""});
        }
    }

    Rectangle{
        id:vedioContent
        anchors.left: parent.left
        anchors.top: homeMenu.bottom
        width: parent.width
        height: parent.height - homeMenu.height
        color: "#252525"
        VedioLayout{
            id: vedioLayout
            height: parent.height
            width: parent.width;
            isShowScreen:!isLocker
            property bool isWarn: false

            onS_click: modelDataCurrentIndex=clickIndex
            z:homeMenu.mCurIndex == 0?1:0
            onS_doubleClick: {



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


            function startWarn(){

                if(!deviceconfig.getSwitchWarn())
                    return;
                if(!warnTimer.running){
                    warnTimer.start();
                }
                warnTimer.haveWarnMsg = true;
            }


            function startRecordLable(){
                imgRecord.startAnimation();
            }
            function stopRecordLable(){
                imgRecord.stopAnimation();
            }


        }

        PlaybackVideo{
            id:rectRepaly
            width: parent.width
            height: parent.height
            color: "#ffffff"
            z:homeMenu.mCurIndex == 1?1:0
        }

        DeviceConfig{
            id:deviceconfig
            height: parent.height
            width: parent.width;
            color: "#DFE1E6"
            z:homeMenu.mCurIndex == 2?1:0
        }
    }



    ScreenVideo{
        id:screenv
    }

    MediaPlayer {
        id: playWarn
        source: "qrc:/alarm.wav"
    }

    Timer{
        id:warnTimer
        property bool haveWarnMsg: false
        repeat: true
        interval: 1000
        triggeredOnStart:true
        onTriggered: {

            console.debug("dingshiqi:")
            //如果此时没有报警,则关闭定时器
            if(!haveWarnMsg){
                warnTimer.stop();
                return;
            }

            //播放过程都会自动停止

            //开启动画
            imgWar.startAnimation();
            //开启录屏
            if(deviceconfig.getSwitchScreenShot())
                screenv.funScreenShoot(deviceconfig.getScrennShotPath(),main,0 ,68,main.width,main.height-68);
            //开启声音
            if(deviceconfig.getSwitchBeer())
                playWarn.play()


            haveWarnMsg = false;

        }
    }
}
