import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Window 2.12
import QtQml 2.12
import ScreenVideo 1.0
import "../qml/liveVedio"
import "simpleControl"
Rectangle {

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

    HomeMenu{
        id:homeMenu
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: 68
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#5D9CFF"}
            GradientStop { position: 1.0; color: "#2D76E7"}
        }
        MouseArea{
            property point clickPoint: "0,0"

            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            propagateComposedEvents: true
            onPressed: {
                clickPoint  = Qt.point(mouse.x, mouse.y)
            }
            //双击过程会出现拖拉事件，导致窗口最大化到还原过程出现bug,因此禁掉
            //            onDoubleClicked: {
            //                enabled = false;
            //                winMax();
            //                enabled = true;
            //            }
            onPositionChanged: {

                var offset = Qt.point(mouse.x - clickPoint.x, mouse.y - clickPoint.y)

                dragPosChange(offset.x, offset.y)
            }
        }
        onMCurIndexChanged: {
            if(mCurIndex == 2){
                deviceconfig.open();
            }
        }
    }


    ListModel{
        id:listDeviceDataModel
        Component.onCompleted: {

            listDeviceDataModel.append({videoType:1,isMax:0,deviceName:""});
            listDeviceDataModel.append({videoType:2,isMax:0,deviceName:""});
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


            myModel: listDeviceDataModel
            onS_click: modelDataCurrentIndex=clickIndex

            onS_doubleClick: {


            }
        }
        Rectangle{
            id:screenShotMask
            anchors.fill: parent
            color: "white"
            opacity: 0

            SequentialAnimation {

                id:animationOpacity
                NumberAnimation { target: screenShotMask; property: "opacity"; to: 0.5; duration: 100 }
                NumberAnimation { target: screenShotMask; property: "opacity"; to: 0; duration: 100 }
            }
            function startAnimation(){
                animationOpacity.start();
            }
        }
    }


    Rectangle{
        id:mhomeStateBar
        width: parent.width
        height:50
        anchors.bottom: parent.bottom

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#5D9CFF"}
            GradientStop { position: 1.0; color: "#2D76E7"}
        }



        states: [
            State {
                name: "show"; PropertyChanges { target: mhomeStateBar; opacity: 1 }
            },
            State {
                name: "hide"; PropertyChanges { target: mhomeStateBar;  opacity: 0 }
            }]

        transitions: Transition {
            PropertyAnimation  {properties: "opacity"; duration: 600; easing.type: Easing.Linear  }
        }

        MouseArea{
            id:mouse
            anchors.fill: parent
            hoverEnabled: true
            //enabled: true
            // preventStealing:true
            propagateComposedEvents:true
            onEntered:{

                mhomeStateBar.state = "show"

                //enabled = false;
            }
            onExited:mhomeStateBar.state = "hide"

        }


        Row{
            id:windowAdjust
            anchors.right: parent.right
            anchors.rightMargin: 60
            anchors.verticalCenter: parent.verticalCenter
            spacing:40
            Image{
                id:img1
                width: 34
                height: 34
                source: "qrc:/images/lock.png"
                property bool isLock: false
                MouseArea{
                    anchors.fill:parent
                    onPressed: {

                        if(img1.isLock){
                            img1.source = "qrc:/images/lock.png"

                        }else{
                            img1.source = "qrc:/images/lock_p.png"
                        }
                        img1.isLock = !img1.isLock;
                    }

                }
            }
            Image{
                id:img2
                width: 34
                height: 34
                source: "qrc:/images/recordv.png"
                property bool isRecord: false
                MouseArea{
                    anchors.fill:parent
                    onPressed: {
                        if(img2.isRecord){


                            if(screenv.funEndScreenRecrod()){
                                img2.source = "qrc:/images/recordv.png"
                                img2.isRecord = false;
                            }

                        }else{

                            if(screenv.funStartScreenRecrod(deviceconfig.getRecordPath())){
                                img2.source = "qrc:/images/recordv_p.png";
                                img2.isRecord = true;
                            }
                        }

                    }
                }
            }

            Image{
                id:img3
                width: 34
                height: 34
                source: "qrc:/images/replay.png"
                MouseArea{
                    anchors.fill:parent
                    onPressed: {
                        img3.source = "qrc:/images/replay_p.png"

                    }
                    onReleased:img3.source = "qrc:/images/replay.png"
                }
            }
        }
    }

    Timer{
        id:delayTimer

        triggeredOnStart:true

        repeat:true

        interval: 65;

        onTriggered: {

        }


    }

    ScreenVideo{
        id:screenv
    }


    DeviceConfig{
        id:deviceconfig
        width: 1888
        height: 922


    }



}
