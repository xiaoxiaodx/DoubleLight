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

    property string toastStr: ""


    ListModel{
        id:listdeviceInfo
        /*
            devicename:
            devicedid:
            devicetype:
            onlinestate:
        */
        function findDeviceByName( name){

            for(var i=0;i<listdeviceInfo.count;i++){
                var map = listdeviceInfo.get(i);
                 if(name === map.devicename){
                    return map

                 }
            }
            return null;
        }
    }

    MainContent{
        id:maincontent
        width: parent.width
        height: parent.height
        visible: isMainContent?true:false

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


    onVisibilityChanged: {
        if(isSpecilState){
            if(main.visibility === 2){
                main.visibility = "Maximized"
                isSpecilState = false;
            }
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

    function saveWindowState(tChange)
    {
        mWindowChange = tChange
        preX = main.x;
        preY = main.y
        preWidth = main.width
        preHeight = main.height
    }

    function recoveryWindowState()
    {
        main.x = preX;
        main.y = preY;
        main.width = preWidth
        main.height = preHeight
        mWindowChange = 0;
    }
    function setDlgPoint(dlgX ,dlgY)
    {
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



}
