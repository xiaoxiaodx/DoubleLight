import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Window 2.12
import QtQml 2.12
import ScreenVideo 1.0
import QtMultimedia 5.8
import QtQuick.Controls 2.5
import "../qml/liveVedio"
import "../qml/playbackVideo"
import "../qml/warnManager"
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


    //   property int modelDataCurrentIndex: -1

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
        height: 68
        visible:!isLocker
        z:2
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#5D9CFF"}
            GradientStop { position: 1.0; color: "#2D76E7"}
        }


        onSwinMin:  winMin()
        onSwinClose: winClose()
        onSwinMax: winMax()

    }
    SwipeView {
          id:vedioContent
          anchors.left: parent.left
          anchors.top: homeMenu.bottom
          width: parent.width
          height: parent.height - homeMenu.height
          currentIndex:homeMenu.mCurIndex
          interactive:false
          Component.onCompleted:{
                  contentItem.highlightMoveDuration = 0      //将移动时间设为0
              }
          z:1
          VedioLayout{
              id: vedioLayout
              isShowScreen:!isLocker
          }



          DeviceConfig{
              id:deviceconfig
              color: "#DFE1E6"
          }

          WarnManager{
              id:warnmanger
              color: "#DFE1E6"
          }
      }

    ScreenVideo{
        id:screenv
        Component.onCompleted: {
            screenv.funCreateAviRecordThread(deviceconfig.getScrennShotPath(),deviceconfig.getRecordPath(),captureScrennTimer.interval);
        }
    }


    MediaPlayer {
        id: playWarn
        source: "qrc:/alarm.wav"
    }

    Timer{
        id:captureScrennTimer
        property int tCount: 0
        property bool isRecord: false
        triggeredOnStart:true

        repeat:true

        interval: 40;

        onTriggered: {

            if(!isRecord){
                screenv.funForfinishRecord();
                captureScrennTimer.stop();
                return;
            }

            screenv.funScreenRecrod(deviceconfig.getRecordPath(),main,0 ,68,main.width,main.height-68);
        }
    }



    Timer{
        id:warnTimer
        property bool isDelay1000: true
        repeat: false
        interval: 1000
        triggeredOnStart:false
        onTriggered: {

            warnTimer.isDelay1000 = true;
        }
    }
}
