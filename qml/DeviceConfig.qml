import QtQuick.Controls 2.5
import QtQuick 2.0
import QtGraphicalEffects 1.12
import QtQuick.Dialogs 1.3
import Qt.labs.settings 1.0
import "simpleControl"
import UpdateProgressC 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "dialog"
import QtQuick.Controls.Styles 1.4
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
    //    signal s_temDrift(var mvalue);
    
    signal s_getInitPar();
    
    signal s_temOffset(var mvalue);
    signal s_temMax(var mvalue);
    signal s_temMin(var mvalue);
    signal s_temImage(var mvalue);
    
    
    property bool isCreate: false
    
    signal s_sendcommoncmd(var mvalue);
    signal s_deviceUpdate()


    property int fontSize: curLanguage ===lKhmer?28:14
    property int lineeditfontSize: 14
    property color fontColor: "#333333"


    
    property string curDevTypeStr:"E03"
    //第一根左对齐线
    property int parSetFirstAlignLine: curLanguage === lChinese?134:curLanguage === lEnglish?230:curLanguage === lKorean?204:curLanguage ===lRussian?210:300
    property int parSetSecondAlignLine: curLanguage === lChinese?368:curLanguage === lEnglish?488:curLanguage === lKorean?388:curLanguage ===lRussian?538:700
    
    property int firstTopMargin: 15
    property int perTopMargin: 20

    property int tempdriftcapMax: 2
    property int tempdriftcapMin: -2
    property int tempcontrolcapMax: 6
    property int tempcontrolcapMin: 0

    property int imagparammirror: -1
    property int imagparamflip: -1
    property int imagparambrightness: -1
    property int imagparamcolorsaturation:-1
    property int imagparamcontrast: -1
    property int imagparamhue: -1
    property int imagparamsharpness: -1
    property int imagparamwdr: -1
    property bool wdrisChange: false

    Settings {
        id:setting
        
        fileName: "config.ini"
        
        property alias recordPath: inputRecordPath.text
        property alias screenShotPath: inputScreenShotPath.text
        property alias temDrift:inputTempDrift.text
        property alias tempMin: inputTempMin.text
        //property alias tempMax: inputTempMax.text
        property alias warnTem:inputTem.text
        //property alias switchTime:swithTime.checked
        property alias switchWarn:swichWarn.checked
        property alias switchScreenShot:swichScreenShot.checked
        property alias switchBeer:swichBeer.checked
        property alias switchRecord:swichRecord.checked

        
        
        property alias updateFilePath:inputUpdatePath.text
        
        //irc 图像选择
        // property alias ircImgColorType: cmbImgSelect.currentIndex
        
        property string tcpip:"192.168.1.188"
        property bool isOpenAdjustRect: true
        property int e03showRectX: 65;
        property int e03showRectY : 41;
        property int e03showRectW : 349;
        property int e03showRectH : 327;
        property int e03showParentW : 494;
        property int e03showParentH : 369;
        
        property int f03showRectX: 65;
        property int f03showRectY : 41;
        property int f03showRectW : 349;
        property int f03showRectH : 327;
        property int f03showParentW : 494;
        property int f03showParentH : 369;
        
        property int d06showRectX: 65;
        property int d06showRectY : 41;
        property int d06showRectW : 349;
        property int d06showRectH : 327;
        property int d06showParentW : 494;
        property int d06showParentH : 369;
        
        property int d04showRectX: 65;
        property int d04showRectY : 41;
        property int d04showRectW : 349;
        property int d04showRectH : 327;
        property int d04showParentW : 494;
        property int d04showParentH : 369;
    }
    
    
    Component.onCompleted: {
        
        setting.screenShotPath =  screenv.funIsExitCurCapturePath(setting.screenShotPath)
        setting.recordPath = screenv.funIsExitCurRecordPath(setting.recordPath)
    }

    Rectangle{
        width: parent.width
        height: parent.height - mDeviceStateBar.height

        color: "#DFE1E6"
        MouseArea{
            anchors.fill: parent
        }
        
        Component.onCompleted: isCreate = true;

        Rectangle{
            id:rectcontent
            width: parent.width-40
            height: parent.height -40
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: "#F8FAFD"

            ScrollView{
                id:scroll
                width: rectcontent.width
                height: rectcontent.height
                
                horizontalScrollBarPolicy:Qt.ScrollBarAlwaysOff
                //frameVisible:true
                style: ScrollViewStyle{
                    
                    incrementControl:null
                    decrementControl :null
                    
                    scrollBarBackground :Rectangle{
                        
                        color: "#F8FAFD"
                        height: scroll.height
                        width: 10
                    }
                    
                    handle:Rectangle{
                        color: "#989898"
                        implicitWidth: 10
                        implicitHeight: 10
                        width: 10
                        height: 20
                        radius: 5
                        visible: styleData.hovered
                    }
                }
                Rectangle {
                    id: rect
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: rectcontent.width
                    height: 1200
                    color: "#F8FAFD"
                    
                    Text {
                        id: labelTime
                        font.pixelSize: fontSize
                        color: fontColor
                        text: qsTr("时间设置")
                        anchors.bottom: lineTime.top
                        anchors.left: lineTime.left
                        anchors.bottomMargin: 20
                        
                    }
                    
                    Rectangle{
                        id:lineTime
                        width: parent.width - 20*2
                        height: 1
                        color: "#e2e2e2"
                        anchors.top: parent.top
                        anchors.topMargin: 62
                        anchors.horizontalCenter: parent.horizontalCenter
                        
                    }
                    
                    Text {
                        id: labelSwitchTime
                        text: qsTr("时间同步")
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.right: rectTimeSynchronization.left
                        anchors.rightMargin: 20
                        anchors.verticalCenter: rectTimeSynchronization.verticalCenter
                    }
                    
                    //                    SimpleSwich{
                    //                        id:swithTime
                    //                        width: 30
                    //                        height: 15
                    //                        anchors.left: lineTime.left
                    //                        anchors.leftMargin: parSetFirstAlignLine
                    //                        anchors.top: lineTime.bottom
                    //                        anchors.topMargin: firstTopMargin
                    //                        onCheckedChanged: {

                    //                            //s_timeSwith(checked)

                    //                        }
                    //                    }

                    Rectangle{
                        id:rectTimeSynchronization
                        anchors.left: lineTime.left
                        anchors.leftMargin: parSetFirstAlignLine
                        anchors.top: lineTime.bottom
                        anchors.topMargin: firstTopMargin
                        width: 44
                        height: 22
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: "#5D9CFF"}
                            GradientStop { position: 1.0; color: "#2D76E7"}
                        }
                        Image{
                            id:imgTimeSynchronization
                            width: 14
                            height: 14
                            source: "qrc:/images/deviceFlush.png"
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                var map = {
                                    cmd:"setcurrenttime"
                                }
                                s_sendcommoncmd(map);
                            }
                            onPressed: imgTimeSynchronization.source = "qrc:/images/deviceFlush_h.png"
                            onReleased: imgTimeSynchronization.source = "qrc:/images/deviceFlush.png"
                        }
                    }

                    Loader{
                        id:loaderToast
                        anchors.verticalCenter: rectTimeSynchronization.verticalCenter
                        anchors.left: rectTimeSynchronization.right
                        anchors.leftMargin: 100
                        sourceComponent: null
                    }

                    Component{
                        id: toastTimeSynchronizationTip
                        Rectangle{
                            id:timeSynchronizationTip
                            width: imgTip.width + txtTip.width + 20 + 12+40
                            height: 52
                            color:"#FFFFFF"
                            Image {
                                id: imgTip
                                width: 20
                                height: 20
                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                anchors.verticalCenter: parent.verticalCenter
                                source: "qrc:/images/connect_succ.png"
                            }

                            Text {
                                id: txtTip
                                font.pixelSize: 16
                                color: fontColor
                                anchors.left: imgTip.right
                                anchors.leftMargin: 12
                                anchors.verticalCenter: parent.verticalCenter
                                text: curLanguage === lEnglish?"Synchronized Successfully":qsTr("同步成功")
                            }

                            SequentialAnimation on opacity {
                                NumberAnimation {to: 1; easing.type: Easing.InQuad; duration: 500}
                                PauseAnimation { duration: 1000}
                                NumberAnimation { to: 0; easing.type: Easing.InQuad; duration: 500}
                            }
                        }
                    }

                    Text {
                        id: txtparset
                        font.pixelSize: fontSize
                        color: fontColor
                        text: qsTr("参数设置")
                        anchors.bottom: lineParset.top
                        anchors.left: lineParset.left
                        anchors.bottomMargin: 20
                    }

                    Rectangle{
                        id:lineParset
                        width:parent.width - 20*2
                        height: 1
                        color: "#e2e2e2"
                        anchors.top: lineTime.bottom
                        anchors.topMargin: 114
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    
                    Text {
                        id: txtTempDrift
                        text: qsTr("温漂设置")
                        color: fontColor
                        font.pixelSize: fontSize
                        anchors.right: rectTempDrift.left
                        anchors.rightMargin: firstTopMargin
                        anchors.verticalCenter: rectTempDrift.verticalCenter
                    }
                    
                    Rectangle{
                        id:rectTempDrift
                        color: "#D6D8DB"
                        width: 88
                        height: 32
                        radius: 4
                        anchors.left: lineParset.left
                        anchors.leftMargin: parSetFirstAlignLine
                        anchors.top: lineParset.bottom
                        anchors.topMargin: 20
                        LineEdit {
                            id: inputTempDrift
                            width: rectTempDrift.width  - 22
                            height: rectTempDrift.height -2
                            anchors.left: parent.left
                            anchors.leftMargin: 1
                            anchors.verticalCenter: rectTempDrift.verticalCenter
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
                                    var num = parseInt(inputTempDrift.text)
                                    if(num >= tempdriftcapMax)
                                        return
                                    
                                    inputTempDrift.text = ""+(Number(num)+Number(1))
                                    
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
                                    var num = parseInt(inputTempDrift.text)
                                    
                                    if(num <= tempdriftcapMin)
                                        return
                                    
                                    inputTempDrift.text = ""+(Number(num)-Number(1))
                                }
                                onReleased: imgValuedown.source="qrc:/images/arrow_low.png"
                            }
                        }
                    }
                    
                    //            Text {
                    //                id: txtTemp
                    //                text: qsTr(" ℃")
                    //                color: fontColor
                    //                font.pixelSize: fontSize
                    //                anchors.verticalCenter: rectTempDrift.verticalCenter
                    //                anchors.left: rectTempDrift.right
                    //                anchors.topMargin: 6
                    //            }
                    
                    ////////////
                    Text {
                        id: txtTempMin
                        text: qsTr("温度控制阀")
                        color: fontColor
                        font.pixelSize: fontSize
                        anchors.right: rectTempMin.left
                        anchors.rightMargin: 20
                        anchors.verticalCenter: rectTempMin.verticalCenter
                        
                    }
                    
                    Rectangle{
                        id:rectTempMin
                        color: "#D6D8DB"
                        width: 88
                        height: 32
                        radius: 4
                        anchors.left: lineParset.left
                        anchors.leftMargin: parSetFirstAlignLine
                        anchors.top:rectTempDrift.bottom
                        anchors.topMargin: perTopMargin

                        LineEdit {
                            id: inputTempMin
                            width: rectTempMin.width  - 22
                            height: rectTempMin.height -2
                            anchors.left: parent.left
                            anchors.leftMargin: 1
                            anchors.verticalCenter: rectTempMin.verticalCenter
                            border.width: 0
                            inputLimite:Qt.ImhDigitsOnly
                            font.pixelSize: fontSize
                            placeholderText: ""
                            isNeedDoubleClickEdit: false
                            textLeftPadding:0
                            txtColor: Qt.rgba(0,0,0,0.65)
                            text: "4"
                            isReadOnly:true
                            color: "#F8FAFD"
                            onTextChanged: {
                                
                                var tempv ;
                                if(inputTempMin.text === "1")
                                    s_temMin("21");
                                else if(inputTempMin.text === "2")
                                    s_temMin("24");
                                else if(inputTempMin.text === "3")
                                    s_temMin("26");
                                else if(inputTempMin.text === "4")
                                    s_temMin("28");
                                else if(inputTempMin.text === "5")
                                    s_temMin("30");
                                else if(inputTempMin.text === "6")
                                    s_temMin("32");
                                
                                
                            }
                        }
                        
                        Image {
                            id: imgValueMinUp
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
                                    imgValueMinUp.source="qrc:/images/arrow_up_p.png"
                                    var num = parseInt(inputTempMin.text)
                                    if(num >= tempcontrolcapMax)
                                        return
                                    
                                    inputTempMin.text = ""+(Number(num)+Number(1))
                                }
                                onReleased: imgValueMinUp.source="qrc:/images/arrow_up.png"
                            }
                        }
                        Image {
                            id: imgValueMindown
                            width: 20
                            height: 15
                            source: "qrc:/images/arrow_low.png"
                            anchors.right: imgValueMinUp.right
                            anchors.top: imgValueMinUp.bottom
                            anchors.topMargin: 1
                            MouseArea{
                                anchors.fill: parent
                                
                                onPressed: {
                                    
                                    imgValueMindown.source="qrc:/images/arrow_low_p.png"
                                    var num = parseInt(inputTempMin.text)
                                    
                                    if(num <= tempcontrolcapMin)
                                        return
                                    
                                    inputTempMin.text = ""+(Number(num)-Number(1))
                                }
                                onReleased: imgValueMindown.source="qrc:/images/arrow_low.png"
                            }
                        }
                        
                    }
                    
                    //            Text {
                    //                id: txtTemp1
                    
                    //                text: qsTr(" ℃")
                    //                color: fontColor
                    //                font.pixelSize: fontSize
                    //                anchors.verticalCenter: rectTempMin.verticalCenter
                    //                anchors.left: rectTempMin.right
                    //                anchors.topMargin: 6
                    
                    //            }
                    
                    
                    Text {
                        id: txtSwichWarn
                        text: qsTr("报警开关")
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.right: swichWarn.left
                        anchors.rightMargin: 20
                        anchors.verticalCenter: swichWarn.verticalCenter
                        
                    }
                    
                    SimpleSwich{
                        id:swichWarn
                        width: 30
                        height: 15
                        anchors.left: lineParset.left
                        anchors.leftMargin: parSetFirstAlignLine
                        anchors.top: rectTempMin.bottom
                        anchors.topMargin: perTopMargin
                        onCheckedChanged:{
                            
                            // s_warnSwith(checked)
                        }
                    }
                    Text {
                        id: txtWarnTemSet
                        text: qsTr("告警温度设置")
                        color: fontColor
                        font.pixelSize: fontSize
                        anchors.right: inputTem.left
                        anchors.rightMargin: 20
                        anchors.verticalCenter: swichWarn.verticalCenter
                        
                    }
                    LineEdit {
                        id: inputTem
                        width: 88
                        height: 28
                        anchors.left: lineParset.left
                        anchors.leftMargin: parSetSecondAlignLine
                        anchors.verticalCenter:swichWarn.verticalCenter
                        border.width: 1
                        border.color: "#DEDFE3"
                        font.pixelSize: fontSize
                        placeholderText: ""
                        isNeedDoubleClickEdit: false
                        textLeftPadding:0
                        txtColor: Qt.rgba(0,0,0,0.65)

                        text: "38"
                        color: "#F8FAFD"
                        validator: RegExpValidator { regExp:  /(\d{1,3})([.,]\d{1,1})?$/ }
                        //inputmask:"^-?(180|1?[0-7]?\\d(\\.\\d{1,1})?)$"//"00.0"
                        onTextChanged: {
                            
                            s_temSet(inputTem.text)
                        }
                    }
                    
                    Text {
                        id: txtTem1
                        text: cmbTempTypeSelect.currentIndex === 0 ?qsTr("℃"):qsTr("℉")
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.left: inputTem.right
                        anchors.leftMargin: 6
                        anchors.verticalCenter: inputTem.verticalCenter
                    }
                    
                    Text {
                        id: txtSwichScreenShot
                        text: qsTr("抓拍开关")
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.right: swichScreenShot.left
                        anchors.rightMargin: 20
                        anchors.verticalCenter: swichScreenShot.verticalCenter
                    }
                    SimpleSwich{
                        id:swichScreenShot
                        width: 30
                        height: 15
                        anchors.left: lineParset.left
                        anchors.leftMargin: parSetFirstAlignLine
                        anchors.top: swichWarn.bottom
                        anchors.topMargin: perTopMargin
                        onCheckedChanged: s_screenShotSwith(checked)
                    }
                    
                    Text {
                        id: txtScreenShotPath
                        text: qsTr("抓拍存储路径")
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.right: rectScreenShotPath.left
                        anchors.rightMargin: 20
                        anchors.verticalCenter: swichScreenShot.verticalCenter
                    }
                    Rectangle{
                        
                        id:rectScreenShotPath
                        color: "#F8FAFD"
                        border.color: "#DEDFE3"
                        border.width: 1
                        width: 200
                        height: 28
                        anchors.left: lineParset.left
                        anchors.leftMargin: parSetSecondAlignLine
                        anchors.verticalCenter: swichScreenShot.verticalCenter
                        LineEdit {
                            id: inputScreenShotPath
                            width: rectScreenShotPath.width - imgScreenShotPath.width - 22
                            height: rectScreenShotPath.height -2
                            anchors.left: parent.left
                            anchors.leftMargin: 2
                            
                            anchors.verticalCenter: rectScreenShotPath.verticalCenter
                            border.width: 0
                            text:screenv.funGetCurPath()
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
                                    fileDialog.folder = "file:///"+inputScreenShotPath.text
                                    fileDialog.pathname = "screenShotPath";
                                    fileDialog.setSelectFolder(true)
                                    fileDialog.open()
                                }
                            }
                        }
                    }
                    

                    Text {
                        id: labelSnapInter
                        text: qsTr("抓拍间隔")
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.right: cmbsnapScrrenSelect.left
                        anchors.rightMargin: 20
                        anchors.verticalCenter: cmbsnapScrrenSelect.verticalCenter
                    }

                    ListModel{
                        id:snapScrrenModel
                        ListElement{showStr:"1s"}
                        ListElement{showStr:"5s"}
                        ListElement{showStr:"10s"}
                        ListElement{showStr:"30s"}
                        ListElement{showStr:"60s"}
                    }

                    MyComBox{
                        id:cmbsnapScrrenSelect
                        width:88
                        height: 28
                        anchors.left: lineParset.left
                        anchors.leftMargin: parSetFirstAlignLine
                        anchors.top:swichScreenShot.bottom
                        anchors.topMargin: perTopMargin
                        contentBg: "#ffffff"
                        itemColorBgNor:"#FFFFFF"
                        itemColorBgHoverd: "#E7EAF1"
                        indicatorImgSrc:"qrc:/images/imgTypeSelect.png"
                        indicatorW: 9
                        indicatorH: 5
                        itemLeftMargin:0
                        itemTopMargin:0
                        itemFontColor: "#5A5E66"
                        contentFontColor: "#a6000000"
                        contentFontSize: lineeditfontSize
                        bordColor:"#DEDFE3"
                        mRadius:2
                        model: snapScrrenModel

                        onCurrentIndexChanged: {


                        }
                    }

                    Text {
                        id: txtSwichBeer
                        text: qsTr("蜂鸣开关")
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.right: swichBeer.left
                        anchors.rightMargin: 20
                        anchors.verticalCenter: swichBeer.verticalCenter
                    }
                    SimpleSwich{
                        id:swichBeer
                        width: 30
                        height: 15
                        anchors.left: lineParset.left
                        anchors.leftMargin: parSetFirstAlignLine
                        anchors.top: cmbsnapScrrenSelect.bottom
                        anchors.topMargin:perTopMargin
                        onCheckedChanged:{
                            
                            //s_beerSwith(checked)
                        }
                    }
                    
                    Text {
                        id: txtencodetype
                        text: qsTr("编码方式")
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.right: checkh264.left
                        anchors.rightMargin: 20
                        anchors.verticalCenter: checkh264.verticalCenter
                    }
                    ExclusiveGroup { id: buttonGroup }
                    SimpleCheckedButton{
                        id:checkh264
                        exclusiveGroup:buttonGroup
                        txtFont.pixelSize: fontSize
                        txtColor: fontColor
                        imgW: 12
                        imgH: 12
                        imgSrc: "qrc:/images/unselect.png"
                        imgCheckSrc: "qrc:/images/select.png"
                        anchors.left: lineParset.left
                        anchors.leftMargin: parSetFirstAlignLine
                        anchors.top: swichBeer.bottom
                        anchors.topMargin:perTopMargin
                        color: "#F8FAFD"
                        text: "H264"
                    }
                    SimpleCheckedButton{
                        id:checkh265
                        exclusiveGroup:buttonGroup
                        txtFont.pixelSize: fontSize
                        txtColor: fontColor
                        imgW: 12
                        imgH: 12
                        imgSrc: "qrc:/images/unselect.png"
                        imgCheckSrc: "qrc:/images/select.png"
                        anchors.left: checkh264.right
                        anchors.leftMargin: 20
                        anchors.verticalCenter: checkh264.verticalCenter
                        color: "#F8FAFD"
                        text: "H265"
                    }
                    
                    Text {
                        id: labelResolution
                        text: qsTr("温标选择")
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.right: cmbTempTypeSelect.left
                        anchors.rightMargin: 20
                        anchors.verticalCenter: cmbTempTypeSelect.verticalCenter
                    }

                    ListModel{
                        id:reTempTypeSelectModel
                        ListElement{showStr:"℃"}
                        ListElement{showStr:"℉"}
                    }

                    MyComBox{
                        id:cmbTempTypeSelect
                        width:88
                        height: 28
                        anchors.left: lineParset.left
                        anchors.leftMargin: parSetFirstAlignLine
                        anchors.top:checkh264.bottom
                        anchors.topMargin: perTopMargin
                        contentBg: "#ffffff"
                        itemColorBgNor:"#FFFFFF"
                        itemColorBgHoverd: "#E7EAF1"
                        indicatorImgSrc:"qrc:/images/imgTypeSelect.png"
                        indicatorW: 9
                        indicatorH: 5
                        itemLeftMargin:0
                        itemTopMargin:0
                        itemFontColor: "#5A5E66"
                        contentFontColor: "#a6000000"
                        contentFontSize: lineeditfontSize
                        bordColor:"#DEDFE3"
                        mRadius:2
                        model: reTempTypeSelectModel

                        onCurrentIndexChanged: {

                            if(cmbTempTypeSelect.currentIndex === 1){
                                // (F（华氏知度）-32)÷1.8
                                console.debug("*********    "+inputTem.text)
                                var tempStr = inputTem.text
                                console.debug("*********    "+tempStr)
                                var tempS = parseFloat(tempStr);
                                console.debug("*********    "+tempS)
                                var tempH = tempS*1.8+32
                                console.debug("*********    "+tempH)
                                inputTem.text = tempH.toFixed(2)
                            }else {
                                var tempStr1 = inputTem.text
                                var tempH1 = parseFloat(tempStr1);
                                var tempS1 = (tempH1 - 32)/1.8
                                inputTem.text = tempS1.toFixed(2)
                            }
                        }
                    }

                    
                    
                    Text {
                        id: txtRecordSet
                        text: qsTr("录像设置")
                        color: fontColor
                        font.pixelSize: fontSize
                        anchors.bottom: lineRecordSet.top
                        anchors.left: lineRecordSet.left
                        anchors.bottomMargin: 20
                    }
                    
                    Rectangle{
                        id:lineRecordSet
                        width: parent.width - 20*2
                        height: 1
                        color: "#e2e2e2"
                        anchors.top: lineWDR.bottom
                        anchors.topMargin: 238
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    
                    Text {
                        id: txtSwichRecord
                        text: qsTr("录像开关")
                        color: fontColor
                        font.pixelSize: fontSize
                        anchors.verticalCenter: swichRecord.verticalCenter
                        anchors.right: swichRecord.left
                        anchors.rightMargin: 20
                    }
                    SimpleSwich{
                        id:swichRecord
                        width: 30
                        height: 15
                        anchors.left: lineRecordSet.left
                        anchors.leftMargin: parSetFirstAlignLine
                        anchors.top: lineRecordSet.bottom
                        anchors.topMargin: firstTopMargin
                        onCheckedChanged: s_recordSwith(checked)
                    }
                    
                    Text {
                        id: txtRecortPath
                        text: qsTr("录像存储路径")
                        color: fontColor
                        font.pixelSize: fontSize
                        anchors.right: rectRecordPath.left
                        anchors.rightMargin: 20
                        anchors.verticalCenter: swichRecord.verticalCenter
                    }
                    Rectangle{
                        id:rectRecordPath
                        color: "#F8FAFD"
                        border.color: "#DEDFE3"
                        border.width: 1
                        width: 200
                        height: 28
                        anchors.left: lineRecordSet.left
                        anchors.leftMargin: parSetSecondAlignLine
                        anchors.verticalCenter: swichRecord.verticalCenter
                        LineEdit {
                            id: inputRecordPath
                            width: rectRecordPath.width - imgRecordPath.width - 22
                            height: rectRecordPath.height -2
                            anchors.left: parent.left
                            anchors.leftMargin: 2
                            
                            anchors.verticalCenter: rectRecordPath.verticalCenter
                            border.width: 0
                            text: screenv.funGetCurPath()
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
                                    fileDialog.folder = "file:///"+inputRecordPath.text
                                    fileDialog.pathname = "recordPath";
                                    fileDialog.setSelectFolder(true)
                                    fileDialog.open()
                                }
                            }
                        }
                    }
                    

                    
                    Text {
                        id: txtImageSet
                        font.pixelSize: fontSize
                        text: qsTr("图像设置")
                        color: fontColor
                        visible: true//curDevTypeStr==="J07"
                        anchors.bottom: lineImageSet.top
                        anchors.left: lineImageSet.left
                        anchors.bottomMargin: 20
                    }

                    Rectangle{
                        id:lineImageSet
                        width:parent.width - 20*2
                        height: 1
                        visible: true//curDevTypeStr==="J07"
                        color: "#e2e2e2"
                        anchors.top: lineDeviceUpdate.bottom
                        anchors.topMargin: 122
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Text {
                        id: txtImageSelect
                        font.pixelSize: fontSize
                        text: qsTr("图像选择")
                        color: fontColor
                        visible: true//curDevTypeStr==="J07"
                        anchors.right: cmbImgSelect.left
                        anchors.rightMargin: firstTopMargin
                        anchors.verticalCenter: cmbImgSelect.verticalCenter
                    }

                    MyComBox{
                        id:cmbImgSelect
                        width:88
                        height: 28
                        visible: true//curDevTypeStr==="J07"
                        anchors.left: lineImageSet.left
                        anchors.leftMargin: parSetFirstAlignLine
                        anchors.top: lineImageSet.bottom
                        anchors.topMargin: 20
                        contentBg: "#ffffff"
                        itemColorBgNor:"#FFFFFF"
                        itemColorBgHoverd: "#E7EAF1"
                        indicatorImgSrc:"qrc:/images/imgTypeSelect.png"
                        indicatorW: 9
                        indicatorH: 5
                        itemLeftMargin:0
                        itemTopMargin:0
                        itemFontColor: "#5A5E66"
                        contentFontColor: "#a6000000"
                        contentFontSize: fontSize
                        bordColor:"#DEDFE3"
                        mRadius:2
                        model: ListModel{
                            ListElement{showStr:"0"}//黑白
                            ListElement{showStr:"1"}//光谱
                            ListElement{showStr:"2"}//泰尔红紫
                            ListElement{showStr:"3"}//琥珀色
                        }

                        onCurrentIndexChanged: {
                            //                    curLanguage = currentIndex
                            //                    main.s_setLanguage(currentIndex);
                            //s_temImage(currentIndex);
                        }
                    }
                    
                    Text {
                        id: txtUpdateFile
                        font.pixelSize: fontSize
                        text: qsTr("设备升级")
                        color: fontColor
                        //  visible: curDevTypeStr==="f03"
                        anchors.bottom: lineDeviceUpdate.top
                        anchors.left: lineDeviceUpdate.left
                        anchors.bottomMargin: 20
                    }
                    Rectangle{
                        id:lineDeviceUpdate
                        width:parent.width - 20*2
                        height: 1
                        //visible: curDevTypeStr==="f03"
                        color: "#e2e2e2"
                        anchors.top: lineRecordSet.bottom
                        anchors.topMargin: 120
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    
                    Rectangle{
                        id:rectUpdatePath
                        color: "#F8FAFD"
                        border.color: "#DEDFE3"
                        border.width: 1
                        width: 200
                        height: 28
                        anchors.left: lineDeviceUpdate.left
                        anchors.leftMargin: 48
                        anchors.top: lineDeviceUpdate.bottom
                        anchors.topMargin: firstTopMargin
                        LineEdit {
                            id: inputUpdatePath
                            width: rectUpdatePath.width - imgRecordPath.width - 22
                            height: rectUpdatePath.height -2
                            anchors.left: parent.left
                            anchors.leftMargin: 2
                            anchors.verticalCenter: rectUpdatePath.verticalCenter
                            border.width: 0
                            text: ""//screenv.funGetCurPath()
                            font.pixelSize: fontSize
                            placeholderText: ""
                            isNeedDoubleClickEdit: false
                            textLeftPadding:0
                            txtColor: Qt.rgba(0,0,0,0.65)
                            color: "#F8FAFD"
                            //onTextChanged: s_recordPathSet(inputRecordPath.text)
                        }
                        
                        Image {
                            id: imgUpdatePath
                            width: 16
                            height: 13
                            source: "qrc:/images/file.png"
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            MouseArea{
                                anchors.fill: parent
                                onPressed: {
                                    imgUpdatePath.source = "qrc:/images/fileopen.png"
                                }
                                onReleased: {
                                    imgUpdatePath.source = "qrc:/images/file.png"
                                }
                                onClicked: {
                                    fileDialog.folder = "file:///"+inputUpdatePath.text
                                    fileDialog.pathname = "updatePath";
                                    fileDialog.setSelectFolder(false)
                                    fileDialog.open()
                                }
                            }
                        }
                    }
                    
                    Rectangle{
                        id:btnUpdate
                        width: txtUpdate.width +24
                        height: 28
                        color: "#3B84F6"
                        radius: 2
                        anchors.left: rectUpdatePath.right
                        anchors.leftMargin: 20
                        anchors.verticalCenter: rectUpdatePath.verticalCenter
                        Text {
                            id: txtUpdate
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            elide: Text.ElideMiddle
                            font.pixelSize: 14
                            color: "#ffffff"
                            text: qsTr("升级")
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if(screenv.funIsExitFile(inputUpdatePath.text)){
                                    askDialog.askStr = screenv.funGetFileName(inputUpdatePath.text)
                                    askDialog.curType = askDialog.fileupdate
                                    askDialog.imgSrc = "qrc:/images/icon_question.png"
                                    askDialog.width = 340
                                    askDialog.height = 176
                                    askDialog.open();
                                }
                            }
                            onPressed: btnUpdate.color = "#81C3FF"
                            onReleased: btnUpdate.color = "#3B84F6"
                        }
                    }
                    
                    

                    Text {
                        id: txtWdr
                        font.pixelSize: fontSize
                        color: fontColor
                        text: qsTr("WDR设置")

                        anchors.bottom: lineWDR.top
                        anchors.left: lineWDR.left
                        anchors.bottomMargin: 20
                    }

                    Rectangle{
                        id:lineWDR
                        width:parent.width - 20*2
                        height: 1
                        color: "#e2e2e2"
                        anchors.top: lineParset.bottom
                        anchors.topMargin: 404
                        anchors.horizontalCenter: parent.horizontalCenter
                    }


                    SimpleSwich{
                        id:swichKuandongtai
                        width: 30
                        height: 15
                        anchors.left: lineWDR.left
                        anchors.leftMargin: parSetFirstAlignLine
                        anchors.top: lineWDR.bottom
                        anchors.topMargin:12
                    }

                    Text {
                        id: txtKuandongtai
                        text: qsTr("WDR")
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.right: swichKuandongtai.left
                        anchors.rightMargin: 20
                        anchors.verticalCenter: swichKuandongtai.verticalCenter
                    }

                    Text {
                        id: txtbrightness
                        text: qsTr("亮度")
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.right: brightnessSlider.left
                        anchors.rightMargin: 20
                        anchors.verticalCenter: brightnessSlider.verticalCenter
                    }

                    Slider {
                        id:brightnessSlider
                        width: 200
                        height:20
                        anchors.left: lineWDR.left
                        anchors.leftMargin: parSetFirstAlignLine
                        anchors.top: swichKuandongtai.bottom
                        anchors.topMargin: perTopMargin
                        stepSize:1
                        minimumValue: 0
                        maximumValue: 100
                        value: 0

                        onValueChanged: imagparambrightness = brightnessSlider.value
                        style: SliderStyle {
                            groove: Rectangle {
                                implicitWidth: 200
                                implicitHeight: 10
                                color: "#DFE1E6"
                                radius: 5
                                Rectangle{
                                    id:sliderrect1
                                    anchors.left: parent.left
                                    width: parent.width * brightnessSlider.value/brightnessSlider.maximumValue
                                    height: parent.height
                                    color: "#8AB8FF"
                                    radius: 5
                                }
                                Rectangle{
                                    id:sliderrect2
                                    anchors.left: sliderrect1.right
                                    width: parent.width - sliderrect1.width
                                    height: parent.height
                                    color: "transparent"
                                    radius: 5
                                }
                            }
                            handle:/*Rectangle {
                                anchors.centerIn: parent
                                color: control.pressed ? "white" : "lightgray"
                                implicitWidth: 14
                                implicitHeight: 14*/
                                   Image {
                                id: imgSlider
                                width: 14
                                height: 14
                                source: "qrc:/images/slider.png"
                            }

                        }
                    }

                    Text {
                        id: txtbrightnessvalue
                        text: brightnessSlider.value
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.left: brightnessSlider.right
                        anchors.leftMargin: 10
                        anchors.verticalCenter: brightnessSlider.verticalCenter
                    }


                    Text {
                        id: txthue
                        text: qsTr("色度")
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.right: hueSlider.left
                        anchors.rightMargin: 20
                        anchors.verticalCenter: hueSlider.verticalCenter
                    }

                    Slider {
                        id:hueSlider
                        width: 200
                        height:20
                        anchors.left: lineWDR.left
                        anchors.leftMargin: parSetFirstAlignLine
                        anchors.top: brightnessSlider.bottom
                        anchors.topMargin: perTopMargin
                        stepSize:1
                        minimumValue: 0
                        maximumValue: 100
                        value: 0
                        onValueChanged: imagparamhue = hueSlider.value
                        style: SliderStyle {
                            groove: Rectangle {
                                implicitWidth: 200
                                implicitHeight: 10
                                color: "#DFE1E6"
                                radius: 5
                                Rectangle{
                                    id:sliderrect3
                                    anchors.left: parent.left
                                    width: parent.width * hueSlider.value/hueSlider.maximumValue
                                    height: parent.height
                                    color: "#8AB8FF"
                                    radius: 5
                                }
                                Rectangle{
                                    id:sliderrect4
                                    anchors.left: sliderrect3.right
                                    width: parent.width - sliderrect3.width
                                    height: parent.height
                                    color: "transparent"
                                    radius: 5
                                }
                            }
                            handle:/*Rectangle {
                                anchors.centerIn: parent
                                color: control.pressed ? "white" : "lightgray"
                                implicitWidth: 14
                                implicitHeight: 14*/
                                   Image {
                                width: 14
                                height: 14
                                source: "qrc:/images/slider.png"
                            }

                        }
                    }
                    Text {
                        id: txthuevalue
                        text: hueSlider.value
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.left: hueSlider.right
                        anchors.leftMargin: 10
                        anchors.verticalCenter: hueSlider.verticalCenter
                    }

                    Text {
                        id: txtcontrast
                        text: qsTr("对比度")
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.right: contrastSlider.left
                        anchors.rightMargin: 20
                        anchors.verticalCenter: contrastSlider.verticalCenter
                    }

                    Slider {
                        id:contrastSlider
                        width: 200
                        height:20
                        anchors.left: lineWDR.left
                        anchors.leftMargin: parSetFirstAlignLine
                        anchors.top: hueSlider.bottom
                        anchors.topMargin: perTopMargin
                        stepSize:1
                        minimumValue: 0
                        maximumValue: 100
                        value: 0
                        onValueChanged: imagparamcontrast = contrastSlider.value
                        style: SliderStyle {
                            groove: Rectangle {
                                implicitWidth: 200
                                implicitHeight: 10
                                color: "#DFE1E6"
                                radius: 2
                                Rectangle{
                                    id:sliderrect5
                                    anchors.left: parent.left
                                    width:  parent.width * contrastSlider.value/contrastSlider.maximumValue
                                    height: parent.height
                                    color: "#8AB8FF"
                                    radius: 5
                                }
                                Rectangle{
                                    id:sliderrect6
                                    anchors.left: sliderrect5.right
                                    width: parent.width - sliderrect5.width
                                    height: parent.height
                                    color: "transparent"
                                    radius: 5
                                }
                            }
                            handle:/*Rectangle {
                                anchors.centerIn: parent
                                color: control.pressed ? "white" : "lightgray"
                                implicitWidth: 14
                                implicitHeight: 14*/
                                   Image {
                                width: 14
                                height: 14
                                source: "qrc:/images/slider.png"
                            }

                        }
                    }

                    Text {
                        id: txtcontrastvalue
                        text: contrastSlider.value
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.left: contrastSlider.right
                        anchors.leftMargin: 10
                        anchors.verticalCenter: contrastSlider.verticalCenter
                    }

                    Text {
                        id: txtcolorsaturation
                        text: qsTr("饱和度")
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.right: colorsaturationSlider.left
                        anchors.rightMargin: 20
                        anchors.verticalCenter: colorsaturationSlider.verticalCenter
                    }

                    Slider {
                        id:colorsaturationSlider
                        width: 200
                        height:20
                        anchors.left: lineWDR.left
                        anchors.leftMargin: parSetFirstAlignLine
                        anchors.top: contrastSlider.bottom
                        anchors.topMargin: perTopMargin

                        stepSize:1
                        minimumValue: 0
                        maximumValue: 100
                        value: 0
                        onValueChanged: imagparamcolorsaturation = colorsaturationSlider.value
                        style: SliderStyle {
                            groove: Rectangle {
                                implicitWidth: 200
                                implicitHeight: 10
                                color: "#DFE1E6"
                                radius: 5
                                Rectangle{
                                    id:sliderrect7
                                    anchors.left: parent.left
                                    width:  parent.width * colorsaturationSlider.value/colorsaturationSlider.maximumValue
                                    height: parent.height
                                    color: "#8AB8FF"
                                    radius: 5
                                }
                                Rectangle{
                                    id:sliderrect8
                                    anchors.left: sliderrect7.right
                                    width: parent.width - sliderrect7.width
                                    height: parent.height
                                    radius: 5
                                    color: "transparent"
                                }
                            }
                            handle:/*Rectangle {
                                anchors.centerIn: parent
                                color: control.pressed ? "white" : "lightgray"
                                implicitWidth: 14
                                implicitHeight: 14*/
                                   Image {

                                width: 14
                                height: 14
                                source: "qrc:/images/slider.png"
                            }

                        }


                    }


                    Text {
                        id: txtcolorsaturationvalue
                        text: colorsaturationSlider.value
                        font.pixelSize: fontSize
                        color: fontColor
                        anchors.left: colorsaturationSlider.right
                        anchors.leftMargin: 10
                        anchors.verticalCenter: colorsaturationSlider.verticalCenter
                    }
                    
                    Rectangle{
                        id:btnSave
                        width: txtSave.width +24
                        height: 28
                        color: "#3B84F6"
                        radius: 2
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 10
                        Text {
                            id: txtSave
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            elide: Text.ElideMiddle
                            font.pixelSize: 14
                            color: "#ffffff"
                            text: qsTr("设置")
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                iradInfoSet();
                            }
                            onPressed: btnSave.color = "#81C3FF"
                            onReleased: btnSave.color = "#3B84F6"
                        }
                    }
                }
            }
        }
    }


    
    Rectangle{
        id:mDeviceStateBar
        width: parent.width
        height:50
        anchors.bottom: parent.bottom
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#5D9CFF"}
            GradientStop { position: 1.0; color: "#2D76E7"}
        }
    }
    
    Connections{
        target: askDialog
        onS_CurTypeMsg:{
            if(askDialog.fileupdate === type){
                s_deviceUpdate();
            }
        }
    }
    
    UpdateProgressC{
        id:updateprogress
        onSignal_updateState:{
            progressdialog.close();
            if(isSucc){
                
            }else{
            }
        }
        onSignal_updateProgress:progressdialog.setProgressValue(progressvalue)
    }
    
    function updateDevice(did,url,path){
        progressdialog.open();
        updateprogress.startupLoad(did,url,inputUpdatePath.text)
    }
    
    ProgressDialog{
        id:progressdialog
        width: 500
        height: 193
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
            }else if(pathname === "updatePath"){
                inputUpdatePath.text = fileDialog.fileUrl.toString().replace('file:///','');
                //devicemanagerment.screenShotPath = txtscreenshotSavePath.text
            }
        }
        onRejected: {
        }
    }
    
    //            dataObj.insert("osdenable",value.value("osdenable").toInt());
    //            alarmparamObj.insert("alarmtempEnable", value.value("alarmtempEnable").toInt());
    //            alarmparamObj.insert("alarmtemp", value.value("alarmtemp").toDouble());
    //            ctrlparamObj.insert("tempdrift", value.value("tempdrift").toInt());
    //            ctrlparamObj.insert("tempcontrol", value.value("tempcontrol").toInt());
    //            dataObj.insert("alarmparam", QJsonValue(alarmparamObj));
    //            dataObj.insert("ctrlparam", QJsonValue(ctrlparamObj));
    
    
    
    function toastTimeSynchronization(){

        loaderToast.sourceComponent = null
        loaderToast.sourceComponent = toastTimeSynchronizationTip

    }
    function setWdr(value){
        imagparamwdr = value
        swichKuandongtai.checked = value
    }

    function setBrightness(value){
        brightnessSlider.value = value
    }

    function setHue(value){
        hueSlider.value = value
    }

    function setContrast(value)
    {
        contrastSlider.value = value
    }

    function setColorsaturation(value)
    {
        colorsaturationSlider.value = value
    }
    
    function getiradInfo(){
        var map ={cmd:"getiradinfo"}
        s_sendcommoncmd(map);
        
        map.cmd = "getalarmparam"
        s_sendcommoncmd(map);

        map.cmd = "getvideoencodeparam";
        s_sendcommoncmd(map);

        map.cmd = "getimagparam"
        s_sendcommoncmd(map);

        map.cmd = "getinftempcolor"
        s_sendcommoncmd(map);

    }

    function setTempcolor(value)
    {

        console.debug("********** setTempcolor  "+value)
        if(value === 1)
            cmbImgSelect.currentIndex = 0
        else if(value === 102)
            cmbImgSelect.currentIndex = 1
        else if(value === 104)
            cmbImgSelect.currentIndex = 2
        else if(value === 105)
            cmbImgSelect.currentIndex = 3
    }
    
    //参数修改设置 全在此函数
    function iradInfoSet(){

        var osdenableV = 0;
        //        if(swithTime.checked)
        //            osdenableV = 1
        //        else
        //            osdenableV = 0
        var alarmtempEnableV;
        
        if(swichWarn.checked)
            alarmtempEnableV = 1;
        else
            alarmtempEnableV = 0;
        

        var pusht ;
        if(cmbsnapScrrenSelect.currentIndex === 0)
            pusht = 1;
        else if(cmbsnapScrrenSelect.currentIndex === 1)
            pusht = 5;
        else if(cmbsnapScrrenSelect.currentIndex === 2)
            pusht = 10;
        else if(cmbsnapScrrenSelect.currentIndex === 3)
            pusht = 30;
        else if(cmbsnapScrrenSelect.currentIndex === 4)
            pusht = 60;

        var map ={
            osdenable:osdenableV,
            alarmtempEnable:alarmtempEnableV,
            alarmtemp:inputTem.text,
            tempdrift:inputTempDrift.text,
            tempcontrol:inputTempMin.text,
            tempdisplay:cmbTempTypeSelect.currentIndex,
            pushtime:pusht,
            cmd:"setiradinfo"}


        s_sendcommoncmd(map);
        
        
        var typeStr;
        if(checkh264.checked)
            typeStr = "h264"
        if(checkh265.checked)
            typeStr = "h265"
        var map1 = {
            cmd:"setvideoencodeparam",
            encoding:typeStr
        }
        s_sendcommoncmd(map1);
        
        var kuandongt = swichKuandongtai.checked?1:0
        if(imagparamwdr !== kuandongt)
            wdrisChange = true
        else
            wdrisChange = false

        
        var map2 ={
            cmd:"setimagparam",
            wdr:swichKuandongtai.checked?1:0,
            mirror:imagparammirror,
            flip:imagparamflip,
            brightness:imagparambrightness,
            colorsaturation:imagparamcolorsaturation,
            contrast:imagparamcontrast,
            hue:imagparamhue,
            sharpness:imagparamsharpness
        }
        s_sendcommoncmd(map2);

        var tmpColor1 = 0;

        if(cmbImgSelect.currentIndex === 0)
            tmpColor1 = 1
        else if(cmbImgSelect.currentIndex === 1)
            tmpColor1 = 102
        else if(cmbImgSelect.currentIndex === 2)
            tmpColor1 = 104
        else if(cmbImgSelect.currentIndex === 3)
            tmpColor1 = 105

        var map3 = {
            cmd:"setinftempcolor",
            tmpColor:tmpColor1
        }
        s_sendcommoncmd(map3);
        

        s_beerSwith(swichBeer.checked)
        s_warnSwith(swichWarn.checked)
        //s_timeSwith(swithTime.checked)
    }

    function setSwitchTempdisplay(mvalue)
    {
        cmbTempTypeSelect.currentIndex = mvalue
    }
    

    function getUpdateFilePath(){
        return setting.updateFilePath;
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
    function setTemDrift(mvalue){
        inputTempDrift.text = mvalue;
    }
    
    
    function getWarnTem(){
        return setting.warnTem
    }
    function setWarnTem(mvalue){
        inputTem.text = mvalue
    }
    
    function setSwitchBeer(mvalue){
        swichBeer.checked = mvalue
    }
    
    function getSwitchWarn()
    {
        return setting.switchWarn;
    }
    
    function setPushtime(mvalue){

        if(mvalue === 1)
            cmbsnapScrrenSelect.currentIndex = 0
        else if(mvalue === 5)
            cmbsnapScrrenSelect.currentIndex = 1
        else if(mvalue === 10)
            cmbsnapScrrenSelect.currentIndex = 2
        else if(mvalue === 30)
            cmbsnapScrrenSelect.currentIndex = 3
        else if(mvalue === 60)
            cmbsnapScrrenSelect.currentIndex = 4
    }
    function setSwitchWarn(mvalue)
    {
        swichWarn.checked = mvalue
    }

    function getSwitchTime(){
        return setting.switchTime
    }
    
    function setSwitchTime(mvalue){
        //swithTime.checked = mvalue
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
    
    function setTempContrl(mavlue){
        inputTempMin.text = mavlue
    }
    
    function setTcpip(value){
        setting.tcpip = value
    }
    function setIsOpenAdjustRect(value){
        setting.isOpenAdjustRect = value
    }

    function setRedRect(pw,ph,rx,ry,rw,rh){
        
        if(curDevTypeStr === "e03"){
            setting.e03showParentW = pw
            setting.e03showParentH = ph
            setting.e03showRectX = rx
            setting.e03showRectY = ry
            setting.e03showRectW = rw
            setting.e03showRectH = rh
        }else if(curDevTypeStr === "f03"){
            setting.f03showParentW = pw
            setting.f03showParentH = ph
            setting.f03showRectX = rx
            setting.f03showRectY = ry
            setting.f03showRectW = rw
            setting.f03showRectH = rh
        }else if(curDevTypeStr === "d04"){
            setting.d04showParentW = pw
            setting.d04showParentH = ph
            setting.d04showRectX = rx
            setting.d04showRectY = ry
            setting.d04showRectW = rw
            setting.d04showRectH = rh
        }else if(curDevTypeStr === "d06"){
            setting.d06showParentW = pw
            setting.d06showParentH = ph
            setting.d06showRectX = rx
            setting.d06showRectY = ry
            setting.d06showRectW = rw
            setting.d06showRectH = rh
        }
    }
    
    
    function getTcpip(){
        return setting.tcpip
    }
    function getIsOpenAdjustRect(){
        return setting.isOpenAdjustRect
    }
    function getShowRectX(){
        
        if(curDevTypeStr === "e03"){
            return setting.e03showRectX
        }else if(curDevTypeStr === "f03"){
            return setting.f03showRectX
        }else if(curDevTypeStr === "d04"){
            return setting.d04showRectX
        }else if(curDevTypeStr === "d06"){
            return setting.d06showRectX
        }
    }
    function getShowRectY(){
        if(curDevTypeStr === "e03"){
            return setting.e03showRectY
        }else if(curDevTypeStr === "f03"){
            return setting.f03showRectY
        }else if(curDevTypeStr === "d04"){
            return setting.d04showRectY
        }else if(curDevTypeStr === "d06"){
            return setting.d06showRectY
        }
        
    }
    function getShowRectW(){
        if(curDevTypeStr === "e03"){
            return setting.e03showRectW
        }else if(curDevTypeStr === "f03"){
            return setting.f03showRectW
        }else if(curDevTypeStr === "d04"){
            return setting.d04showRectW
        }else if(curDevTypeStr === "d06"){
            return setting.d06showRectW
        }
    }

    function getShowRectH(){
        if(curDevTypeStr === "e03"){
            return setting.e03showRectH
        }else if(curDevTypeStr === "f03"){
            return setting.f03showRectH
        }else if(curDevTypeStr === "d04"){
            return setting.d04showRectH
        }else if(curDevTypeStr === "d06"){
            return setting.d06showRectH
        }
    }

    function getShowParentW(){
        if(curDevTypeStr === "e03"){
            return setting.e03showParentW
        }else if(curDevTypeStr === "f03"){
            return setting.f03showParentW
        }else if(curDevTypeStr === "d04"){
            return setting.d04showParentW
        }else if(curDevTypeStr === "d06"){
            return setting.d06showParentW
        }
    }

    function getShowParentH(){
        if(curDevTypeStr === "e03"){
            return setting.e03showParentH
        }else if(curDevTypeStr === "f03"){
            return setting.f03showParentH
        }else if(curDevTypeStr === "d04"){
            return setting.d04showParentH
        }else if(curDevTypeStr === "d06"){
            return setting.d06showParentH
        }
    }

    
    function setVideoType(typeStr){
        
        if(typeStr === "h264")
            checkh264.checked = true;
        else if(typeStr === "h265")
            checkh265.checked = true;
    }
    
    function getVideoType(){
        
        if(checkh264.checked)
            return "h264"
        if(checkh265.checked)
            return "h265"
    }
    
    

    Connections{
        target: main
        onS_setLanguage:setLanguage(typeL);
    }
    
    function setLanguage(type){
        
        switch(type){
        case lEnglish:
            txtRecordSet.text = "Video Settings"
            txtRecortPath.text = "Storage Path"
            txtScreenShotPath.text = "Storage Path"
            txtWarnTemSet.text = "Alarm Temperature"
            labelSwitchTime.text = "Time"
            labelTime.text = "Time OSD"
            txtparset.text = "Parameter Settings"
            txtSwichBeer.text = "Buzzer"
            txtSwichRecord.text = "Video"
            txtSwichScreenShot.text = "Snapshot"
            txtSwichWarn.text = "Alarm"
            txtTempMin.text = "Temperature control valve"
            txtTempDrift.text = "Temperature drift setting"
            txtUpdate.text = "Upgrade"
            txtUpdateFile.text = "Upgrade"
            txtSave.text = "Save"
            txtencodetype.text = "Encoding Style"
            txtImageSet.text = "Image settings"
            txtImageSelect.text = "Image selectione"
            labelResolution.text = "Standard"

            //txtTip.text = "Synchronized Successfully"
            labelSwitchTime.text = "Time Sync"
            labelTime.text = "Time Settings"
            labelSnapInter.text = "Snapshot Interval"
            txtbrightness.text = "Brightness"
            txtcolorsaturation.text = "Saturation"
            txtcontrast.text = "Contrast"
            txthue.text = "Chroma"
            txtWdr.text = "WDR Settings"
            break;
        case lKorean:
            txtRecordSet.text = "비디오 설정"
            txtRecortPath.text = "저장 경로"
            txtScreenShotPath.text = "저장 경로"
            txtWarnTemSet.text = "알람온도설정"
            labelSwitchTime.text = "시간스위치 "
            labelTime.text = "시간표시"
            txtparset.text = "파라미터 설정"
            txtSwichBeer.text = "알람 경보음"
            txtSwichRecord.text = "비디오"
            txtSwichScreenShot.text = "사진저장"
            txtSwichWarn.text = "알람"
            txtTempMin.text = "최대/최소 온도"
            txtTempDrift.text = "온도변화폭 설정"
            txtUpdate.text = "업그레이드"
            txtUpdateFile.text = "업그레이드"
            txtSave.text = "설정"

            txtImageSet.text = "Image settings"
            txtImageSelect.text = "Image selectione"

            break;
        case lItaly:
            txtRecordSet.text = "Settaggio Video"
            txtRecortPath.text = "Directory Video"
            txtScreenShotPath.text = "Directory Fotografie"
            txtWarnTemSet.text = "Allarme Temperatura"
            labelSwitchTime.text = "Ora"
            labelTime.text = "Ora OSD"
            txtparset.text = "Settaggio Parametri"
            txtSwichBeer.text = "Sirena"
            txtSwichRecord.text = "Tasto Video"
            txtSwichScreenShot.text = "Fotografia"
            txtSwichWarn.text = "Allarme"
            txtTempMin.text = "Max Min Temperatura"
            txtTempDrift.text = "Correzione Temperatura"
            txtUpdate.text = "aggiornare"
            txtUpdateFile.text = "aggiornare"
            txtSave.text = "Settaggio"

            txtImageSet.text = "Image settings"
            txtImageSelect.text = "Image selectione"

            break;
        case lChinese:
            txtRecordSet.text = "录像设置"
            txtRecortPath.text = "录像存储路径"
            txtScreenShotPath.text = "抓拍存储路径"
            txtWarnTemSet.text = "告警温度设置"
            labelSwitchTime.text = "时间开关"
            labelTime.text = "时间显示"
            txtparset.text = "参数设置"
            txtSwichBeer.text = "蜂鸣开关"
            txtSwichRecord.text = "录像开关"
            txtSwichScreenShot.text = "抓拍开关"
            txtSwichWarn.text = "报警开关"
            txtTempMin.text = "温度控制阀"
            txtTempDrift.text = "温漂设置"
            txtUpdate.text = "升级"
            txtUpdateFile.text = "设备升级"
            txtSave.text = "设置"
            txtencodetype.text = "编码方式"
            labelResolution.text = "温标选择"
            txtImageSet.text = "Image settings"
            txtImageSelect.text = "Image selectione"
            //txtTip.text = "同步成功"
            labelSwitchTime.text = "时间同步"
            labelTime.text = "时间设置"
            labelSnapInter.text = "抓拍间隔"
            txtbrightness.text = "亮度"
            txtcolorsaturation.text = "饱和度"
            txtcontrast.text = "对比度"
            txthue.text = "色度"
            txtWdr.text = "WDR设置"
            break;
        case lRussian:
            txtRecordSet.text = "Настройка записи"
            txtRecortPath.text = "Путь хранения видео"
            txtScreenShotPath.text = "Путь сохранения снимка"
            txtWarnTemSet.text = "Температура тревоги"
            labelSwitchTime.text = "Время"
            labelTime.text = "Время OSD"
            txtparset.text = "Настройка параметров"
            txtSwichBeer.text = "Звуковой сигнал"
            txtSwichRecord.text = "Запись"
            txtSwichScreenShot.text = "Снимок"
            txtSwichWarn.text = "Тревога"
            txtTempMin.text = "Max Min температура"
            txtTempDrift.text = "Коррекция температуры"
            txtUpdate.text = "обновить"
            txtUpdateFile.text = "обновить"
            txtSave.text = "Настройка"

            txtImageSet.text = "Image settings"
            txtImageSelect.text = "Image selectione"

            break;
        case lLithuanian:
            txtRecordSet.text = "Vaizdo parametrų nustatymas"
            txtRecortPath.text = "Vaizdo įrašo saugojimo kelias "
            txtScreenShotPath.text = "Momentinių nuotraukų išsaugojimo kelias"
            txtWarnTemSet.text = "Temperatūros aliarmas"
            labelSwitchTime.text = "Rodymas"
            labelTime.text = "Laiko rodymas"
            txtparset.text = "Parametrų nustatymai"
            txtSwichBeer.text = "Sirena"
            txtSwichRecord.text = "Įrašyti vaizdą"
            txtSwichScreenShot.text = "Nuotrauka "
            txtSwichWarn.text = "Aliarmas"
            txtTempMin.text = "Temperatūros ribų nustatymas"
            txtTempDrift.text = "Temperatūrinio dreifo nustatymas "
            txtUpdate.text = "patobulinti"
            txtUpdateFile.text = "patobulinti"
            txtSave.text = "nustatymas"

            txtImageSet.text = "Image settings"
            txtImageSelect.text = "Image selectione"

            break;
        }
    }
    
}
