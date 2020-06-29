import QtQuick 2.0

Rectangle{
    id:rectfullScreen


    property string importSuccStr: ""
    property string importFailStr: ""




    property int importAmount: 100
    property int importSuccCount:50
    property int importFailCount: 0
    signal s_finished();
    Rectangle{
        id:rectBatchImportState
        width: 465
        height: 334
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: "#ffffff"
        radius: 10
        Text {
            id: txt
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 20
            font.pixelSize:18
            color: "#333333"
            text: qsTr("批量导入")
        }

        Image {
            id: img11
            width: 18
            height: 18
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.verticalCenter: txt.verticalCenter
            source: "qrc:/images/dataclos.png"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    rectfullScreen.visible =false

                }
            }
        }




        Rectangle{

            anchors.horizontalCenter: rectBatchImportState.horizontalCenter
            width: rectBatchImportState.width
            height: rectBatchImportState.height -73
            anchors.top: img11.bottom
            Loader{
                id:loaderToast
                sourceComponent: null
            }
        }


        Component{
            id: cmpSucc

            Rectangle{
                width: rectBatchImportState.width
                height: rectBatchImportState.height -73
                Image {
                    id: img
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 30
                    width: 40
                    height: 40
                    source: "qrc:/images/connect_succ.png"
                }
                Text {
                    id: txt1
                    color: "#333333"
                    font.pixelSize: 18
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: img.bottom
                    anchors.topMargin: 10
                    text: qsTr("导入完成 ")
                }
                Text {
                    id: txt2
                    color: "#929292"
                    font.pixelSize: 14
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: txt1.bottom
                    anchors.topMargin: 10
                    text: importSuccStr
                }
                Rectangle{
                    id:rectbtn
                    width: 180
                    height: 34
                    color: "#ffffff"
                    border.width: 1
                    border.color: "#0486FE"
                    anchors.top: txt2.bottom
                    anchors.topMargin: 40
                    anchors.horizontalCenter: parent.horizontalCenter
                    Text {
                        id: txtbtn
                        color: "#3B84F6"
                        font.pixelSize: 14
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("完成")
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked:s_finished();
                    }
                }

            }
        }

        Component {
            id: cmpFail
            Rectangle{
                width: rectBatchImportState.width
                height: rectBatchImportState.height - 73
                Image {
                    id: img
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 30
                    width: 40
                    height: 40
                    source: "qrc:/images/connect_succ.png"
                }
                Text {
                    id: txt1
                    color: "#333333"
                    font.pixelSize: 18
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: img.bottom
                    anchors.topMargin: 10
                    text: qsTr("导入完成")
                }
                Text {
                    id: txt2
                    color: "#929292"
                    font.pixelSize: 14
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: txt1.bottom
                    anchors.topMargin: 10
                    text: importFailStr
                }

                Rectangle{
                    id:rectbtncheck
                    width: 180
                    height: 34
                    color: "#ffffff"
                    border.width: 1
                    border.color: "#0486FE"
                    anchors.top: txt2.bottom
                    anchors.topMargin: 40
                    anchors.horizontalCenter: parent.horizontalCenter
                    Text {
                        id: txtbtncheck
                        color: "#3B84F6"
                        font.pixelSize: 14
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("查看详情")
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked:s_finished();
                    }
                }

                Rectangle{
                    id:rectbtn
                    width: 180
                    height: 34
                    color: "#ffffff"
                    border.width: 1
                    border.color: "#0486FE"
                    anchors.top: rectbtncheck.bottom
                    anchors.topMargin: 10
                    anchors.horizontalCenter: parent.horizontalCenter
                    Text {
                        id: txtbtn
                        color: "#3B84F6"
                        font.pixelSize: 14
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("完成")
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked:s_finished();
                    }
                }

            }
        }

        Component {
            id: cmpImporting
            Rectangle{
                width: rectBatchImportState.width
                height: rectBatchImportState.height - 73
                CircularProgress{
                    id:circularprogress
                    width: 80
                    height: 80
                    anchors.top: parent.top
                    anchors.topMargin: 30
                    progress: importSuccCount/importAmount
                    arcBackgroundColor:"#DBE6F8"
                    arcColor: "#0486FE"
                    arcWidth:4
                    anchors.horizontalCenter: parent.horizontalCenter
                }


                Text {
                    id: txt1
                    color: "#0486FE"
                    font.pixelSize: 14
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: circularprogress.bottom
                    anchors.topMargin: 13
                    text: importSuccCount + "/"+importAmount
                }
                Text {
                    id: txt2
                    color: "#929292"
                    font.pixelSize: 14
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: txt1.bottom
                    anchors.topMargin: 16
                    text: "正在导入中…"
                }

            }
        }

    }


    function test(){
        console.debug("***test***")
        loaderToast.sourceComponent = cmpImporting
    }

}
