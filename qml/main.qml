import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Window 2.12
import QtQml 2.12

import "./dialog"


Window {

    id: main;
    flags:Qt.FramelessWindowHint |
          Qt.WindowMinimizeButtonHint |
          Qt.Window
    visible: true

    width:1400
    height:900

    property bool isLocker: false
    visibility : "AutomaticVisibility"

    property int mouseAdjustWidth: 10
    property int minW: 1400
    property int minH: 900

    property string toastStr: ""
    signal lockerCHange(bool lockchange);
    onIsLockerChanged: lockerCHange(isLocker)

    property int restoreX: 0
    property int restoreY: 0
    property int restoreW: 0
    property int restoreH: 0

    property int windowSizeState: 1 //1：正常，0最小化，2最大化

    property int curLanguage: 1
    property int lChinese: 0
    property int lEnglish: 1
    property int lItaly: 2
    property int lKorean: 3
    property int lRussian: 4
    property int lLithuanian: 5


    signal s_setLanguage(var typeL);

    Component.onCompleted: s_setLanguage(curLanguage)
    onVisibilityChanged: {

        console.debug("visibly change "+visibility)
    }


    ListModel{
        id:pushwarnmodel
        property int numberOfPeople : 0
        property int numberOfNormalPeople: 0
        property int numberOfAbnormalTemperaturePeople: 0
        property int numberOfNoMaskPeople: 0
    }
    MainContent{
        id:maincontent
        width: parent.width
        height: parent.height
        visible: true//isMainContent?true:false

        onWinMin: {
//            if(main.visibility === 4)
//                isSpecilState = true;
            main.visibility = "Minimized"
        }
        onWinMax: {
            if(windowSizeState === 1)
                maxWindow()
            else if(windowSizeState === 2)
                restoreWindow();
        }
        onWinClose:{
            askDialog.width = 427
            askDialog.height = 176
            askDialog.askStr = curLanguage=== lChinese?"确认退出系统吗？":
                               curLanguage===lEnglish?"Confirm to exit ?":
                               curLanguage===lKorean?"나가시겠습니까?":
                               curLanguage===lItaly?"Uscita Dal Sistema?":
                               curLanguage===lRussian?"Подтвердить выход?":
                               curLanguage===lLithuanian?"Patvirtinti išėjimą?":"";

            askDialog.imgSrc = "qrc:/images/icon_question.png"
            askDialog.curType = askDialog.exeClose
            askDialog.open();
          //  Qt.quit();
        }
        onDragPosChange:main.setDlgPoint(mx,my);

        //        QmlWatingBusy{
        //            id:busyWait
        //            anchors.centerIn: parent
        //            width: 100
        //            height: 100
        //        }
    }

    Rectangle{
        color: "#00000000"
        width: parent.width
        height: parent.height
        visible: isLocker
        MouseArea{
            anchors.fill: parent
            propagateComposedEvents: true
            onClicked: {
                if(!isLocker)
                    mouse.accepted = false;
                isLocker = false;


            }
        }
    }
    property point mousePressPt: "0,0"
    MouseArea{
        id:areaLeft
        x:0
        y:mouseAdjustWidth
        width: mouseAdjustWidth
        height: parent.height - mouseAdjustWidth
        hoverEnabled: true

        onEntered: cursorShape = Qt.SizeHorCursor
        onExited: cursorShape = Qt.ArrowCursor
        onPressed:  mousePressPt  = Qt.point(mouse.x, mouse.y)
        onPositionChanged: {

            if(pressed){
                var offsetX = mouse.x - mousePressPt.x
                var offsetY = mouse.y - mousePressPt.y
                adjustWindow(MainContent.ADJUSTW.WLEFT,offsetX,0);
            }
        }

    }

    MouseArea{
        id:areaRight
        x:parent.width - mouseAdjustWidth
        y:0
        width: mouseAdjustWidth
        height: parent.height - mouseAdjustWidth
        hoverEnabled: true
        onEntered: cursorShape = Qt.SizeHorCursor
        onExited: cursorShape = Qt.ArrowCursor
        onPressed:  mousePressPt  = Qt.point(mouse.x, mouse.y)
        onPositionChanged: {


            if(pressed){
                var offsetX = mouse.x + mousePressPt.x
                var offsetY = mouse.y + mousePressPt.y
                adjustWindow(MainContent.ADJUSTW.WRIGHT,offsetX,0);
            }

        }
    }

    MouseArea{
        id:areaLeftBottom
        x:0
        y:parent.height - mouseAdjustWidth
        width: mouseAdjustWidth
        height: mouseAdjustWidth
        hoverEnabled: true

        onEntered: cursorShape = Qt.SizeBDiagCursor
        onExited: cursorShape = Qt.ArrowCursor
        onPressed:  mousePressPt  = Qt.point(mouse.x, mouse.y)
        onPositionChanged: {

            if(pressed){
                var offsetX = mouse.x - mousePressPt.x
                var offsetY = mouse.y - mousePressPt.y
                adjustWindow(MainContent.ADJUSTW.WLEFTBOTTOM,offsetX,offsetY);
            }
            //setDlgPoint(offset.x, 0)
        }
    }

    MouseArea{
        id:areaRightBottom
        x:main.width - mouseAdjustWidth
        y:main.height - mouseAdjustWidth
        width: mouseAdjustWidth
        height: mouseAdjustWidth
        hoverEnabled: true

        onEntered: cursorShape = Qt.SizeFDiagCursor
        onExited: cursorShape = Qt.ArrowCursor
        onPressed:  mousePressPt  = Qt.point(mouse.x, mouse.y)
        onPositionChanged: {

            if(pressed){
                var offsetX = mouse.x - mousePressPt.x
                var offsetY = mouse.y - mousePressPt.y
                adjustWindow(MainContent.ADJUSTW.WRIGHTBOTTOM,offsetX,offsetY);
            }
        }
    }

    MouseArea{
        id:areaBottom
        x:mouseAdjustWidth
        y:parent.height - mouseAdjustWidth
        width: parent.width - mouseAdjustWidth*2
        height: mouseAdjustWidth
        hoverEnabled: true

        onEntered: cursorShape = Qt.SizeVerCursor
        onExited: cursorShape = Qt.ArrowCursor
        onPressed:  mousePressPt  = Qt.point(mouse.x, mouse.y)
        onPositionChanged: {

            if(pressed){
                var offsetX = mouse.x - mousePressPt.x
                var offsetY = mouse.y - mousePressPt.y
                adjustWindow(MainContent.ADJUSTW.WBOTTOM,0,offsetY);
            }
            //setDlgPoint(offset.x, 0)

        }
    }

    AskDialog{
        id:askDialog

        onS_CurTypeMsg: {

            if(askDialog.exeClose === type)
                Qt.quit();
        }

    }

    MsgDialog{
        id:msgdialog
    }

    Loader{
        id:loaderToast
        z:10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom

        sourceComponent: null
    }

    Component {
        id: toast
        QmlToast{
            txtStr:toastStr
            backColor: "transparent"
            txtColor:"#ffffff"
            maxWid:main.width/2
        }
    }


    function showToast(stri){
        toastStr = stri;
        loaderToast.sourceComponent = null;
        loaderToast.sourceComponent = toast
    }

    function setDlgPoint(dlgX ,dlgY)
    {
        //console.debug("dx dy    "+dlgX+"    "+dlgY)
        //设置窗口拖拽不能超过父窗口
        if(main.x + dlgX < 0)
        {
            main.x = 0
        }
        else if(main.x + dlgX > Screen.desktopAvailableWidth - main.width)
        {
            main.x = Screen.desktopAvailableWidth - main.width
        }
        else
        {
            main.x = main.x + dlgX
        }
        if(main.y + dlgY < 0)
        {
            main.y = 0
        }
        else if(main.y + dlgY >  Screen.desktopAvailableHeight - main.height)
        {
            main.y =  Screen.desktopAvailableHeight - main.height
        }
        else
        {
            main.y = main.y + dlgY
        }
    }

    function maxWindow(){
        restoreX = main.x
        restoreY = main.y
        restoreW = main.width
        restoreH = main.height

        main.x = 0;
        main.y = 0;
        main.width = Screen.desktopAvailableWidth
        main.height = Screen.desktopAvailableHeight
        windowSizeState = 2;
    }

    function restoreWindow(){
        main.x = restoreX;
        main.y = restoreY;
        main.width = restoreW
        main.height = restoreH
        windowSizeState = 1
    }

    function adjustWindow(adjustw,dX,dY)
    {
        var dx = 0;
        var dy = 0;
        var dw = 0;
        var dh = 0;
        //        if(adjustw === MainContent.ADJUSTW.WLEFT){
        //            main.x = main.x + dX;
        //            main.width = main.width - dX;
        //        }else if(adjustw === MainContent.ADJUSTW.WRIGHT){

        //            main.width = main.width + dX;

        //        }else if(adjustw === MainContent.ADJUSTW.WTOP){

        //        }else if(adjustw === MainContent.ADJUSTW.WBOTTOM){
        //            main.height = main.height + dY;
        //        }else if(adjustw === MainContent.ADJUSTW.WRIGHTTOP){

        //        }else if(adjustw === MainContent.ADJUSTW.WLEFTTOP){

        //        }else if(adjustw === MainContent.ADJUSTW.WLEFTBOTTOM){
        //            main.x = main.x + dX;
        //            main.width = main.width - dX;

        //            main.height = main.height + dY;

        //        }else if(adjustw === MainContent.ADJUSTW.WRIGHTBOTTOM){

        //            main.width = main.width + dX;

        //            main.height = main.height + dY;
        //        }
        if(adjustw === MainContent.ADJUSTW.WLEFT){
            dx =  dX;
            dw = - dX;
        }else if(adjustw === MainContent.ADJUSTW.WRIGHT){

            dw =  dX;

        }else if(adjustw === MainContent.ADJUSTW.WTOP){

        }else if(adjustw === MainContent.ADJUSTW.WBOTTOM){
            dh = dY;
        }else if(adjustw === MainContent.ADJUSTW.WRIGHTTOP){

        }else if(adjustw === MainContent.ADJUSTW.WLEFTTOP){

        }else if(adjustw === MainContent.ADJUSTW.WLEFTBOTTOM){
            dx =  dX;
            dw =  - dX;
            dh =  dY;
        }else if(adjustw === MainContent.ADJUSTW.WRIGHTBOTTOM){

            dw = dX;
            dh = dY;
        }

        if(main.width + dw <= minW)
            main.width = minW;
        else{
            main.width = main.width + dw;
            main.x = main.x + dx;
        }

        if(main.height + dh <= minH)
            main.height = minH
        else{
            main.height = main.height + dh;
            main.y = main.y + dy;
        }
    }
}
