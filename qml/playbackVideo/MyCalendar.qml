import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.5
import "../simpleControl"
Popup {
    id: root


    modal: true
    focus: true
    //设置窗口关闭方式为按“Esc”键关闭
    closePolicy: Popup.CloseOnEscape|Popup.CloseOnPressOutside
    //设置窗口的背景控件，不设置的话Popup的边框会显示出来
    background: rect


    property color fontColor: "#333333"
    property color bgColor: "#ffffff"

    signal s_yearChange(var value)
    signal s_mouthChange(var value)
    signal s_dayChange(var value)

    Rectangle {
        id: rect
        anchors.fill: parent
        color: bgColor
        radius: 4
        Calendar{
            id:the_calendar
            width: parent.width
            height: parent.height - bottomRect.height
            anchors.margins: 2
            style: CalendarStyle {
                gridColor: "transparent"
                //gridVisible: false

                background: Rectangle{
                    id:background
                    anchors.fill: parent
                    color:bgColor
                }

                //标题年月
                navigationBar:Item{
                    //color: "transparent"
                    height: 39


                    QmlImageButton {
                        id: yearPre

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 16
                        width: 11
                        height: 11
                        imgSourseNormal: "qrc:/images/yearpre.png"
                        imgSoursePress:"qrc:/images/yearpre_p.png"
                        imgSourseHover: imgSoursePress
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                control.showPreviousYear()

                                s_yearChange(""+the_calendar.visibleYear + (the_calendar.visibleMonth+1)+"00000000")
                            }
                        }
                    }

                    QmlImageButton {
                        id: yearNext

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 16
                        width: 11
                        height: 11
                        imgSourseNormal: "qrc:/images/yearnext.png"
                        imgSoursePress:"qrc:/images/yearnext_p.png"
                        imgSourseHover: imgSoursePress
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                control.showNextYear()
                                s_yearChange(""+the_calendar.visibleYear + (the_calendar.visibleMonth+1)+"00000000")
                            }
                        }
                    }
                    QmlImageButton {
                        id: mouthPre

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 38
                        width: 7
                        height: 11
                        imgSourseNormal: "qrc:/images/mouthpre.png"
                        imgSoursePress:"qrc:/images/mouthpre_p.png"
                        imgSourseHover:imgSoursePress
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                control.showPreviousMonth()
                                s_mouthChange(""+the_calendar.visibleYear + (the_calendar.visibleMonth+1)+"00000000")
                            }
                        }
                    }
                    QmlImageButton {
                        id: mouthNext

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 38
                        width: 7
                        height: 11
                        imgSourseNormal: "qrc:/images/mouthnext.png"
                        imgSoursePress:"qrc:/images/mouthnext_p.png"
                        imgSourseHover:imgSoursePress
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                control.showNextMonth()
                                s_mouthChange(""+the_calendar.visibleYear + (the_calendar.visibleMonth+1)+"00000000")
                            }
                        }
                    }

                    Rectangle{

                        id:dateShowRect
                        width: labelYear.width + labelMonth.width
                        height: parent.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: bgColor
                        Label {
                            id: labelYear
                            anchors.verticalCenter: parent.verticalCenter

                            //text: the_calendar.selectedDate.getFullYear()+qsTr('年')
                            text:control.visibleYear+qsTr('年')
                            //elide: Text.ElideRight
                            horizontalAlignment: Text.AlignLeft
                            font.pixelSize: 14

                            color: fontColor
                        }

                        Label {
                            id: labelMonth
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: labelYear.right
                            anchors.leftMargin: 10
                            //注意Date原本的月份是0开始
                            text: (control.visibleMonth+1)+qsTr('月')
                            //elide: Text.ElideRight
                            horizontalAlignment: Text.AlignRight
                            font.pixelSize: 14

                            color:fontColor
                        }
                    }


                    Rectangle{
                        width: parent.width
                        height: 1
                        anchors.bottom: parent.bottom
                        color: Qt.rgba(255,255,255,0.09)
                    }
                }
                //星期
                dayOfWeekDelegate: Item{
                    //color: "transparent"
                    height: the_calendar.height/8
                    Label {
                        text: control.__locale.dayName(styleData.dayOfWeek, control.dayOfWeekFormat)
                        anchors.centerIn: parent
                        color: fontColor
                        font.pixelSize: 14

                    }
                }
                //日期
                dayDelegate: Rectangle {
                    //选中-当月未选中-其他
                    //                color: styleData.selected
                    //                       ?Qt.rgba(52/255,142/255,145/255,1)
                    //                       : (styleData.visibleMonth && styleData.valid
                    //                          ?Qt.rgba(6/255,45/255,51/255,1)
                    //                          : Qt.rgba(3/255,28/255,35/255,1));
                    //color: styleData.selected?"#409EFF":((styleData.visibleMonth && styleData.valid)?(calendarEventModel.getDateEvent(styleData.date)):"#191919")
                    color: bgColor//(styleData.visibleMonth && styleData.valid)?(calendarEventModel.getDateEvent(styleData.date)):bgColor
                    border.width: 2
                    border.color: styleData.selected?"#21e9cc":"transparent"
                    Label {
                        text: styleData.date.getDate()
                        anchors.centerIn: parent
                        font.pixelSize: 14

                        //                    color: styleData.valid
                        //                           ?Qt.rgba(197/255,1,1,1)
                        //                           : Qt.rgba(16/255,100/255,100/255,1)
                        color: styleData.today?"red":((styleData.visibleMonth && styleData.valid)?fontColor:Qt.rgba(0,0,0,0.25))
                        //color:styleData.visibleDay?"red":"white"
                    }
                }

            }
        }


        Rectangle{
            id:bottomRect
            width: parent.width
            height: 40
            color: bgColor
            anchors.bottom: parent.bottom
            Text {
                id: txtToday
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 10
                color: fontColor
                text: Qt.formatDate(the_calendar.selectedDate,"yyyy-MM-dd")
            }

            QmlButton{
                width: 44
                height: 22

                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 16
                colorNor:"#409EFF"
                colorPressed: "#cc409EFF"
                borderColor: "#D9D9D9"
                borderW: 0
                mRadius:3
                fontsize: 10

                text: qsTr("确定")
                onClicked: {
                    s_dayChange(Qt.formatDate(the_calendar.selectedDate,"yyyyMMdd000000"))
                    root.close()
                }

            }

        }
    }

    function getCurrentData(){
        return the_calendar.selectedDate
    }



}
