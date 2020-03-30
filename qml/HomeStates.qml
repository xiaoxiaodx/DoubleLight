import QtQuick 2.0

Rectangle{
    id:mhomeStateBar



    signal s_testClick();
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#5D9CFF"}
        GradientStop { position: 1.0; color: "#2D76E7"}
    }



    //    states: [
    //        State {
    //            name: "show"; PropertyChanges { target: mhomeStateBar; opacity: 1 }
    //        },
    //        State {
    //            name: "hide"; PropertyChanges { target: mhomeStateBar;  opacity: 0 }
    //        }]

    //    transitions: Transition {
    //        PropertyAnimation  {properties: "opacity"; duration: 600; easing.type: Easing.Linear  }
    //    }

    //    MouseArea{
    //        id:mouse
    //        anchors.fill: parent
    //        hoverEnabled: true
    //        //enabled: true
    //        // preventStealing:true
    //        propagateComposedEvents:true
    //        onEntered:{

    //            mhomeStateBar.state = "show"

    //            //enabled = false;
    //        }
    //        onExited:mhomeStateBar.state = "hide"

    //    }


    Row{
        id:windowAdjust
        anchors.right: parent.right
        anchors.rightMargin: 60
        anchors.verticalCenter: parent.verticalCenter
        spacing:40
        visible: homeMenu.mCurIndex === 0
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
                        //s_testClick()
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

                    if(!deviceconfig.getSwitchRecord()){
                        main.showToast(qsTr("禁止录像"))
                        return;
                    }
                    if(img2.isRecord){

                        captureScrennTimer.stop()
                        if(screenv.funEndScreenRecrod()){
                            img2.source = "qrc:/images/recordv.png"
                            img2.isRecord = false;
                            vedioLayout.stopRecordLable();
                        }

                    }else{

                        if(screenv.funStartScreenRecrod(deviceconfig.getRecordPath())){
                            captureScrennTimer.start();
                            img2.source = "qrc:/images/recordv_p.png";
                            vedioLayout.startRecordLable();
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

    Timer{
        id:captureScrennTimer

        triggeredOnStart:true

        repeat:true

        interval: 65;

        onTriggered: {
            screenv.funScreenRecrod(main,0 ,68,main.width,main.height-68);
        }
    }
}
