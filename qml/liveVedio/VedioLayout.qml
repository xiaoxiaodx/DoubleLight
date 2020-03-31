import QtQuick 2.0
import QtQuick.Layouts 1.12
Rectangle {

    id:root

    property int rowNum: 1
    property int columnNum: 2
    //property alias myModel: repeater.model

    property int currentIndex: -1



    signal s_doubleClick(var index,var ismax);
    signal s_click(var clickIndex);
    signal s_showToastMsg(string str)
    signal s_deleteObject()
    signal s1_authenticationFailue(string str)

    color: "#252525"

    //    Repeater{
    //        id:repeater

    RowLayout{
        anchors.fill: parent
        id: rowlayout
        spacing: 0
        VideoLivePlay{
            id:videoNormal
            color: "#252525"
            Layout.fillHeight: true;
            Layout.fillWidth: true
            videoType:1
            mIsSelected: 1 === currentIndex

            onClick: {

                currentIndex = 1

            }

            onDoubleClick: {
                if(videoTemp.visible){
                    videoTemp.visible = false;
                }else{
                    videoTemp.visible = true;
                }
            }


        }

        VideoLivePlay{
            id:videoTemp
            videoType:2
            Layout.fillHeight: true;
            Layout.fillWidth: true
            color: "#252525"
            mIsSelected: 2 === currentIndex
            onClick: {
                currentIndex = 2
            }

            onDoubleClick: {

                if(videoNormal.visible){
                    videoNormal.visible = false;
                }else{
                    videoNormal.visible = true;
                }
            }




        }
    }





}
