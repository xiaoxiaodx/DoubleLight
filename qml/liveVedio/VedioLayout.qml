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
        height: parent.height - mhomeStateBar.height - rectlistwarninfo.height
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

    Rectangle{
        id:rectlistwarninfo
        property int preHeight: 160
        property int preWidth: 120
        width: parent.width
        height: rectlistwarninfo.preHeight + 57
        anchors.top: rowlayout.bottom
        color: "#000000"
        z:1
        Rectangle{
            id:statisticsPeople
            width: parent.width
            height: 37
            color: "#000000"
            Rectangle{
                id:rectnumerTotal
                width: txtnumberOfPeople.width + 24 + name.width
                height: parent.height - 10
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                color: "#cc193548"
                Text {
                    id: name
                    text: qsTr("总通行：")
                    color:"white"
                    font.pixelSize: 12
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    id: txtnumberOfPeople
                    text: pushwarnmodel.numberOfPeople
                    anchors.left: name.right
                    anchors.leftMargin: 2
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                    color: "#3B84F6"
                }
            }

            Rectangle{
                id:rectnumerNormal
                width: txtnumberOfNormalPeople.width + 24 +name1.width
                height: parent.height - 10
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: rectnumerTotal.right
                anchors.leftMargin: 10
                color: "#cc193548"
                Text {
                    id: name1
                    text: qsTr("正常：")
                    color:"white"
                    font.pixelSize: 12
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    id: txtnumberOfNormalPeople
                    text: pushwarnmodel.numberOfNormalPeople
                    anchors.left: name1.right
                    anchors.leftMargin: 2
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#179B0E"
                    font.pixelSize: 12
                }
            }

            Rectangle{
                id:rectnumberOfAbnormalTemperaturePeople
                width: txtnumberOfAbnormalTemperaturePeople.width + 24 +name2.width
                height: parent.height - 10
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: rectnumerNormal.right
                anchors.leftMargin: 10
                color: "#cc193548"
                Text {
                    id: name2
                    text: qsTr("异常：")
                    color:"white"
                    font.pixelSize: 12
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    id: txtnumberOfAbnormalTemperaturePeople
                    text: pushwarnmodel.numberOfAbnormalTemperaturePeople
                    anchors.left: name2.right
                    anchors.leftMargin: 2
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                    color: "#FA3F00"
                }
            }

            Rectangle{
                id:rectnomask
                width: txtnumberOfAbnormalTemperaturePeople.width + 24 +name3.width
                height: parent.height - 10
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: rectnumberOfAbnormalTemperaturePeople.right
                anchors.leftMargin: 10
                color: "#cc193548"
                Text {
                    id: name3
                    text: qsTr("未带口罩：")
                    color:"white"
                    font.pixelSize: 12
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    id: txtnumberOfNoMaskPeople
                    text: pushwarnmodel.numberOfNoMaskPeople
                    anchors.left: name3.right
                    anchors.leftMargin: 2
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                    color: "#EF9D36"
                }
            }
        }


        ListView{
            width: rectlistwarninfo.width
            height: rectlistwarninfo.height - statisticsPeople.height
            anchors.top: statisticsPeople.bottom
            orientation:ListView.Horizontal
            model: pushwarnmodel
            delegate: Rectangle{
                width: rectlistwarninfo.preWidth + 10
                height: rectlistwarninfo.preHeight + 20
                color: "#202020"
                Rectangle{
                    width: rectlistwarninfo.preWidth
                    height: rectlistwarninfo.preHeight
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    Image {
                        id:warnimg
                        width: parent.width
                        height: parent.width

                        source: "file:///"+absolutePath

                        //source: "qrc:/images/arrow_low.png"
                        //source: "file:///"+"F:/work/doubleLight/build-DoubleLight-Desktop_Qt_5_12_2_MinGW_64_bit-Release/image/111.png"
                    }
                    Rectangle{
                        color: "#cc193548"
                        width: parent.width
                        height: parent.height-warnimg.height
                        anchors.top: warnimg.bottom
                        Text {
                            id:txttemp
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.top: parent.top
                            anchors.topMargin: 2
                            font.pixelSize: 14
                            color: temptype===80?"#FA3F00":(temptype===81?"#179B0E":(temptype===82?"#EF9D36":"#FFFFFF"))
                            text: warnTemp
                        }
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.top: txttemp.bottom
                            anchors.topMargin: 3
                            font.pixelSize: 8
                            color: "#ffffff"
                            text: warnTime
                        }
                    }
                }
            }
        }

        layer.enabled: true
    }


    Image{
        id:imgWar
        width: 58
        height: 58
        anchors.right: parent.right
        anchors.rightMargin: 13
        anchors.top: parent.top
        anchors.topMargin: 68
        source: "qrc:/images/warn_ico.png"
        z:2
        opacity: 0
        SequentialAnimation {
            id:animationWarnOpacity
            //loops: Animation.Infinite
            alwaysRunToEnd: true
            NumberAnimation { target: imgWar; property: "opacity"; to: 1; duration: 200 }
            NumberAnimation { target: imgWar; property: "opacity"; to: 1; duration: 300 }
            NumberAnimation { target: imgWar; property: "opacity"; to: 0; duration: 200 }
        }
        function startAnimation(){

            animationWarnOpacity.start();
        }
        function stopAnimation(){

            animationWarnOpacity.stop();
            imgWar.opacity = 0;
        }
    }


    Image{
        id:imgRecord
        width: 57
        height: 24
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
        source: "qrc:/images/lablerecord.png"
        z:2
        opacity: 0
        SequentialAnimation {
            id:animationRecordOpacity
            loops: Animation.Infinite
            // alwaysRunToEnd: true
            NumberAnimation { target: imgRecord; property: "opacity"; to: 1; duration: 300 }
            NumberAnimation { target: imgRecord; property: "opacity"; to: 1; duration: 300 }
            NumberAnimation { target: imgRecord; property: "opacity"; to: 0; duration: 300 }
        }
        function startAnimation(){

            animationRecordOpacity.start();
        }
        function stopAnimation(){

            animationRecordOpacity.stop();
            imgRecord.opacity = 0;
        }
    }

    function startWarn(warntemp){

        if(!deviceconfig.getSwitchWarn())
            return;


        //  if(warnTimer.isDelay1000){
        //            warnTimer.isDelay1000 = false;
        //            warnTimer.start();
        //开启动画
        imgWar.startAnimation();
        //开启截图
        if(deviceconfig.getSwitchScreenShot())
            warnmanger.screenShot(deviceconfig.getScrennShotPath(),main,0 ,68,main.width,main.height-68,warntemp)

        //开启声音
        if(deviceconfig.getSwitchBeer())
            playWarn.play()
        //}
    }


    function startWarn1()
    {


        //开启声音
        if(deviceconfig.getSwitchBeer())
            playWarn.play()
    }

    function startRecordLable(){
        imgRecord.startAnimation();
    }
    function stopRecordLable(){
        imgRecord.stopAnimation();
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
        height:50
        anchors.bottom: parent.bottom
    }



    //    Timer{
    //        id:updateTimer
    //        repeat:true
    //        interval: 1800000;
    //        Component.onCompleted: updateTimer.start();
    //        onTriggered: {
    //            videoTemp.updateDate();
    //            videoNormal.updateDate();
    //        }
    //    }

    Connections{
        target: main
        onS_setLanguage:setLanguage(typeL);
    }


    function setLanguage(type){

        console.debug("setLanguage "+type)
        var index = 0;
        switch(type){
        case lEnglish:
            name3.text =  "No mask："
            name2.text =  "Alarm："
            name1.text = "Normal："
            name.text = "Amount："
            break;
        case lKorean:

            break;
        case lItaly:

            break;
        case lChinese:
            name3.text =  "未带口罩："
            name2.text =  "异常："
            name1.text = "正常："
            name.text = "总通行："
            break;
        case lRussian:

            break;
        case lLithuanian:

            break;
        }
    }

}
