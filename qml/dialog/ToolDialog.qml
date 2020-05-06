import QtQuick 2.0
import QtQuick.Controls 2.5
import "../simpleControl"
import QtGraphicalEffects 1.12

Popup {
    id: root
    x: parent.width/2 - root.width/2
    y: parent.height/2 - root.height/2
    modal: true
    focus: true
    //设置窗口关闭方式为按“Esc”键关闭
    closePolicy: Popup.OnEscape
    //设置窗口的背景控件，不设置的话Popup的边框会显示出来
    background: rect

    property int curType: -1;
    //signal s_deviceIDstr(var name,var strID,var strAccoount,var strPassword)
    signal s_showToast(var str1)
    signal s_CurTypeMsg(var type);

    property string imgSrc:""
    property int exeClose: 0
    property int warnInfoSingleDelete: 1
    property int warnInfoMutipleDelete: 2

    Rectangle {
        id: rect
        anchors.fill: parent
        color: "#ffffff"
        radius: 3
        //设置标题栏区域为拖拽区域
        Rectangle{
            id:rectdid
            width: parent.width
            height: 40

            Text{
                id: txtDid
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("text")
            }

            Button{
                id:btnWriteDid
                text: "烧写"
                anchors.left: txtDid.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: txtWritedDid
                anchors.left: btnWriteDid.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("text")
            }
        }

        Rectangle{
            id:rectSetModel
            width: parent.width
            height: 40
            anchors.left: rectdid.left
            anchors.top: rectdid.bottom
            anchors.topMargin: 10
            LineEdit {
                id: inputTempDrift
                width: rectSetModel.width  - 22
                height: rectSetModel.height -2
                anchors.left: rectdid.left
                anchors.verticalCenter: rectSetModel.verticalCenter
                border.width: 0
                inputLimite:Qt.ImhDigitsOnly
                font.pixelSize: fontSize
                placeholderText: ""
                isNeedDoubleClickEdit: false
                textLeftPadding:0
                txtColor: Qt.rgba(0,0,0,0.65)
                text: "0"
                color: "#F8FAFD"
                isReadOnly:true
                onTextChanged: {
                    s_temOffset(inputTempDrift.text);

                }
            }

            Button{
                id:btnSetModel
                anchors.left: rectdid.left
                anchors.verticalCenter: inputTempDrift.verticalCenter
                text: "设置型号"
                onClicked: {
                }
            }

            Text {
                id: txtSetModel
                anchors.left: btnSetModel.right
                anchors.leftMargin: 10
                anchors.verticalCenter: btnSetModel.verticalCenter
                text: qsTr("")
            }
        }

        Rectangle{
            id:faceRect
            width: parent.width
            height: 40
            anchors.left: rectdid.left
            anchors.top: rectSetModel.bottom
            anchors.topMargin: 10

            Text {
                id: txtKeyID
                text: qsTr("")
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }

            Button{
                id:btnWriteLicense
                anchors.left: txtKeyID.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: "烧写licence"
                onClicked: {}
            }

            Text {
                id: txtLicenseInfo
                anchors.left: btnWriteLicense.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("")
            }

        }



        Rectangle{
            id:rectSdCard
            width: parent.width
            height: 40
            anchors.left: rectdid.left
            anchors.top: faceRect.bottom
            anchors.topMargin: 10
            Text {
                id: txtsdinfo
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("text")
            }

            Button{
                id:btnGetSd
                text: "获取sdcard信息"
            }
        }





        Rectangle{
            id:btnEnsure
            width: txtEnsure.width +24
            height: 34
            border.width: 1
            border.color: "#3B84F6"
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: 40
            anchors.bottomMargin: 40
            Text {
                id: txtEnsure
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideMiddle
                font.pixelSize: 14
                color: "#3B84F6"
                text: qsTr("确定")
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {

                    if(curType > -1){
                        s_CurTypeMsg(curType)
                    }
                    root.close()
                }
            }
        }

        Rectangle{
            id:btnCancel
            width: txtCancel.width + 24
            height: 34
            anchors.bottom: btnEnsure.bottom
            anchors.right: btnEnsure.left
            anchors.rightMargin: 16
            border.width: 1
            border.color: "#909399"
            Text {
                id: txtCancel
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideMiddle
                font.pixelSize: 14
                color: "#909399"
                text: qsTr("取消")
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {

                    root.close()
                }
            }
        }


        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            horizontalOffset: 4
            verticalOffset: 4
            color:"#80000000"
        }
    }




}

