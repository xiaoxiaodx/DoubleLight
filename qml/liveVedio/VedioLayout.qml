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
        height:isLocker?0:50
        anchors.bottom: parent.bottom
    }



    Timer{
        id:updateTimer
        repeat:true
        interval: 1800000;

        onTriggered: {
            videoTemp.updateDate();
            videoNormal.updateDate();
        }
    }



}
