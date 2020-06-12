import QtQuick 2.0
import QtQuick.Controls 2.5
import "../simpleControl"
import QtGraphicalEffects 1.12
import QtQuick 2.0
import QtMultimedia 5.4
Popup {
    id: root
    x: parent.width/2 - root.width/2
    y: parent.height/2 - root.height/2
    //    width: 380
    //    height: 253
    modal: true
    focus: true
    //设置窗口关闭方式为按“Esc”键关闭
    closePolicy: Popup.OnEscape
    //设置窗口的背景控件，不设置的话Popup的边框会显示出来
    background: rect
    dim:false

    property alias videoSrc: mediaPlayer.source

    Rectangle {
        id: rect
        anchors.fill: parent
        color: "#ffffff"
        radius: 3

        Rectangle {
            color: "black"
            anchors.fill: parent

            MediaPlayer {
                id: mediaPlayer

                autoPlay: true
            }

            VideoOutput {
                id: video
                anchors.fill: parent
                source: mediaPlayer
            }

            layer.enabled: true
            layer.effect: DropShadow {
                transparentBorder: true
                horizontalOffset: 4
                verticalOffset: 4
                color:"#80000000"
            }
        }

        Image {
            id: img11
            width: 18
            height: 18
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5
            source: "qrc:/images/img_close.png"

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    mediaPlayer.stop();
                    mediaPlayer.source = "";
                    root.close();
                }
            }
        }
    }




}
