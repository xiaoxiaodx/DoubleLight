import QtQuick 2.0
import "../qml/simpleControl"

Rectangle {


    property alias mCurIndex: tabbarBtn.curIndex

    signal swinMin();
    signal swinMax();
    signal swinClose();


    Rectangle{
        width: parent.width - rectLanguage.width -20
        height: parent.height
        color: "#00ffffff"
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
                //tabbarBtn.barModel.append({txtStr:qsTr("录像回放")})//,imgSrc:"qrc:/images/homemenuClose.png",imgSrcEnter:"qrc:/images/homemenuClose.png"})
                tabbarBtn.barModel.append({txtStr:qsTr("设备配置")})//,imgSrc:"qrc:/images/homemenuClose.png",imgSrcEnter:"qrc:/images/homemenuClose.png"})
                tabbarBtn.barModel.append({txtStr:qsTr("告警管理")})//,imgSrc:"qrc:/images/homemenuClose.png",imgSrcEnter:"qrc:/images/homemenuClose.png"})

            }

        }

        MouseArea{
            property point clickPoint: "0,0"

            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            propagateComposedEvents: true
            onPressed: {
                homeMenu.isDoubleClick = false;
                clickPoint  = Qt.point(mouse.x, mouse.y)
                //mouse.accepted = false;
            }

            onDoubleClicked: {
                homeMenu.isDoubleClick = true
                winMax();
            }
            onPositionChanged: {

                if(!homeMenu.isDoubleClick){
                    var offset = Qt.point(mouse.x - clickPoint.x, mouse.y - clickPoint.y)

                    dragPosChange(offset.x, offset.y)
                }
            }
        }
    }

    Rectangle{
        id:rectLanguage
        width: imgl.width + cmb.width +line.width+10
        height: parent.height
        color: "#00ffffff"
        anchors.right: windowAdjust.left
        anchors.rightMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        Image {
            id: imgl
            width: 20
            height: 20
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/images/language.png"
        }
        MyComBox{
            id:cmb
            width:110
            height: 20
            anchors.left: imgl.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            contentBg: "#00FFFFFF"
            itemColorBgNor:"#ffffff"
            itemColorBgHoverd: "#E7EAF1"
            indicatorImgSrc:"qrc:/images/language_down.png"
            indicatorW: 11
            indicatorH: 7
            itemLeftMargin:-12
            itemTopMargin:0

            model: ListModel{
                ListElement{showStr:"简体中文"}
                ListElement{showStr:"English"}
                ListElement{showStr:"Italian"}
                ListElement{showStr:"Korean"}
            }
            //{["showStr":"简体中文" "showStr":"English","Italian","Korean"]}
            onCurrentIndexChanged: {
                curLanguage = currentIndex
                main.s_setLanguage(currentIndex);
            }
        }
        Rectangle{
            id:line
            width: 2
            height: 18
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Row{
        id:windowAdjust
        anchors.right: parent.right
        anchors.rightMargin: 10
        width: 100
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


    Connections{
        target: main
        onS_setLanguage:setLanguage(typeL);
    }

    function setLanguage(type){

        var index = 0;
        switch(type){
        case lEnglish:
            tabbarBtn.barModel.get(index++).txtStr =  "Main Preview"
            tabbarBtn.barModel.get(index++).txtStr =  "Device Settings"
            tabbarBtn.barModel.get(index++).txtStr =  "Alarm Management"
            break;
            break;
        case lKorean:
            tabbarBtn.barModel.get(index++).txtStr =  "미리보기"
            tabbarBtn.barModel.get(index++).txtStr =  "장비설정"
            tabbarBtn.barModel.get(index++).txtStr =  "알람관리"
            break;
            break;
        case lItaly:
            tabbarBtn.barModel.get(index++).txtStr =  "Vista Principale"
            tabbarBtn.barModel.get(index++).txtStr =  "Settaggio"
            tabbarBtn.barModel.get(index++).txtStr =  "Gestione Allarmi"
            break;
            break;
        case lChinese:
            tabbarBtn.barModel.get(index++).txtStr =  "主预览"
            tabbarBtn.barModel.get(index++).txtStr =  "设备配置"
            tabbarBtn.barModel.get(index++).txtStr =  "告警管理"
            break;
        }
    }


}
