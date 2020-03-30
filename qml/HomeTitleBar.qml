import QtQuick 2.0
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls 2.12
import Qt.labs.platform 1.1
import QtQuick 2.12
import QtQuick.Controls 2.12

import "../qml/simpleControl"

Rectangle {

    id:root

    property bool isPress: false


    signal winMin();
    signal winMax();
    signal winClose();
    signal dragPosChange(var tx,var ty);


    Rectangle {
        anchors.fill: parent

        color: "transparent"
        Image {
            id: namew
            width: 130
            height: 22
            anchors.left: parent.left
            anchors.leftMargin: 25
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/images/iEagleCam2.png"
        }



        MouseArea {
            id:mousearea
            property point clickPoint: "0,0"

            anchors.fill: parent
            acceptedButtons: Qt.LeftButton

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


        Row{
            id:windowAdjust
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            spacing:20
            QmlImageButton{
                width: 20
                height: 20
                imgSourseHover: "qrc:/images/win_min_p.png"
                imgSourseNormal: "qrc:/images/win_min.png"
                imgSoursePress: "qrc:/images/win_min_p.png"
                onClick:winMin()
            }
            QmlImageButton{
                width: 20
                height: 20
                imgSourseHover: "qrc:/images/win_max_p.png"
                imgSourseNormal: "qrc:/images/win_max.png"
                imgSoursePress: "qrc:/images/win_max_p.png"
                onClick:winMax()

            }
            QmlImageButton{
                width: 20
                height: 20
                imgSourseHover: "qrc:/images/win_close_p.png"
                imgSourseNormal: "qrc:/images/win_close.png"
                imgSoursePress: "qrc:/images/win_close_p.png"
                onClick:winClose()


            }

        }

    }


    function strToimg(str){

        return pathMapping[str]

    }

}
