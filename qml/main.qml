import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Window 2.12
import QtQml 2.12



//import DeviceManagerment 1.0
Window {

    id: main;
    flags:Qt.FramelessWindowHint |
          Qt.WindowMinimizeButtonHint |
          Qt.Window
    property bool isMainContent: true
    property bool isSpecilState: false      //窗口在最大化的时候调用最小化 会出现特例（窗口还原后大小不再是最大化了）
    visible: true

    width:1000
    height:800

    visibility : "Windowed"

    property int mouseAdjustWidth: 10
    property int minW: 960
    property int minH: 600
    property string toastStr: ""

//    ListModel{
//        id:listdeviceInfo
//        /*
//            devicename:
//            devicedid:
//            devicetype:
//            onlinestate:
//        */
//        function findDeviceByName( name){

//            for(var i=0;i<listdeviceInfo.count;i++){
//                var map = listdeviceInfo.get(i);
//                if(name === map.devicename){
//                    return map

//                }
//            }
//            return null;
//        }
//    }


    onVisibilityChanged: {
        if(isSpecilState){
            if(main.visibility === 2){
                main.visibility = "Maximized"
                isSpecilState = false;
            }
        }
    }
    MainContent{
        id:maincontent
        width: parent.width
        height: parent.height
        visible: true//isMainContent?true:false

        onWinMin: {
            if(main.visibility === 4)
                isSpecilState = true;

            main.visibility = "Minimized"
        }
        onWinMax: {
            if(main.visibility === 2)
                main.visibility = "Maximized"

            else if(main.visibility === 4)
                main.visibility = "Windowed"
        }
        onWinClose:Qt.quit();
        onDragPosChange:main.setDlgPoint(mx,my);

        //        QmlWatingBusy{
        //            id:busyWait
        //            anchors.centerIn: parent
        //            width: 100
        //            height: 100
        //        }
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
            //setDlgPoint(offset.x, 0)

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

    //    function saveWindowState(tChange)
    //    {
    //        mWindowChange = tChange
    //        preX = main.x;
    //        preY = main.y
    //        preWidth = main.width
    //        preHeight = main.height
    //    }

    //    function recoveryWindowState()
    //    {
    //        main.x = preX;
    //        main.y = preY;
    //        main.width = preWidth
    //        main.height = preHeight
    //        mWindowChange = 0;
    //    }
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
