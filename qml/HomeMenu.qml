import QtQuick 2.0
import "../qml/simpleControl"

Rectangle {


    property alias mCurIndex: tabbarBtn.curIndex

    signal swinMin();
    signal swinMax();
    signal swinClose();

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

    Row{
        id:windowAdjust
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        spacing:20
        QmlImageButton{
            width: 20
            height: 20
            imgSourseHover: "qrc:/images/win_min_P.png"
            imgSourseNormal: "qrc:/images/win_min.png"
            imgSoursePress: "qrc:/images/win_min_P.png"
            onClick:swinMin()
        }
        QmlImageButton{
            width: 20
            height: 20
            imgSourseHover: "qrc:/images/win_max_p.png"
            imgSourseNormal: "qrc:/images/win_max.png"
            imgSoursePress: "qrc:/images/win_max_p.png"
            onClick:swinMax()

        }
        QmlImageButton{
            width: 20
            height: 20
            imgSourseHover: "qrc:/images/win_close_p.png"
            imgSourseNormal: "qrc:/images/win_close.png"
            imgSoursePress: "qrc:/images/win_close_p.png"
            onClick:swinClose()


        }

    }


}
