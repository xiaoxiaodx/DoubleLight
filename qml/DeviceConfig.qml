import QtQuick.Controls 2.5
import QtQuick 2.0
import QtGraphicalEffects 1.12
import QtQuick.Dialogs 1.3
import Qt.labs.settings 1.0
import "simpleControl"
Rectangle {
    id: root

    signal s_timeSwith(bool mchecked);
    signal s_warnSwith(bool mchecked);
    signal s_screenShotSwith(bool mchecked);
    signal s_beerSwith(bool mchecked);
    signal s_recordSwith(bool mchecked);
    signal s_temSet(var mvalue);
    signal s_screenShotPathSet(var mvalue);
    signal s_recordPathSet(var mvalue);
    signal s_temDrift(var mvalue);
    signal s_getInitPar();


    signal s_temOffset(var mvalue);
    signal s_temMax(var mvalue);
    signal s_temMin(var mvalue);
    property int fontSize: 14
    Settings {
        id:setting
        property alias recordPath: inputRecordPath.text
        property alias screenShotPath: inputScreenShotPath.text
        property alias temDrift:inputTempDrift.text
        property alias warnTem:inputTem.text

        property alias switchTime:swithTime.checked
        property alias switchWarn:swichWarn.checked
        property alias switchScreenShot:swichScreenShot.checked
        property alias switchBeer:swichBeer.checked
        property alias switchRecord:swichRecord.checked
    }

    Rectangle {
        id: rect
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width -40
        height: parent.height -40
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

        SimpleSwich{
            id:swithTime
            width: 30
            height: 15
            anchors.left: labelSwitchTime.right
            anchors.leftMargin: 20
            anchors.verticalCenter: labelSwitchTime.verticalCenter
            onCheckedChanged: s_timeSwith(checked)
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

        Rectangle{
            id:rectTempDrift
            color: "#D6D8DB"
            width: 88
            height: 32
            radius: 4
            anchors.left: txtTempDrift.right
            anchors.verticalCenter: txtTempDrift.verticalCenter
            anchors.leftMargin: 20
            LineEdit {
                id: inputTempDrift
                width: rectTempDrift.width  - 22
                height: rectTempDrift.height -2
                anchors.left: parent.left
                anchors.leftMargin: 1
                anchors.verticalCenter: rectTempDrift.verticalCenter
                border.width: 0
                inputLimite:Qt.ImhFormattedNumbersOnly
                font.pixelSize: fontSize
                placeholderText: ""
                isNeedDoubleClickEdit: false
                textLeftPadding:0
                txtColor: Qt.rgba(0,0,0,0.65)
                color: "#F8FAFD"
                onTextChanged: {
                    s_temDrift(inputTempDrift.text);

                }
            }
            Image {
                id: imgValueUp
                width: 20
                height: 15
                source: "qrc:/images/arrow_up.png"
                anchors.right: parent.right
                anchors.rightMargin: 1
                anchors.top: parent.top
                anchors.topMargin: 1
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        imgValueUp.source="qrc:/images/arrow_up_p.png"
                        var num = parseFloat(inputTempDrift.text).toFixed(2)

                        inputTempDrift.text = ""+(Number(num)+Number(0.01)).toFixed(2)

                    }
                    onReleased: imgValueUp.source="qrc:/images/arrow_up.png"
                }
            }
            Image {
                id: imgValuedown
                width: 20
                height: 15
                source: "qrc:/images/arrow_low.png"
                anchors.right: imgValueUp.right
                anchors.top: imgValueUp.bottom
                anchors.topMargin: 1
                MouseArea{
                    anchors.fill: parent

                    onPressed: {

                        imgValuedown.source="qrc:/images/arrow_low_p.png"
                        var num = parseFloat(inputTempDrift.text).toFixed(2)

                        inputTempDrift.text = ""+(Number(num)-Number(0.01)).toFixed(2)
                    }
                    onReleased: imgValuedown.source="qrc:/images/arrow_low.png"
                }
            }

        }

        Text {
            id: txtTemp
            text: qsTr("℃")
            font.pixelSize: fontSize
            anchors.verticalCenter: rectTempDrift.verticalCenter
            anchors.left: rectTempDrift.right
            anchors.topMargin: 6
        }

        Text {
            id: txtSwichWarn
            text: qsTr("报警开关")
            font.pixelSize: fontSize
            anchors.top: txtTempDrift.bottom
            anchors.left: txtTempDrift.left
            anchors.topMargin: 26
        }

        SimpleSwich{
            id:swichWarn
            width: 30
            height: 15
            anchors.left: txtSwichWarn.right
            anchors.leftMargin: 20
            anchors.verticalCenter: txtSwichWarn.verticalCenter
            onCheckedChanged: s_warnSwith(checked)
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
        SimpleSwich{
            id:swichScreenShot
            width: 30
            height: 15
            anchors.left: txtSwichScreenShot.right
            anchors.leftMargin: 20
            anchors.verticalCenter: txtSwichScreenShot.verticalCenter
            onCheckedChanged: s_screenShotSwith(checked)
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
        SimpleSwich{
            id:swichBeer
            width: 30
            height: 15
            anchors.left: txtSwichBeer.right
            anchors.leftMargin: 20
            anchors.verticalCenter: txtSwichBeer.verticalCenter
            onCheckedChanged: s_beerSwith(checked)
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
        SimpleSwich{
            id:swichRecord
            width: 30
            height: 15
            anchors.left: txtSwichRecord.right
            anchors.leftMargin: 20
            anchors.verticalCenter: txtSwichRecord.verticalCenter
            onCheckedChanged: s_recordSwith(checked)
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


        Column{

            anchors.left: rectRecordPath.left
            anchors.top: rectRecordPath.bottom
            anchors.topMargin: 10
            spacing: 5
            Rectangle{
                width: 300
                height: 60
                anchors.horizontalCenter: parent.horizontalCenter

                LineEdit {
                    id: intputMax
                    width: 300
                    height: 60
                    font.pixelSize: fontSize
                    placeholderText: ""
                    isNeedDoubleClickEdit: false
                    textLeftPadding:0
                    txtColor: Qt.rgba(0,0,0,0.65)
                    color: "#DEDFE3"

                }
                Button{
                    id:btnInputMax
                    anchors.left: intputMax.right
                    anchors.leftMargin: 10
                    anchors.verticalCenter: intputMax.verticalCenter
                    width: 100
                    height: 60
                    text:"tempMax"
                    onClicked: {
                        s_temMax(intputMax.text)
                    }

                }
            }

            Rectangle{
                width: 300
                height: 60
                anchors.horizontalCenter: parent.horizontalCenter

                LineEdit {
                    id: intputmin
                    width: 300
                    height: 60
                    font.pixelSize: fontSize
                    placeholderText: ""
                    isNeedDoubleClickEdit: false
                    textLeftPadding:0
                    txtColor: Qt.rgba(0,0,0,0.65)

                    color: "#DEDFE3"

                }
                Button{
                    id:btnInputMin
                    anchors.left: intputmin.right
                    anchors.leftMargin: 10
                    anchors.verticalCenter: intputmin.verticalCenter
                    width: 100
                    height: 60
                    text: "tempMin"
                    onClicked: {
                        s_temMin(intputmin.text)
                    }

                }
            }

            Rectangle{
                width: 300
                height: 60
                anchors.horizontalCenter: parent.horizontalCenter

                LineEdit {
                    id: intputOffset
                    width: 300
                    height: 60
                    font.pixelSize: fontSize
                    placeholderText: ""
                    isNeedDoubleClickEdit: false
                    textLeftPadding:0
                    txtColor: Qt.rgba(0,0,0,0.65)
                    color: "#DEDFE3"

                }
                Button{
                    id:btnInputOffset
                    anchors.left: intputOffset.right
                    anchors.leftMargin: 10
                    anchors.verticalCenter: intputOffset.verticalCenter
                    width: 100
                    height: 60
                    text: "tempOffset"
                    onClicked: {
                        s_temOffset(intputOffset.text)
                    }

                }
            }
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
    function getTemDrift(){
        return setting.temDrift
    }
    function getWarnTem(){
        return setting.warnTem
    }
    function getSwitchWarn()
    {
        return setting.switchWarn;
    }

    function getSwitchTime(){
        return setting.switchTime
    }

    function getSwitchRecord(){
        return setting.switchRecord
    }

    function getSwitchScreenShot(){
        return setting.switchScreenShot
    }

    function getSwitchBeer(){
        return setting.switchBeer
    }


}
