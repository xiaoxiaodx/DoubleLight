import QtQuick 2.0
import XVideo 1.0
import Qt.labs.settings 1.0
import QtQuick.Controls 1.4
import "../simpleControl"
Rectangle {

    id:mPlayRect
    signal doubleClick(bool isFullScreen);
    signal click();
    signal s_showToastMsg(string str)
    signal s_sendList(var vmap)

    property bool mIsSelected: false

    border.color: mIsSelected?"#98C5FF":"#252525"
    border.width: 3

    signal s_startTemWarn();
    signal s_endTemWarn();
    signal s_tempmodelSelect(var mtype);
    signal s_testRect(var x0,var y0,var w0,var h0,var x1,var y1,var w1,var h1,var x2,var y2,var w2,var h2);


    property int mouseAdjustWidth1: 4
    property int    wTOP:1
    property int    wBOTTOM:2
    property int    wLEFT:3
    property int    wRIGHT:4
    property int    wLEFTTOP:5
    property int    wRIGHTTOP:6
    property int    wLEFTBOTTOM:7
    property int    wRIGHTBOTTOM:8
    property int    wCenter:9
    property point mousePressPt1: "0,0"


    MouseArea{
        anchors.fill: parent
        //hoverEnabled: true
        propagateComposedEvents:true
        onClicked: {
            click()
            // mouse.accepted = false
        }
        onDoubleClicked:doubleClick(true);
    }

//    Text {
//        id: pos1
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.bottomMargin: 10
//        anchors.bottom:parent.bottom
//        color: "red"
//        text: qsTr("text")
//    }
    XVideo{
        id:video

        //property real whradia:  0.75
       //property real hwradia: 1.333
        property real whradia: 0.5625
        property real hwradia: 1.778
        //anchors.fill: parent

        width:(mPlayRect.width*whradia>mPlayRect.height?mPlayRect.height*hwradia:mPlayRect.width) -6
        height: (mPlayRect.width*whradia>mPlayRect.height?mPlayRect.height:mPlayRect.width*whradia) -6

        anchors.horizontalCenter: mPlayRect.horizontalCenter
        anchors.verticalCenter: mPlayRect.verticalCenter

        Component.onCompleted:{
            //video.fun_setInitPar(deviceconfig.getTcpip(),deviceconfig.getShowParentW(),deviceconfig.getShowParentH(),deviceconfig.getShowRectX(),deviceconfig.getShowRectY(),deviceconfig.getShowRectW(),deviceconfig.getShowRectH())
           // video.startNormalVideo(deviceconfig.getWarnTem())
        }

        onSignal_loginStatus: main.showToast(msg);

        onSignal_httpUiParSet:httpParCallback(map);

        onSignal_connected:{
            deviceconfig.setTcpip(ip)
            homeMenu.setDeviceConnectState(istrue)
        }
//                MouseArea{
//                    id:mouse22
//                    anchors.fill: parent
//                    cursorShape: Qt.CrossCursor
//                    onClicked: {
//                        var kx = video.width / 1920;
//                        var ky = video.height / 1080;
//                        var x1 = mouse.x / kx;
//                        var y1 = mouse.y / ky;
//                        pos1.text ="pos:"+ x1 +"    "+y1
//                    }
//                }
        Rectangle{
            id:rectadmjt
            x:0//deviceconfig.getShowRectX()
            y:0//deviceconfig.getShowRectY()
            width: 100//deviceconfig.getShowRectW()
            height:100// deviceconfig.getShowRectH()
            color: "#505D9CFF"
            visible:false;//deviceconfig.getIsOpenAdjustRect();
            MouseArea{
                id:areaTop
                x:mouseAdjustWidth1
                y:0
                width: parent.width - mouseAdjustWidth1*2
                height: mouseAdjustWidth1
                hoverEnabled: true

                onEntered: cursorShape = Qt.SizeVerCursor
                onExited: cursorShape = Qt.ArrowCursor
                onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                onPositionChanged: {

                    if(pressed){
                        var offsetX = mouse.x - mousePressPt1.x
                        var offsetY = mouse.y - mousePressPt1.y
                        adjustWindow(wTOP,0,offsetY);
                    }
                }
            }

            MouseArea{
                id:areaLeftTop
                x:0
                y:0
                width: mouseAdjustWidth1
                height: mouseAdjustWidth1
                hoverEnabled: true

                onEntered: cursorShape = Qt.SizeFDiagCursor
                onExited: cursorShape = Qt.ArrowCursor
                onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                onPositionChanged: {

                    if(pressed){
                        var offsetX = mouse.x - mousePressPt1.x
                        var offsetY = mouse.y - mousePressPt1.y
                        adjustWindow(wLEFTTOP,offsetX,offsetY);
                    }
                }

            }

            MouseArea{
                id:areaRightTop
                x:parent.width - mouseAdjustWidth1
                y:0
                width: mouseAdjustWidth1
                height: mouseAdjustWidth1
                hoverEnabled: true

                onEntered: cursorShape = Qt.SizeBDiagCursor
                onExited: cursorShape = Qt.ArrowCursor
                onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                onPositionChanged: {

                    if(pressed){
                        var offsetX = mouse.x - mousePressPt1.x
                        var offsetY = mouse.y - mousePressPt1.y
                        adjustWindow(wRIGHTTOP,offsetX,offsetY);
                    }
                }

            }

            MouseArea{
                id:areaLeft
                x:0
                y:mouseAdjustWidth1
                width: mouseAdjustWidth1
                height: parent.height - mouseAdjustWidth1
                hoverEnabled: true

                onEntered: cursorShape = Qt.SizeHorCursor
                onExited: cursorShape = Qt.ArrowCursor
                onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                onPositionChanged: {

                    if(pressed){
                        var offsetX = mouse.x - mousePressPt1.x
                        var offsetY = mouse.y - mousePressPt1.y
                        adjustWindow(wLEFT,offsetX,0);
                    }
                }

            }

            MouseArea{
                id:areaRight
                x:parent.width - mouseAdjustWidth1
                y:0
                width: mouseAdjustWidth1
                height: parent.height - mouseAdjustWidth1
                hoverEnabled: true
                onEntered: cursorShape = Qt.SizeHorCursor
                onExited: cursorShape = Qt.ArrowCursor
                onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                onPositionChanged: {


                    if(pressed){
                        var offsetX = mouse.x + mousePressPt1.x
                        var offsetY = mouse.y + mousePressPt1.y
                        adjustWindow(wRIGHT,offsetX,0);
                    }

                }
            }

            MouseArea{
                id:areaLeftBottom
                x:0
                y:parent.height - mouseAdjustWidth1
                width: mouseAdjustWidth1
                height: mouseAdjustWidth1
                hoverEnabled: true

                onEntered: cursorShape = Qt.SizeBDiagCursor
                onExited: cursorShape = Qt.ArrowCursor
                onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                onPositionChanged: {

                    if(pressed){
                        var offsetX = mouse.x - mousePressPt1.x
                        var offsetY = mouse.y - mousePressPt1.y
                        adjustWindow(wLEFTBOTTOM,offsetX,offsetY);
                    }
                    //setDlgPoint(offset.x, 0)

                }
            }

            MouseArea{
                id:areaRightBottom
                x:rectadmjt.width - mouseAdjustWidth1
                y:rectadmjt.height - mouseAdjustWidth1
                width: mouseAdjustWidth1
                height: mouseAdjustWidth1
                hoverEnabled: true

                onEntered: cursorShape = Qt.SizeFDiagCursor
                onExited: cursorShape = Qt.ArrowCursor
                onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                onPositionChanged: {

                    if(pressed){
                        var offsetX = mouse.x - mousePressPt1.x
                        var offsetY = mouse.y - mousePressPt1.y
                        adjustWindow(wRIGHTBOTTOM,offsetX,offsetY);
                    }
                    //setDlgPoint(offset.x, 0)
                }
            }

            MouseArea{
                id:areaBottom
                x:mouseAdjustWidth1
                y:parent.height - mouseAdjustWidth1
                width: parent.width - mouseAdjustWidth1*2
                height: mouseAdjustWidth1
                hoverEnabled: true

                onEntered: cursorShape = Qt.SizeVerCursor
                onExited: cursorShape = Qt.ArrowCursor
                onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                onPositionChanged: {

                    if(pressed){
                        var offsetX = mouse.x - mousePressPt1.x
                        var offsetY = mouse.y - mousePressPt1.y
                        adjustWindow(wBOTTOM,0,offsetY);
                    }
                    //setDlgPoint(offset.x, 0)
                }
            }

            MouseArea{
                id:areaCenter
                x:mouseAdjustWidth1
                y:mouseAdjustWidth1
                width: parent.width - mouseAdjustWidth1*2
                height: parent.height - mouseAdjustWidth1*2
                hoverEnabled: true
                onPressed:  mousePressPt1  = Qt.point(mouse.x, mouse.y)
                onPositionChanged: {

                    if(pressed){
                        var offsetX = mouse.x - mousePressPt1.x
                        var offsetY = mouse.y - mousePressPt1.y
                        adjustWindow(wCenter,offsetX,offsetY);
                    }
                    //setDlgPoint(offset.x, 0)

                }
            }

            Rectangle{
                id:rectEnsure
                anchors.right: rectadmjt.right
                anchors.top:rectadmjt.top
                width: 20
                height: 10
                color: "transparent"
                Image{
                    id:imgEnsure
                    width: 12
                    height: 24
                    source: "qrc:/images/adjust_ensure.png"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {

                            video.fun_setRectPar(rectadmjt.x,rectadmjt.y,rectadmjt.width,rectadmjt.height,video.width,video.height)
                            //rectadmjt.visible = false

                            deviceconfig.setRedRect(video.width,video.height,rectadmjt.x,rectadmjt.y,rectadmjt.width,rectadmjt.height)
                            //deviceconfig.setIsOpenAdjustRect(false)
                        }
                    }
                }
                Image{
                    id:imgCancel
                    width: 12
                    height: 12
                    anchors.top: imgEnsure.bottom
                    anchors.left: imgEnsure.left
                    source: "qrc:/images/adjust_cancel.png"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            rectadmjt.x = deviceconfig.getShowRectX()
                            rectadmjt.y = deviceconfig.getShowRectY()
                            rectadmjt.width = deviceconfig.getShowRectW()
                            rectadmjt.height = deviceconfig.getShowRectH()
                        }
                    }
                }
            }
        }
    }


    Connections{
        target: deviceconfig
        onS_timeSwith:video.fun_timeSwitch(mchecked);//时间使能
        onS_temSet:video.fun_temSet(mvalue);//警报温度设置
        onS_deviceUpdate:{

            var map ={
                cmd:"update"
            }
            video.fun_sendCommonPar(map)
        }
        onS_beerSwith:{
            var audioEnable = 0;
            if(mchecked)
                audioEnable = 1;
            var map ={
                cmd:"setalarmparam",
                alarmaudiooutenabled:audioEnable
            }
            video.fun_sendCommonPar(map)
        }

        onS_sendcommoncmd:video.fun_sendCommonPar(mvalue);

        onS_warnSwith:{
            var map ={
                cmd:"",
                isSubscription:false
            }
            map.isSubscription=mchecked
            if(mchecked)
                map.cmd="alarmsubscription"
            else
                map.cmd="unalarmsubscription"
            video.fun_sendCommonPar(map)
        }
        onS_temMin:video.fun_temMin(mvalue);//温度控制阀
        onS_temOffset:video.fun_temOffset(mvalue);//温漂


    }
    //    Connections{
    //        target: videoTemp
    //        onS_sendList:video.fun_setListRect(vmap)
    //    }

    Connections{
        target: homeMenu
        onS_startSearchDevice:{
            video.funStartSearch();
        }
        onS_connectDevice:{

            video.startNormalVideo(deviceconfig.getWarnTem(),str)
        }
    }

    function funsetlistRect(map){
        video.fun_setListRect(map)
    }

    function funinitRedFrame(w,h){
        video.fun_initRedFrame(w,h)
    }

    function httpParCallback(smap){

        var strcmd = smap.cmd;
        console.debug("strcmd:"+strcmd)

        if(strcmd === "getosdparam"){
            var enable = smap.enable;

        }else if(strcmd === "getrecordparam"){

        }else if(strcmd === "deviceinfo"){
            //deviceconfig.setTcpip(smap.ip)
            homeMenu.addDeviceInfo(smap.uuid)
        }else if(strcmd === "getinftempmodel"){

            console.debug(" **************** "+smap.tempmodel)
            var map ={
                cmd:""
            }
            //var enable = smap.timeenable;
            if(smap.tempmodel === "D04")
                deviceconfig.curDevTypeStr = "d04"
            else if(smap.tempmodel === "D06")
                deviceconfig.curDevTypeStr = "d06"
            else if(smap.tempmodel === "E03")
                deviceconfig.curDevTypeStr = "e03"
            else if(smap.tempmodel === "F03")
                deviceconfig.curDevTypeStr = "f03"
            else if(smap.tempmodel === "J07-S" || smap.tempmodel === "J07"){
                deviceconfig.curDevTypeStr = "J07-S"

                if(deviceconfig.getSwitchWarn()){
                    var map1 ={
                        cmd:"alarmsubscription",
                        isSubscription:true
                    }
                    video.fun_sendCommonPar(map1)
                }
            }

            map.cmd = "setcurrenttime"
            video.fun_sendCommonPar(map);

            video.fun_setInitPar(deviceconfig.getTcpip(),deviceconfig.getShowParentW(),deviceconfig.getShowParentH(),deviceconfig.getShowRectX(),deviceconfig.getShowRectY(),deviceconfig.getShowRectW(),deviceconfig.getShowRectH())

            s_tempmodelSelect(smap.tempmodel,deviceconfig.getTcpip());
        }else if(strcmd === "getiradinfo"){
            var alarmtempEnable = smap.alarmtempEnable;
            var alarmTemp = smap.alarmTemp;
            var tempdriftcaplevelMin = smap.tempdriftcaplevelMin;
            var tempdriftcaplevelMax = smap.tempdriftcaplevelMax;
            var tempcontrolcaplevelMin = smap.tempcontrolcaplevelMin;
            var tempcontrolcaplevelMax = smap.tempcontrolcaplevelMax;
            var tempdrift = smap.tempdrift;
            var tempcontrol = smap.tempcontrol;
            var osdenable = smap.osdenable;
            var tempdisplay = smap.tempdisplay;

            deviceconfig.setSwitchTempdisplay(tempdisplay);
            deviceconfig.tempcontrolcapMax = tempcontrolcaplevelMax
            deviceconfig.tempcontrolcapMin = tempcontrolcaplevelMin;
            deviceconfig.tempdriftcapMax = tempdriftcaplevelMax;
            deviceconfig.tempdriftcapMin = tempdriftcaplevelMin;
            deviceconfig.setTemDrift(tempdrift)
            deviceconfig.setWarnTem(alarmTemp.toFixed(2))
            deviceconfig.setSwitchTime(osdenable)
            deviceconfig.setTempContrl(tempcontrol)
            deviceconfig.setSwitchWarn(alarmtempEnable)


        }else if(strcmd === "pushalarm"){
            startWarn(smap.temperature);
        }else if(strcmd === "update"){
            deviceconfig.updateDevice(smap.did,smap.url)
            // updateprogress.startupLoad(smap.did,smap.url,deviceconfig.getUpdateFilePath)
        }else if("getiradrect" === strcmd){
            s_testRect(smap.x0,smap.y0,smap.w0,smap.h0,smap.x1,smap.y1,smap.w1,smap.h1,smap.x2,smap.y2,smap.w2,smap.h2);
        }else if("alarmsubscription" === strcmd){



        }else if("getalarmparam" === strcmd){
            var beerenable = smap.alarmaudiooutenabled;
            deviceconfig.setSwitchBeer(beerenable)
        }else if("getinftempcolor" === strcmd){
            deviceconfig.setTempcolor(smap.tempcolor);
        }else if("getimagparam" === strcmd){
            deviceconfig.imagparamflip = smap.flip
            deviceconfig.imagparambrightness = smap.brightness
            deviceconfig.imagparamcolorsaturation =smap.colorsaturation
            deviceconfig.imagparamcontrast = smap.contrast
            deviceconfig.imagparamhue = smap.hue
            deviceconfig.imagparammirror = smap.mirror
            deviceconfig.imagparamsharpness = smap.sharpness
            deviceconfig.imagparamwdr = smap.wdr
            deviceconfig.setWdr(smap.wdr);

        }else if("setimagparam" === strcmd){
            if(deviceconfig.wdrisChange){
                msgdialog.width = 500
                msgdialog.height = 176
                var txtstr ;
                switch(curLanguage)
                {
                case lChinese:
                    txtstr = "wdr设置成功，设备正在重启"
                    break;
                case lEnglish:
                    txtstr = "The wdr setting is successful, the Camera is restarting"
                    break;
                case lItaly:
                    txtstr = "La fotocamera si riavvierà dopo l'impostazione"
                    break;
                case lKorean:
                     txtstr = "설정 후 카메라가 다시 시작됩니다"
                    break;
                case lRussian:
                     txtstr = "Камера перезагрузится после настройки"
                    break;
                case lLithuanian:
                     txtstr = "Nustačius fotoaparatą, jis paleis iš naujo"
                    break;
                case ltuerqi:
                     txtstr = "Kamera ayarlandıktan sonra yeniden başlatılacak"
                    break;
                case ltuerqi1:
                     txtstr = "Kamera ayarlandıktan sonra yeniden başlatılacak"
                    break;
                case lputaoya:
                     txtstr = "A câmera será reiniciada após a configuração"
                    break;
                case lxibanya:
                     txtstr = "La cámara se reiniciará después de configurar"
                    break;
                case lfayu:
                     txtstr = "La caméra redémarrera après le réglage"
                    break;
                case lniboer:
                     txtstr = "क्यामेरा सेटिंग पछि रिस्टार्ट हुनेछ"
                    break;
                case lKhmer:
                     txtstr = "កាមេរ៉ានឹងចាប់ផ្តើមឡើងវិញបន្ទាប់ពីកំណត់"
                    break;

                }

                msgdialog.msgStr = txtstr;
                msgdialog.open();
            }
        }
    }

    function updateDate(){
        video.fun_updateDate();
    }

    function adjustWindow(adjustw,dX,dY)
    {
        var dx = 0;
        var dy = 0;
        var dw = 0;
        var dh = 0;
        if(adjustw === wLEFT){
            rectadmjt.x = rectadmjt.x + dX;
            rectadmjt.width = rectadmjt.width - dX;
        }else if(adjustw === wRIGHT){

            rectadmjt.width = rectadmjt.width + dX;

        }else if(adjustw === wTOP){
            rectadmjt.y = rectadmjt.y + dY;
            rectadmjt.height = rectadmjt.height - dY;

        }else if(adjustw === wBOTTOM){
            console.debug( "*****   " + dY)
            rectadmjt.height = rectadmjt.height + dY;
        }else if(adjustw === wRIGHTTOP){

            rectadmjt.x = rectadmjt.x - dX;
            rectadmjt.width = rectadmjt.width + dX;

            rectadmjt.y = rectadmjt.y + dY;
            rectadmjt.height = rectadmjt.height - dY;

        }else if(adjustw === wLEFTTOP){

            rectadmjt.x = rectadmjt.x + dX;
            rectadmjt.width = rectadmjt.width - dX;

            rectadmjt.y = rectadmjt.y + dY;
            rectadmjt.height = rectadmjt.height - dY;

        }else if(adjustw === wLEFTBOTTOM){
            rectadmjt.x = rectadmjt.x + dX;
            rectadmjt.width = rectadmjt.width - dX;

            rectadmjt.height = rectadmjt.height + dY;

        }else if(adjustw === wRIGHTBOTTOM){

            rectadmjt.width = rectadmjt.width + dX;

            rectadmjt.height = rectadmjt.height + dY;
        }else if(wCenter === adjustw){

            rectadmjt.x = rectadmjt.x + dX;
            rectadmjt.y = rectadmjt.y + dY;
        }
        //video.fun_setRectPar(rectadmjt.x,rectadmjt.y,rectadmjt.width,rectadmjt.height,video.width,video.height)
        console.debug("矩形位置："+rectadmjt.x+" "+rectadmjt.y+" "+rectadmjt.width+" "+rectadmjt.height+" "+video.width+" "+video.height);
    }

}



