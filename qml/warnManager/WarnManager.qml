
import QtQuick 2.0
import WarnModel 1.0
import "../playbackVideo"
Rectangle {
    id: root
    property int checkedHeaderLeftMargin: 20
    property int warnTimeHeaderLeftMargin: 44
    property int warnTempHeaderLeftMargin: 304
    property int warnImgHeaderLeftMargin: 478
    property int deleteHeaderLeftMargin: 771
    property int fontSize: 14




    WarnModel{
        id:warnmodel
        // Component.objectName: warnmodel.flushWarnInfo(deviceconfig.getScrennShotPath());
    }

    Rectangle{
        id:rectWarnArea
        width: parent.width-40
        height: parent.height-40
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#F8FAFD"


        //第一排控件
        Text {
            id: textitle
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 60
            anchors.topMargin: 35
            font.pixelSize: 18
            text: qsTr("日志列表")
        }

        Rectangle{
            id:rectBatch
            width: 96
            height: 36
            color: "#3B84F6"
            radius: 4
            anchors.left: textitle.right
            anchors.leftMargin: 39
            anchors.verticalCenter: textitle.verticalCenter

            Image {
                id: imgBatchDelete
                width: 14
                height: 15
                anchors.left: parent.left
                anchors.leftMargin: 9
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/images/imgDelete.png"
            }

            Text {
                id: txtBatchDelete
                font.pixelSize: fontSize
                anchors.left: imgBatchDelete.right
                anchors.leftMargin: 9
                anchors.verticalCenter: parent.verticalCenter
                color: "#ffffff"
                text: qsTr("批量删除")
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    console.debug("shacnhu")
                    warnList.currentIndex = 3;}
            }
        }

        //日期选择
        Rectangle{

            id:dateRect
            color: "#ffffff"
            border.width: 1
            border.color: "#DCDFE6"
            radius: 4
            width: 190
            height: 34
            anchors.verticalCenter: rectBatch.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 190

            Image {
                id: imgdate
                width: 14
                height: 13
                source: "qrc:/images/data1.png"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 10
            }

            Text {
                id: txtDate
                anchors.left: imgdate.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: fontSize
                color: "#909399"
                text:""
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onPressed: {
                    calendar.open()
                }
                onEntered: imgdate.source = "qrc:/images/data1.png"
                onReleased: imgdate.source = "qrc:/images/data1.png"
            }

        }




        //时间选择
        Rectangle{

            id:timeRect
            color: "#ffffff"
            border.width: 1
            border.color: "#DCDFE6"
            radius: 4
            width: 120
            height: 34
            anchors.left: dateRect.right
            anchors.verticalCenter: rectBatch.verticalCenter
            anchors.leftMargin: 10

            Image {
                id: imgtime
                width: 12
                height: 12
                source: "qrc:/images/time.png"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 10
            }

            Text {
                id: txttime
                anchors.left: imgtime.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: fontSize
                color: "#909399"
                text: qsTr("")
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onPressed: {
                    selecttime.open();
                }
                onEntered: imgtime.source = "qrc:/images/time.png"
                onReleased: imgtime.source = "qrc:/images/time.png"
            }

        }

        //警报列表
        Rectangle{
            id:warnHeader
            width: parent.width -120
            height: 50
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 84
            color: "#EEF3FA"
            Image{
                id:imgSelect
                width: 14
                height: 14
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: checkedHeaderLeftMargin
                source: "qrc:/images/btnSelect.png"
            }
            Text {
                id: txtWarnTime
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: warnTimeHeaderLeftMargin
                font.pixelSize: fontSize
                text: qsTr("告警时间")
            }
            Text {
                id: txtWarnTemp
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: warnTempHeaderLeftMargin
                font.pixelSize: fontSize
                text: qsTr("告警温度")
            }
            Text {
                id: warnImg
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: warnImgHeaderLeftMargin
                font.pixelSize: fontSize
                text: qsTr("抓拍图片")
            }
            Text {
                id: txtDelete
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: deleteHeaderLeftMargin
                font.pixelSize: fontSize
                text: qsTr("操作")
            }
        }
        ListView{
            id:warnList
            width: warnHeader.width
            height: parent.height - 210 -warnHeader.height
            anchors.top: warnHeader.bottom
            anchors.left: warnHeader.left
            model: warnmodel
            delegate: Rectangle{
                property bool enter: false
                width: parent.width
                height: 59
                color:warnList.currentIndex === index?"#8AB8FF":(enter?"#EEF3FA":"#F8FAFD")
                Image{
                    id:select
                    width: 14
                    height: 14
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: checkedHeaderLeftMargin
                    source: "qrc:/images/btnSelect.png"
                }
                Text {
                    id: warnTime
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: warnTimeHeaderLeftMargin
                    font.pixelSize: fontSize
                    text: model.warnTime;
                }
                Text {
                    id: warnTemp
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: warnTempHeaderLeftMargin
                    font.pixelSize: fontSize
                    text: model.warnTemp;
                }

                Image {
                    id: captureImg
                    width: 13
                    height: 14
                    anchors.left: parent.left
                    anchors.leftMargin: warnImgHeaderLeftMargin
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/images/btnSelect.png"
                }
                Text {
                    id: captureTxt

                    anchors.left: captureImg.right
                    anchors.leftMargin: 7
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: fontSize
                    text: model.imgName;
                }


                Text {
                    id: deletetxt
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: deleteHeaderLeftMargin
                    font.pixelSize: fontSize
                    color: "#3B84F6"
                    text: qsTr("删除")
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            warnmodel.funDeleteIndex(index);
                        }
                    }
                }

                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    propagateComposedEvents:true
                    onEntered:enter = true
                    onExited: enter = false;
                    onClicked: {

                        warnList.currentIndex = index;
                         mouse.accepted = false
                    }
                }
            }
        }
        MyCalendar{
            id:calendar
            width: 280
            height: 314
            dim:false
            x:dateRect.x
            y:67
            onS_dayChange:warnmodel.funFlushWarnInfo(deviceconfig.getScrennShotPath(),value);

            onS_dayChange1: txtDate.text = value


            //onS_mouthChange:getRecordInfo(1,value)

            onS_yearChange: ;

            // Component.onCompleted:getRecordInfo(2, Qt.formatDate(calendar.getCurrentData(),"yyyyMMdd000000"))
        }

        SelectTime{
            id:selecttime
            x: timeRect.x
            y:timeRect.y + timeRect.height + 4
            dim:false
            width: 120
            height: 245
            onS_ensure: {
                var timeStr = timeh+":"+timem+":"+times
                txttime.text = timeStr
                var curIndex = warnmodel.funFindIndex(timeh,timem,times)
                warnList.currentIndex = curIndex
            }

        }
    }


}
