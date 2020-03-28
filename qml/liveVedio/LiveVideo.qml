import QtQuick 2.0
//import QtQuick.Controls 2.5
//import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
import "../simpleControl"
Rectangle{
    id:rect
    MouseArea{
        anchors.fill: parent

        onClicked: ;
        onPressed: ;

    }

    signal st_showToastMsg(string str1);
    signal s_multiScreenNumChange(int num);

    property int multiScreenNum: 2
    property int premultiScreenNum: 2

    property int modelDataCurrentIndex: -1
    property int listDeviceCurrentIndex: -1

    property string shotScreenFilePath2: ""
    property string recordingFilePath2: ""

    property bool isMax: false

    MouseArea{
        anchors.fill: parent

        onClicked: ;
        onPressed: ;

    }




    Rectangle{
        id:vedioContent
        anchors.left: leftContent.right
        anchors.top: parent.top
        width: parent.width - leftContent.width
        height: parent.height
        color: "#131415"
        VedioLayout{
            id: vedioLayout
            height: parent.height - mvideomultiWindow.height
            width: parent.width;

            myLayoutSquare:multiScreenNum

            myModel: listDeviceDataModel
            recordingFilePath1: recordingFilePath2
            shotScreenFilePath1: shotScreenFilePath2


            Component.onCompleted: {
                var listCount = listDeviceDataModel.count;
                var needCount = multiScreenNum*multiScreenNum;


                for(var i = listCount;i < needCount;i++){

                    listDeviceDataModel.append({did:"",account:"",password:"",ip:"",port:"",isCreateConnected:0,isMax:0,deviceName:""});

                }

            }

            onS_click: modelDataCurrentIndex=clickIndex



            onS_doubleClick: {

                //console.debug("onS_doubleClick  " + multiScreenNum + "  "+premultiScreenNum + "  "+ismax)

                if(ismax > 0 ){

                    premultiScreenNum = multiScreenNum;
                    s_multiScreenNumChange(1)

                }else{

                    multiScreenNum = premultiScreenNum
                    s_multiScreenNumChange(premultiScreenNum)
                }
            }

            onS1_authenticationFailue: {

                for(var i=0;i<listDeviceDid.count;i++){

                    if(listDeviceDid.get(i).did == str)
                        listDeviceDid.remove(i)
                }

            }

        }

        VideoMultiWindow{

            id:mvideomultiWindow
            anchors.top:vedioLayout.bottom
            width: parent.width
            height: 50
            color: "#131415"
            onS_multiScreenNumChange: {

                console.debug("VideoMultiWindow "+num)
                if(num < 5)
                    setMultiScreenNum(num)

            }
        }


    }

    ListModel{
        id:listDeviceDataModel

    }








    function setMultiScreenNum(num){

        multiScreenNum = num;

        if(num > 1){

            for(var i=0;i<listDeviceDataModel.count;i++){

                if(listDeviceDataModel.get(i).isMax > 0){
                    listDeviceDataModel.get(i).isMax = 0;
                    break
                }

            }

        }
        for(var i= listDeviceDataModel.count ; i<=multiScreenNum*multiScreenNum;i++)
            listDeviceDataModel.append({did:"",account:"",password:"",ip:"",port:"",isCreateConnected:0,isMax:0});
    }


    function sendCloudConrol(startStr,direction){

        //console.debug("sendCloudConrol  " + listDeviceDataModel.get(modelDataCurrentIndex).deviceName)
        if(modelDataCurrentIndex > -1){
            var map={movecmd:startStr,direction:direction,speedx:200,speedy:0,speedz:0,posx:0,posy:0,posz:0}
            devicemanagerment.funP2pSendData(listDeviceDataModel.get(modelDataCurrentIndex).deviceName,"setptzmove",map)

        }else
            showToast(qsTr("Unspecified device"))
    }


}
