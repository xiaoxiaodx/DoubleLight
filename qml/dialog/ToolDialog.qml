import QtQuick 2.0
import QtQuick.Controls 2.5
import "../simpleControl"
import QtGraphicalEffects 1.12
import QtQuick.Dialogs 1.3
import Qt.labs.settings 1.0
import ToolUtil 1.0
Popup {
    id: root
    x: parent.width - root.width -10
    y: parent.height/2- root.height/2
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
    signal s_setinftemplevel(var map)
    property int rectW: 330


    property bool isOneClickTest: false
    Settings{
        id:settings

        property alias filepath: txtPath.text
        property alias  drif: inputTempDrift.text
        property alias type: inputType.text
    }
    ToolUtil{
        id:toolutil

        onSignal_setDidInfo:{
            txtDid.text = did
            txtlisence.text = p2pkey
            txtPublish.text = pushKey
        }

        onSignal_tip:{
            if(isSucc){
                btnEnsure.enabled = false;
                //writeDidTip.text = "已存入文件"
                //txtLicenseInfo.text ="已存入文件"
            }
            savedid.text = str;
        }
        onSignal_sendLisence:{//lisence

            var map = {
                cmd:"setsignature",
                signature:lisence
            }
            s_setsignature(map)
        }

        Component.onCompleted: {
            toolutil.readDidFile(settings.filepath);
        }
    }
    Text {
        id: txtPath
        visible: false
        text: qsTr("")
    }
    Rectangle {
        id: rect
        anchors.fill: parent
        color: "#ffffff"
        radius: 3
        //设置标题栏区域为拖拽区域
        Text {
            id: txtver
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 10
            anchors.topMargin: 10
            font.pixelSize: 12
            text: qsTr("")
        }
        Rectangle{
            id:rectdid
            width:rectW
            height: 160
            border.width: 1
            border.color: "red"
            anchors.left: parent.left
            anchors.leftMargin: 10
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

                        onClicked: setDid();
                    }

                    Text{
                        id:writeDidTip
                        anchors.left: btnWriteDid.right
                        anchors.verticalCenter: btnWriteDid.verticalCenter
                        anchors.leftMargin: 10
                        text: "null"
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
                    setinftempmodel();
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
                    setinftemptype()

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
                onClicked:getdevicekey();
            }

            Button{
                id:btnWriteLicense
                anchors.left: btnGetdevicekey.right
                anchors.leftMargin: 10
                anchors.verticalCenter: btnGetdevicekey.verticalCenter
                text: "烧写licence"
                onClicked: {
                    toolutil.funStartCmd(txtKeyID.text)
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
                wrapMode: Text.WordWrap

                text: qsTr("null")
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
                onClicked: sdcardgetInfo();
            }

            Button{
                id:btnFormat
                anchors.left: btnGetSd.right
                anchors.leftMargin: 10
                anchors.verticalCenter:btnGetSd.verticalCenter

                text: "sd卡格式化"
                onClicked: {

                    sdcardFormat();
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


        Button{
            id:btnsetinftemplevel
            anchors.top: rectSdCard.bottom
            anchors.topMargin: 5
            anchors.left: rectdid.left
            text:"设置温度等级"
            onClicked: {
                var map = {
                    cmd:"setinftemplevel",
                    templevel:6
                }
                s_setinftemplevel(map)
            }
        }

        Text {
            id: txtinftemplevel
            anchors.left: btnsetinftemplevel.right
            anchors.leftMargin: 10
            anchors.verticalCenter: btnsetinftemplevel.verticalCenter
            text: qsTr("")
        }

        Button{
            id:btnOneClick
            anchors.bottom: btnEnsure.top
            anchors.bottomMargin: 20
            anchors.horizontalCenter: btnEnsure.horizontalCenter
            text: qsTr("一键测试")
            onClicked: {
                isOneClickTest = true;
                btnOneClick.enabled = false;
                setDid();
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
                text: qsTr("保存did lisence")
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {

                    toolutil.setDidLisence(writeDidTip.text,txtLicenseInfo.text);
                    //root.close()
                }
            }
        }

        Text {
            id: savedid
            anchors.top: btnEnsure.bottom
            anchors.topMargin: 2
            anchors.horizontalCenter: btnEnsure.horizontalCenter
            font.pixelSize: 10
            color: "red"
            text: qsTr("tips")
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
                text: qsTr("关闭窗口")
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
            var path = str.replace('file:///','');
            txtPath.text = path
            toolutil.readDidFile(path)


        }
        onRejected: {

        }
    }


    //烧写did

    function setDid(){
        var map = {
            cmd:"setdid",
            uuid:txtDid.text,
            lisence:txtlisence.text,
            pushlis:txtPublish.text
        }
        s_setdid(map)
        writeDidTip.text = ""
    }
    //did成功后  设置型号
    function getdid(str)
    {
        writeDidTip.text = str;
        toolutil.deleteFirstDid();
        toolutil.setWriteDidLabel();
        btnWriteDid.enabled = false;

        if(isOneClickTest)
            setinftempmodel();
    }


    //设置型号
    function setinftempmodel(){

        txtSetModel.text = ""
        var map = {
            cmd:"setinftempmodel",
            tempmodel:inputTempDrift.text
        }
        s_setinftempmodel(map)
    }

    //型号设置成功后设置模组
    function getinftempmodel(str)
    {
        txtSetModel.text = str;
        if(isOneClickTest)
            setinftemptype()
    }


    //设置模组
    function setinftemptype(){

        txtSetType.text =""
        var map = {
            cmd:"setinftemptype",
            temptype:inputType.text
        }
        s_setinftemptype(map)
    }
    //模组成功后，获取sdcard信息
    function getinftempType(str){

        txtSetType.text = str;

        if(isOneClickTest)
            sdcardgetInfo()
    }

    //获取sdcard信息
    function sdcardgetInfo(){
        var map = {
            cmd:"getsdcardparam"
        }
        s_getsdcardparam(map)
    }

    //sdcard 信息获取后,格式化sdcard
    function getsdcardparam(str){
        txtsdinfo.text = str
        if(isOneClickTest)
            sdcardFormat()
    }

    //sdcard 格式化
    function sdcardFormat(){
        var map = {
            cmd:"setsdcardformat"
        }
        s_setsdcardformat(map)
    }

    //sdcard 格式化后 ，设置层级
    function setSdcardFarmat(str)
    {
        txtFormat.text = str;
        if(isOneClickTest){

            var map = {
                cmd:"setinftemplevel",
                templevel:6
            }
            s_setinftemplevel(map)
        }
    }

    //温度层级设置后，设置key
    function setinftemplevel(str){
        txtinftemplevel.text = str
        if(isOneClickTest)
            getdevicekey();
    }

    //获取算法key
    function getdevicekey(){
        var map = {
            cmd:"getdevicekey",
        }
        s_getdevicekey(map)
    }



    function setdevicekey(str)
    {
        txtKeyID.text = str;

        if(isOneClickTest)
            toolutil.funStartCmd(txtKeyID.text)

    }

    function getsignature(str)
    {
        txtLicenseInfo.text = str

        btnOneClick.text =" 测试完毕"
        if(isOneClickTest)
            toolutil.setDidLisence(writeDidTip.text,str);
    }



    /***********/
    function setver(ver)
    {
        txtver.text = "版本:"+ver
    }
}

