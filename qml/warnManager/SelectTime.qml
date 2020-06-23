import QtQuick 2.12
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.12
Popup {
    id: root
    x: parent.width/2 - root.width/2
    y: parent.height/2 - root.height/2
    //    width: 380
    //    height: 253
    modal: true
    focus: true
    dim:false
    //设置窗口关闭方式为按“Esc”键关闭
    closePolicy: Popup.OnEscape
    //设置窗口的背景控件，不设置的话Popup的边框会显示出来
    background: rect


    property color fontColor: Qt.rgba(0,0,0,0.65);

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
        color: "#ffffff"
        radius: 4

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
        }
        ListView{
            id:listhour
            anchors.left: parent.left
            anchors.leftMargin: 4
            anchors.top: parent.top
            anchors.topMargin: 4
            cacheBuffer:0
            width: (parent.width - 10)/3
            height: parent.height - bottomRect.height - 9
            model: hourModel
            ScrollBar.vertical: ScrollBar {}
            delegate: Rectangle{
                width: parent.width
                height: 34
                border.width: 0
                color: index === listhour.currentIndex?"#EEF3FA":"#ffffff"
                Text {
                    id: txtHour
                    anchors.centerIn: parent
                    font.pixelSize: 12
                    color: fontColor
                    text: model.time
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: listhour.currentIndex = index
                }
            }
        }
        Rectangle{

            id:line
            width: 1
            height: listhour.height
            anchors.left: listhour.right
            anchors.top: listhour.top
        }
        ListView{
            id:listmin
            width: (parent.width - 10)/3
            height: parent.height - bottomRect.height - 9
            anchors.left: listhour.right
            anchors.leftMargin: 1
            anchors.top: listhour.top
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
                color: index === listmin.currentIndex?"#EEF3FA":"#ffffff"
                Text {
                    id: txtmin
                    anchors.centerIn: parent
                    font.pixelSize: 12
                    color: fontColor
                    text: model.time
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: listmin.currentIndex = index
                }
            }
        }
        Rectangle{

            id:line1
            width: 1
            height: listmin.height
            anchors.left: listmin.right
            anchors.top: listmin.top
        }
        ListView{
            id:listsec
            width: (parent.width - 10)/3
            height: parent.height - bottomRect.height - 9
            anchors.left: listmin.right
            anchors.leftMargin: 1
            anchors.top: listmin.top
            model: secModel
            ScrollBar.vertical: ScrollBar {}
            delegate: Rectangle{
                width: parent.width
                height: 34
                border.width: 0
                color: index === listsec.currentIndex?"#EEF3FA":"#ffffff"
                Text {
                    id: txtSec
                    anchors.centerIn: parent
                    font.pixelSize: 12
                    color: fontColor
                    text: model.time
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: listsec.currentIndex = index
                }
            }
        }


        Rectangle{
            id:line3
            anchors.top: listsec.bottom
            anchors.left: parent.left
            width: parent.width
            height: 1
            color: Qt.rgba(0,0,0,0.09)
        }

        Rectangle{
            id:bottomRect
            width: parent.width - 8
            height: 33
            z:1
            anchors.top: line3.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                id: txtCancel
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.bottomMargin: 6
                anchors.leftMargin: 20
                font.pixelSize: 12
                color: fontColor
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
                color: "#3B84F6"
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

        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            horizontalOffset: 0
            verticalOffset: 0
            color:"#30000000"
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
            txtEnsure.text = "yes"
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
            break
        case lRussian:
            txtCancel.text = "нет"
            txtEnsure.text = "да"
            break;
        case lLithuanian:
            txtCancel.text = "Ne"
            txtEnsure.text = "Taip"
            break;
        case ltuerqi:
            txtCancel.text = "İptal"
            txtEnsure.text = "Onayla"
            break;
        case ltuerqi1:
            txtCancel.text = "İptal"
            txtEnsure.text = "Onayla"
            break;
        case lputaoya:
            txtCancel.text = "no"
            txtEnsure.text = "yes"
            break;
        case lxibanya:
            txtCancel.text = "no"
            txtEnsure.text = "yes"
            break;
        case lfayu:
            txtCancel.text = "no"
            txtEnsure.text = "yes"
            break;
        case lniboer:
            txtCancel.text = "रद्द"
            txtEnsure.text = "निश्चित"
            break;
        case lKhmer:
            txtCancel.text = "បាទ/ចាស"
            txtEnsure.text = "ទេ"
            break;
        }
    }
}
