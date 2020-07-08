import QtQuick 2.0

import QtQuick.Controls 2.5
import "./simpleControl"
import QtGraphicalEffects 1.12
Rectangle{
    id:root



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
            source:"qrc:/images/fullscreen.png"
            property bool isLock: false
            MouseArea{
                anchors.fill:parent
                onClicked: isLocker = true;
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


                       if(!img2.isChecked){

                           if(!deviceconfig.getSwitchRecord()){
                               //main.showToast(qsTr("禁止录像"))
                               return;
                           }
                           img2.isChecked = true;
                           img2.source = "qrc:/images/recordv_p.png"
                           captureScrennTimer.isRecord = true
                           vedioLayout.startRecordLable();
                           if(!captureScrennTimer.running){
                               screenv.funCreateAviRecordThread(deviceconfig.getRecordPath()+"/"+deviceconfig.deviceDid,captureScrennTimer.interval);
                               captureScrennTimer.start()
                           }
                       } else{
                            img2.isChecked = false;
                            img2.source = "qrc:/images/recordv.png"
                           captureScrennTimer.isRecord = false
                           vedioLayout.stopRecordLable();
                       }


                }
            }
        }

//        Image{
//            id:img3
//            width: 34
//            height: 34
//            source: "qrc:/images/replay.png"
//            MouseArea{
//                anchors.fill:parent
//                onPressed: {
//                    img3.source = "qrc:/images/replay_p.png"

//                }
//                onReleased:img3.source = "qrc:/images/replay.png"

//                onClicked: qmlLanguage.setLanguage(1)
//            }
//        }
    }



    Image {
        id: imgVersion
        width: 29
        height: 29
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.verticalCenter: parent.verticalCenter
        source: "qrc:/images/imgVersion.png"
        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onClicked: popVersion.open()
            onEntered: imgVersion.source = "qrc:/images/imgVersion_h.png"
            onExited: imgVersion.source = "qrc:/images/imgVersion.png"
        }
    }


    Popup {
        id: popVersion
        x: imgVersion.x + imgVersion.width + 5
        y: imgVersion.y
        modal: true
        focus: true
        //设置窗口关闭方式为按“Esc”键关闭
        closePolicy: Popup.OnEscape
        //设置窗口的背景控件，不设置的话Popup的边框会显示出来
        background: rect
        dim:false


        Rectangle {
            id: rect

            width: txtVersion.width + 25 + imgcopy.width
            height: txtVersion.height+10
            color: "#ffffff"
            radius: 4
            Text {
                id: txtVersion
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                color: "#333333"
                font.pixelSize: 12
                text: qsTr("")
            }
            Image{
                id:imgcopy
                width: 10
                height: 10
                source: "qrc:/images/imgVersionCopy.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: txtVersion.right
                anchors.leftMargin: 5

                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        warnmodel.setClipbord(txtVersion.text);
                    }
                    onEntered: imgcopy.source = "qrc:/images/imgVersionCopy_h.png"
                    onExited: imgcopy.source = "qrc:/images/imgVersionCopy.png"
                }
            }
        }
    }

    function setVersionInfo( str){
        txtVersion.text = str
    }

}
