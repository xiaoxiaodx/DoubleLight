import QtQuick 2.0
import QtQuick.Controls 2.5
import "../simpleControl"
import QtGraphicalEffects 1.12
import QtQuick.Dialogs 1.3

import ToolUtil 1.0
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

    signal s_setdid(var map)
    signal s_setinftempmodel(var map)
    signal s_setinftemptype(var map)
    signal s_getdevicekey(var map)
    signal s_setsignature(var map)
    signal s_getsdcardparam(var map)
    signal s_setsdcardformat(var map)

    property int rectW: 330

    ToolUtil{
        id:toolutil

        onSignal_setDidInfo:{
            txtDid.text = did
            txtlisence.text = p2pkey
            txtPublish.text = pushKey
        }

        onSignal_sendLisence:{//lisence

             var map = {
                 cmd:"setsignature",
                 signature:lisence
             }
             s_setsignature(map)
        }
    }
    Rectangle {
        id: rect
        anchors.fill: parent
        color: "#ffffff"
        radius: 3
        //设置标题栏区域为拖拽区域
        Rectangle{
            id:rectdid
            width:rectW
            height: 160
            border.width: 1
            border.color: "red"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 10
            Column{

                spacing: 5
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 10
                Rectangle{

                    width: 100
                    height: 40
                    Button{
                        id:btnImportFile
                        text: "导入did文件"
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: fileDialog.open()
                    }

                    Button{
                        id:btnWriteDid
                        text: "烧写"
                        anchors.left: btnImportFile.right
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter

                        onClicked: {
                            var map = {
                                cmd:"setdid",
                                uuid:txtDid.text,
                                lisence:txtlisence.text,
                                pushlis:txtPublish.text
                            }
                            s_setdid(map)
                            writeDidTip.text = ""
                        }
                    }

                    Text{
                        id:writeDidTip
                        anchors.left: btnWriteDid.right
                        anchors.verticalCenter: btnWriteDid.verticalCenter
                        anchors.leftMargin: 10
                    }
                }

                Rectangle{

                    height: lableDid.height
                    width: lableDid.width + txtDid.width+10
                    Text{
                        id: lableDid
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("    did:")
                    }
                    Text{
                        id: txtDid
                        anchors.left: lableDid.right
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("")
                    }
                }

                Rectangle{

                    height: labllisence.height
                    width: labllisence.width + txtlisence.width + 10
                    Text{
                        id: labllisence
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("p2p key:")
                    }
                    Text{
                        id: txtlisence
                        anchors.left: labllisence.right
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("")
                    }
                }

                Rectangle{

                    height: lablePublish.height
                    width: lablePublish.width + txtPublish.width+10
                    Text{
                        id: lablePublish
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("publish key:")
                    }
                    Text{
                        id: txtPublish
                        anchors.left: lablePublish.right
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("")
                    }
                }

            }
        }

        Rectangle{
            id:rectSetModel
            width: 200
            height: 40
            anchors.left: rectdid.left
            anchors.top: rectdid.bottom
            anchors.topMargin: 10
            LineEdit {
                id: inputTempDrift
                width: rectSetModel.width  - 40
                height: rectSetModel.height -2
                anchors.left: rectdid.left
                anchors.verticalCenter: rectSetModel.verticalCenter
                border.width: 1
                //inputLimite:Qt.ImhDigitsOnly
                //font.pixelSize: fontSize
                placeholderText: ""
                //isNeedDoubleClickEdit: false
                textLeftPadding:0
                txtColor: Qt.rgba(0,0,0,0.65)
                text: "0"
                color: "#F8FAFD"
            }

            Button{
                id:btnSetModel
                anchors.left: inputTempDrift.right
                anchors.leftMargin: 10
                anchors.verticalCenter: inputTempDrift.verticalCenter
                text: "设置型号"
                onClicked: {
                    txtSetModel.text = ""
                    var map = {
                        cmd:"setinftempmodel",
                        tempmodel:inputTempDrift.text
                    }
                    s_setinftempmodel(map)
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
            id:rectSetType
            width: 200
            height: 40
            anchors.left: rectdid.left
            anchors.top: rectSetModel.bottom
            anchors.topMargin: 10
            LineEdit {
                id: inputType
                width: rectSetType.width  - 40
                height: rectSetType.height -2
                anchors.left: rectdid.left
                anchors.verticalCenter: rectSetType.verticalCenter
                border.width: 1
                //inputLimite:Qt.ImhDigitsOnly
                //font.pixelSize: fontSize
                placeholderText: ""
                //isNeedDoubleClickEdit: false
                textLeftPadding:0
                txtColor: Qt.rgba(0,0,0,0.65)
                text: "0"
                color: "#F8FAFD"
            }

            Button{
                id:btnSetType
                anchors.left: inputType.right
                anchors.leftMargin: 10
                anchors.verticalCenter: inputType.verticalCenter
                text: "设置模组"
                onClicked: {

                    txtSetType.text =""
                    var map = {
                        cmd:"setinftemptype",
                        temptype:inputType.text
                    }
                    s_setinftemptype(map)
                }
            }

            Text {
                id: txtSetType
                anchors.left: btnSetType.right
                anchors.leftMargin: 10
                anchors.verticalCenter: btnSetType.verticalCenter
                text: qsTr("")
            }
        }

        Rectangle{
            id:faceRect
            width: rectW
            height: 150
            anchors.left: rectdid.left
            anchors.top: rectSetType.bottom
            anchors.topMargin: 10
            border.width: 1
            border.color: "red"

            Button{
                id:btnGetdevicekey
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 10
                text: "获取算法 key"
                onClicked: {
                    var map = {
                        cmd:"getdevicekey",
                    }
                    s_getdevicekey(map)
                }
            }

            Button{
                id:btnWriteLicense
                anchors.left: btnGetdevicekey.right
                anchors.leftMargin: 10
                anchors.verticalCenter: btnGetdevicekey.verticalCenter
                text: "烧写licence"
                onClicked: {

                    toolutil.funStartCmd(txtKeyID.text)



                   /* var map = {
                        cmd:"devicekey",
                        key:txtKeyID.text
                    }
                    s_setsignature(map)*/
                }
            }

            Text {
                id: labelKeyID
                text: qsTr("deviceKey:")
                anchors.left: btnGetdevicekey.left
                anchors.top: btnGetdevicekey.bottom
                anchors.topMargin: 10
            }

            Text {
                id: txtKeyID
                text: qsTr("")
                anchors.left: labelKeyID.right
                anchors.leftMargin: 10
                anchors.top: labelKeyID.top
            }

            Text {
                id: labelLicenseInfo
                anchors.left: btnGetdevicekey.left
                anchors.top: labelKeyID.bottom
                anchors.topMargin: 10
                text: qsTr("lisence info:")
            }

            Text {
                id: txtLicenseInfo
                anchors.left: labelLicenseInfo.right
                anchors.leftMargin: 10
                anchors.top: labelLicenseInfo.top

                text: qsTr("")
            }

        }



        Rectangle{
            id:rectSdCard
            width: rectW
            height: 100
            anchors.left: rectdid.left
            anchors.top: faceRect.bottom
            anchors.topMargin: 10
            border.width: 1
            border.color: "red"


            Button{
                id:btnGetSd
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 10
                text: "获取sdcard信息"
                onClicked: {

                    var map = {
                        cmd:"getsdcardparam"
                    }
                    s_getsdcardparam(map)
                }
            }

            Button{
                id:btnFormat
                anchors.left: btnGetSd.right
                anchors.leftMargin: 10
                anchors.verticalCenter:btnGetSd.verticalCenter

                text: "sd卡格式化"
                onClicked: {
                    var map = {
                        cmd:"setsdcardformat"
                    }
                    s_setsdcardformat(map)

                }
            }

            Text {
                id: txtsdinfo
                anchors.left: btnGetSd.left
                anchors.leftMargin: 10
                anchors.top: btnGetSd.bottom
                anchors.topMargin: 10
                text: qsTr("sdcard:")
            }


            Text{
                id:txtFormat
                anchors.left: btnFormat.right
                anchors.verticalCenter: btnFormat.verticalCenter
                anchors.leftMargin: 10
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
            anchors.rightMargin: 20
            anchors.bottomMargin: 20
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
            anchors.rightMargin: 10
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

    FileDialog {
        id: fileDialog
        property string pathname:""
        title: "Please choose a file path"
        selectFolder:false
        selectMultiple: false
        //folder: shortcuts.home
        onAccepted: {
            var str = fileDialog.fileUrl.toString();
            toolutil.readDidFile(str.replace('file:///',''))
        }
        onRejected: {

        }
    }

    function getdid(str)
    {
        writeDidTip.text = str;
        toolutil.setWriteDidLabel();
    }

    function getinftempmodel(str)
    {
        txtSetModel.text = str;
    }

    function getdevicekey(str)
    {
        txtKeyID.text = str;
    }

    function getsignature(str)
    {
        txtLicenseInfo.text = str
    }

    function getsdcardparam(str){
        txtsdinfo.text = str
    }

    function setSdcardFarmat(str)
    {
        txtFormat.text = str;
    }

    function getinftempType(str){

        txtSetType.text = str;
    }
}

