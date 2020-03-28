import QtQuick 2.0
import "../qml/simpleControl"

Rectangle {


    property alias mCurIndex: tabbarBtn.curIndex


    Image {
        id: btnImg

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 40
        anchors.topMargin: 14
        width: 148
        height: 41
        source: "qrc:/images/logo.png"

    }

    QmlTabBarButtonH{
        id:tabbarBtn
        height: parent.height
        anchors.left: btnImg.right
        anchors.leftMargin: 60
        btnBgColor:"transparent"
        btnBgSelectColor:"#272727"
        mflagColor:"white"
        textColor: "white"
        textSelectColor:"white"
        txtLeftMargin:7
        textSize:18
        Component.onCompleted: {

            tabbarBtn.barModel.append({txtStr:qsTr("主预览")})//,imgSrc:"qrc:/images/homemenuClose.png",imgSrcEnter:"qrc:/images/homemenuClose.png"})
            tabbarBtn.barModel.append({txtStr:qsTr("录像回放")})//,imgSrc:"qrc:/images/homemenuClose.png",imgSrcEnter:"qrc:/images/homemenuClose.png"})
            tabbarBtn.barModel.append({txtStr:qsTr("设备配置")})//,imgSrc:"qrc:/images/homemenuClose.png",imgSrcEnter:"qrc:/images/homemenuClose.png"})

        }

    }


}
