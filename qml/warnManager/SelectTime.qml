import QtQuick 2.12
import QtQuick.Controls 2.5

Rectangle {
    id: root
    visible: false;
    signal s_ensure(var timeh,var timem,var times);
    ListModel{
        id:hourModel
        Component.onCompleted: {
            for(var i=0;i<24;i++){
                if(i<10)
                    hourModel.append({time:"0"+i})
                else
                    hourModel.append({time:""+i})
            }
        }
    }
    ListModel{
        id:minModel
        Component.onCompleted: {
            for(var i=0;i<60;i++)
                if(i<10)
                    minModel.append({time:"0"+i})
                else
                    minModel.append({time:""+i})
        }
    }
    ListModel{
        id:secModel
        Component.onCompleted: {
            for(var i=0;i<60;i++)
                if(i<10)
                    secModel.append({time:"0"+i})
                else
                    secModel.append({time:""+i})
        }
    }
    Rectangle{
        id:rect
        anchors.fill: parent
        color: "#DFE1E6"
        radius: 4

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
        }
        ListView{
            id:listhour
            anchors.left: parent.left
            anchors.top: parent.top
            cacheBuffer:0
            width: 40
            height: parent.height - 34
            model: hourModel
            ScrollBar.vertical: ScrollBar {}
            delegate: Rectangle{
                width: parent.width
                height: 34
                border.width: 0
                color: index === listhour.currentIndex?"#F7F7F7":"#ffffff"
                Text {
                    id: txtHour
                    anchors.centerIn: parent
                    font.pixelSize: 12
                    text: model.time
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: listhour.currentIndex = index
                }
            }
        }
        ListView{
            id:listmin
            x:41
            y:0
            width: 40
            height: parent.height - 34
            model: minModel
            ScrollBar.vertical: ScrollBar {
                id: scrollBar
                onActiveChanged: active = true;
                //                Component.onCompleted: {
                //                    scrollBar.handle.color = Qt.rgba(0,0,0,0.15);
                //                    scrollBar.active = true;
                //                    scrollBar.handle.width = 1;
                //                    scrollBar.handle.height = 1;
                //                }
            }

            delegate: Rectangle{
                width: parent.width
                height: 34
                border.width: 0
                color: index === listmin.currentIndex?"#F7F7F7":"#ffffff"
                Text {
                    id: txtmin
                    anchors.centerIn: parent
                    font.pixelSize: 12
                    text: model.time
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: listmin.currentIndex = index
                }
            }
        }
        ListView{
            id:listsec
            x:82
            y:0
            width: 40
            height: parent.height - 34
            model: secModel
            ScrollBar.vertical: ScrollBar {}
            delegate: Rectangle{
                width: parent.width
                height: 34
                border.width: 0
                color: index === listsec.currentIndex?"#F7F7F7":"#ffffff"
                Text {
                    id: txtSec
                    anchors.centerIn: parent
                    font.pixelSize: 12
                    text: model.time
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: listsec.currentIndex = index
                }
            }
        }

        Rectangle{
            id:bottomRect
            width: parent.width
            height: 33
            anchors.bottom: parent.bottom
            Text {
                id: txtCancel
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.bottomMargin: 6
                anchors.leftMargin: 20
                font.pixelSize: 12
                text: qsTr("取消")
                MouseArea{
                    anchors.fill: parent
                    onClicked: root.visible =false
                }
            }


            Text {
                id: txtEnsure
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.bottomMargin: 6
                anchors.rightMargin: 20
                text: qsTr("确定")
                font.pixelSize: 12
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        var hourstr = ""+hourModel.get(listhour.currentIndex).time;
                        var minstr = ""+minModel.get(listmin.currentIndex).time
                        var secstr = ""+secModel.get(listsec.currentIndex).time
                        s_ensure(hourstr,minstr,secstr);
                        root.visible = false;
                    }
                }
            }
        }

    }


    Connections{
        target: main
        onS_setLanguage:setLanguage(typeL);
    }

    function setLanguage(type){
        switch(type){
        case lEnglish:
            txtCancel.text = "no"
            txtEnsure.text = "yes "
            break;
        case lKorean:
            txtCancel.text = "취소"
            txtEnsure.text = "확인"
            break;
        case lItaly:
            txtCancel.text = "no"
            txtEnsure.text = "sì"
            break;
        case lChinese:
            txtCancel.text = "取消"
            txtEnsure.text = "确定"
            break;
        }
    }
}
