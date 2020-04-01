import QtQuick 2.0

Rectangle{
    id:mhomeStateBar



    signal s_testClick();
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#5D9CFF"}
        GradientStop { position: 1.0; color: "#2D76E7"}
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
            property bool isChecked: false
            MouseArea{
                anchors.fill:parent
                onClicked: {

                    if(!deviceconfig.getSwitchRecord()){
                        main.showToast(qsTr("禁止录像"))
                        return;
                    }else{
                       if(!img2.isChecked){

                           img2.isChecked = true;
                           img2.source = "qrc:/images/recordv_p.png"
                           captureScrennTimer.isRecord = true
                           vedioLayout.startRecordLable();
                           if(!captureScrennTimer.running)
                               captureScrennTimer.start()
                       } else{
                            img2.isChecked = false;
                          img2.source = "qrc:/images/recordv.png"
                           captureScrennTimer.isRecord = false
                           vedioLayout.stopRecordLable();
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

        property int tCount: 0
        property bool isRecord: false
        property int min30count: 25*60*30;
        triggeredOnStart:true

        repeat:true

        interval: 40;

        onTriggered: {

            if(!isRecord){
                tCount = 0 ;
                screenv.funEndScreenRecrod()
                captureScrennTimer.stop();
                return;
            }

            if(tCount === 0){//时钟为0时 开始新的一次文件录像
                if(!screenv.funStartScreenRecrod(deviceconfig.getRecordPath()))
                  return
            }

            screenv.funScreenRecrod(main,0 ,68,main.width,main.height-68);


            tCount++;
            if(tCount >= min30count){

                if(screenv.funEndScreenRecrod()){
                    tCount = 0 ;
                }
            }




        }
    }
}
