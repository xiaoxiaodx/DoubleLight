import QtQuick.Controls 2.5
import QtQuick 2.0
import QtGraphicalEffects 1.12
import QtQuick.Dialogs 1.3
import Qt.labs.settings 1.0
import "simpleControl"
Popup {
    id: root
    x: parent.width/2 - root.width/2
    y: parent.height/2 - root.height/2
    width: 380
    height: 253
    modal: true
    focus: true
    //设置窗口关闭方式为按“Esc”键关闭
    closePolicy: Popup.OnEscape
    //设置窗口的背景控件，不设置的话Popup的边框会显示出来
    background: rect

    property int fontSize: 14


    signal s_timeSwith(bool mchecked);
    signal s_warnSwith(bool mchecked);
    signal s_screenShotSwith(bool mchecked);
    signal s_beerSwith(bool mchecked);
    signal s_recordSwith(bool mchecked);
    signal s_temSet(var mvalue);
    signal s_screenShotPathSet(var mvalue);
    signal s_recordPathSet(var mvalue);
    signal s_temOffset(var mvalue);


    Settings {
        id:setting
        property alias recordPath: inputScreenShotPath.text
        property alias screenShotPath: inputRecordPath.text
    }

    Rectangle {
        id: rect
        anchors.fill: parent
        color: "#F8FAFD"


        Text {
            id: labelTime
            font.pixelSize: fontSize
            text: qsTr("时间显示")
            anchors.bottom: line1.top
            anchors.left: line1.left
            anchors.bottomMargin: 20

        }

        Rectangle{
            id:line1
            width: parent.width - 20*2
            height: 1
            color: "#e2e2e2"
            anchors.top: parent.top
            anchors.topMargin: 62
            anchors.horizontalCenter: parent.horizontalCenter

        }

        Text {
            id: labelSwitchTime
            text: qsTr("时间开关")
            font.pixelSize: fontSize
            anchors.top: line1.bottom
            anchors.left: line1.left
            anchors.leftMargin: 48
            anchors.topMargin: 20
        }

        SwitchButton{
            id:swithTime
            width: 30
            height: 15
            anchors.left: labelSwitchTime.right
            anchors.leftMargin: 20
            anchors.verticalCenter: labelSwitchTime.verticalCenter
            bgColor: "#476BFD"
            onCheckedChange: s_timeSwith(checked)
        }

        Text {
            id: name
            font.pixelSize: fontSize
            text: qsTr("参数设置")

            anchors.bottom: line2.top
            anchors.left: line2.left
            anchors.bottomMargin: 20

        }
        Rectangle{
            id:line2
            width:parent.width - 20*2
            height: 1
            color: "#e2e2e2"
            anchors.top: parent.top
            anchors.topMargin: 187
            anchors.horizontalCenter: parent.horizontalCenter
        }


        Text {
            id: txtTempDrift
            text: qsTr("温漂设置")
            font.pixelSize: fontSize
            anchors.top: line2.bottom
            anchors.left: line2.left
            anchors.leftMargin: 48
            anchors.topMargin: 24
        }

        Text {
            id: txtSwichWarn
            text: qsTr("报警开关")
            font.pixelSize: fontSize
            anchors.top: txtTempDrift.bottom
            anchors.left: txtTempDrift.left
            anchors.topMargin: 26
        }

        SwitchButton{
            id:swichWarn
            width: 30
            height: 15
            anchors.left: txtSwichWarn.right
            anchors.leftMargin: 20
            anchors.verticalCenter: txtSwichWarn.verticalCenter
            bgColor: "#476BFD"
            onCheckedChange: s_warnSwith(checked)
        }
        Text {
            id: txtWarnTemSet
            text: qsTr("告警温度设置")
            font.pixelSize: fontSize
            anchors.left: swichWarn.right
            anchors.verticalCenter: swichWarn.verticalCenter
            anchors.leftMargin: 100
        }
        LineEdit {
            id: inputTem
            width: 88
            height: 28
            anchors.left: txtWarnTemSet.right
            anchors.leftMargin: 20
            anchors.verticalCenter: txtWarnTemSet.verticalCenter
            border.width: 1
            border.color: "#DEDFE3"
            font.pixelSize: fontSize
            placeholderText: ""
            isNeedDoubleClickEdit: false
            textLeftPadding:0
            txtColor: Qt.rgba(0,0,0,0.65)
            color: "#F8FAFD"
            onTextChanged: s_temSet(inputTem.text)
        }
        Text {
            id: txtTem1
            text: qsTr("℃")
            font.pixelSize: fontSize

            anchors.left: inputTem.right
            anchors.leftMargin: 6
            anchors.verticalCenter: inputTem.verticalCenter
        }

        Text {
            id: txtSwichScreenShot
            text: qsTr("抓拍开关")
            font.pixelSize: fontSize
            anchors.top: txtSwichWarn.bottom
            anchors.left: txtSwichWarn.left
            anchors.topMargin: 26
        }
        SwitchButton{
            id:swichScreenShot
            width: 30
            height: 15
            anchors.left: txtSwichScreenShot.right
            anchors.leftMargin: 20
            anchors.verticalCenter: txtSwichScreenShot.verticalCenter
            bgColor: "#476BFD"
            onCheckedChange: s_screenShotSwith(checked)
        }

        Text {
            id: txtScreenShotPath
            text: qsTr("抓拍存储路径")
            font.pixelSize: fontSize
            anchors.left: swichScreenShot.right
            anchors.leftMargin: 20
            anchors.verticalCenter: swichScreenShot.verticalCenter
        }
        Rectangle{

            id:rectScreenShotPath
            color: "#F8FAFD"
            border.color: "#DEDFE3"
            border.width: 1
            width: 200
            height: 28
            anchors.left: txtScreenShotPath.right
            anchors.verticalCenter: txtScreenShotPath.verticalCenter
            anchors.leftMargin: 20
            LineEdit {
                id: inputScreenShotPath
                width: rectScreenShotPath.width - imgScreenShotPath.width - 22
                height: rectScreenShotPath.height -2
                anchors.left: parent.left
                anchors.leftMargin: 2

                anchors.verticalCenter: rectScreenShotPath.verticalCenter
                border.width: 0

                font.pixelSize: fontSize
                placeholderText: ""
                isNeedDoubleClickEdit: false
                textLeftPadding:0
                txtColor: Qt.rgba(0,0,0,0.65)
                color: "#F8FAFD"
                //onTextChanged: s_screenShotPathSet(inputScreenShotPath.text)
            }
            Image {
                id: imgScreenShotPath
                width: 16
                height: 13
                source: "qrc:/images/file.png"
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        imgScreenShotPath.source = "qrc:/images/fileopen.png"
                    }
                    onReleased: {
                        imgScreenShotPath.source = "qrc:/images/file.png"
                    }
                    onClicked: {
                        fileDialog.pathname = "screenShotPath";

                        fileDialog.open()
                    }
                }
            }

        }

        Text {
            id: txtSwichBeer
            text: qsTr("蜂鸣开关")
            font.pixelSize: fontSize
            anchors.top: txtSwichScreenShot.bottom
            anchors.left: txtSwichScreenShot.left
            anchors.topMargin: 26
        }
        SwitchButton{
            id:swichBeer
            width: 30
            height: 15
            anchors.left: txtSwichBeer.right
            anchors.leftMargin: 20
            anchors.verticalCenter: txtSwichBeer.verticalCenter
            bgColor: "#476BFD"
            onCheckedChange: s_beerSwith(checked)
        }


        Text {
            id: txtRecordSet
            text: qsTr("录像设置")
            font.pixelSize: fontSize
            anchors.bottom: line3.top
            anchors.left: line3.left
            anchors.bottomMargin: 20
        }

        Rectangle{
            id:line3
            width: parent.width - 20*2
            height: 1
            color: "#e2e2e2"
            anchors.top: parent.top
            anchors.topMargin: 448
            anchors.horizontalCenter: parent.horizontalCenter
        }


        Text {
            id: txtSwichRecord
            text: qsTr("录像开关")
            font.pixelSize: fontSize
            anchors.top: line3.bottom
            anchors.left: line3.left
            anchors.topMargin: 20
            anchors.leftMargin: 48
        }
        SwitchButton{
            id:swichRecord
            width: 30
            height: 15
            anchors.left: txtSwichRecord.right
            anchors.leftMargin: 20
            anchors.verticalCenter: txtSwichRecord.verticalCenter
            bgColor: "#476BFD"
            onCheckedChange: s_recordSwith(checked)
        }

        Text {
            id: txtRecortPath
            text: qsTr("录像存储路径")
            font.pixelSize: fontSize
            anchors.left: swichRecord.right
            anchors.leftMargin: 20
            anchors.verticalCenter: swichRecord.verticalCenter
        }
        Rectangle{

            id:rectRecordPath
            color: "#F8FAFD"
            border.color: "#DEDFE3"
            border.width: 1
            width: 200
            height: 28
            anchors.left: txtRecortPath.right
            anchors.verticalCenter: txtRecortPath.verticalCenter
            anchors.leftMargin: 20
            LineEdit {
                id: inputRecordPath
                width: rectRecordPath.width - imgRecordPath.width - 22
                height: rectRecordPath.height -2
                anchors.left: parent.left
                anchors.leftMargin: 2

                anchors.verticalCenter: rectRecordPath.verticalCenter
                border.width: 0

                font.pixelSize: fontSize
                placeholderText: ""
                isNeedDoubleClickEdit: false
                textLeftPadding:0
                txtColor: Qt.rgba(0,0,0,0.65)
                color: "#F8FAFD"
                //onTextChanged: s_recordPathSet(inputRecordPath.text)

            }
            Image {
                id: imgRecordPath
                width: 16
                height: 13
                source: "qrc:/images/file.png"
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        imgRecordPath.source = "qrc:/images/fileopen.png"
                    }
                    onReleased: {
                        imgRecordPath.source = "qrc:/images/file.png"
                    }
                    onClicked: {
                        fileDialog.pathname = "recordPath";

                        fileDialog.open()
                    }
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
        selectFolder:true
        selectMultiple: false
        //folder: shortcuts.home
        onAccepted: {
            if(pathname === "recordPath"){
                var str = fileDialog.fileUrl.toString();
                inputRecordPath.text = str.replace('file:///','');
               // devicemanagerment.recordingPath = txtVedioSavePath.text
            }else if(pathname === "screenShotPath"){
                inputScreenShotPath.text = fileDialog.fileUrl.toString().replace('file:///','');
                //devicemanagerment.screenShotPath = txtscreenshotSavePath.text
            }

        }
        onRejected: {


        }
    }

    function getRecordPath(){
        return setting.recordPath;
    }
    function getScrennShotPath(){
        return setting.screenShotPath;
    }

    function setDlgPoint(dlgX ,dlgY)
    {
        //设置窗口拖拽不能超过父窗口
        if(root.x + dlgX < 0)
        {
            root.x = 0
        }
        else if(root.x + dlgX > root.parent.width - root.width)
        {
            root.x = root.parent.width - root.width
        }
        else
        {
            root.x = root.x + dlgX
        }
        if(root.y + dlgY < 0)
        {
            root.y = 0
        }
        else if(root.y + dlgY > root.parent.height - root.height)
        {
            root.y = root.parent.height - root.height
        }
        else
        {
            root.y = root.y + dlgY
        }
    }
}
