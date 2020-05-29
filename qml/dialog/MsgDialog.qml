import QtQuick 2.0
import QtQuick.Controls 2.5
import "../simpleControl"
import QtGraphicalEffects 1.12

Popup {
    id: root
    x: parent.width/2 - root.width/2
    y: parent.height/2 - root.height/2
    property alias msgStr: txt.text
//    width: 380
//    height: 253
    modal: true
    focus: true
    //设置窗口关闭方式为按“Esc”键关闭
    closePolicy: Popup.OnEscape
    //设置窗口的背景控件，不设置的话Popup的边框会显示出来
    background: rect
    dim:false
    property int curType: -1;

    //signal s_deviceIDstr(var name,var strID,var strAccoount,var strPassword)

    signal s_showToast(var str1)
    signal s_CurTypeMsg(var type);

    property int exeClose: 0
    property int warnInfoSingleDelete: 1
    property int warnInfoMutipleDelete: 2
    property int deviceInfoSingleDelete: 3
    property int deviceInfoMutipleDelete: 4

    Rectangle {
        id: rect
        anchors.fill: parent
        color: "#ffffff"
        radius: 3

        //设置标题栏区域为拖拽区域
        Image {
            id: img
            width: 22
            height: 22
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 20
            source: "qrc:/images/icon_msg.png"
        }

        Text {
            id: txt
            anchors.left: img.right
            anchors.leftMargin: 16
            anchors.verticalCenter: img.verticalCenter
            font.pixelSize:14
            text: qsTr("text")
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
                    close();
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


    Connections{
        target: main
        onS_setLanguage:setLanguage(typeL);
    }

    function setLanguage(type){
        switch(type){
        case lEnglish:
            //txtCancel.text = "Cancel"
            txtEnsure.text = "Confirm "
            break;
        case lKorean:
            //txtCancel.text = "취소"
            txtEnsure.text = "확인"
            break;
        case lItaly:
            //txtCancel.text = "Annullato"
            txtEnsure.text = "Confermare"
            break;
        case lChinese:
           // txtCancel.text = "取消"
            txtEnsure.text = "确定"
            break;
        case lRussian:
           // txtCancel.text = "Отмена"
            txtEnsure.text = "Подтвердить"
            break;
        case lLithuanian:
           // txtCancel.text = "Taip"
            txtEnsure.text = "Ne"

            break;
        case ltuerqi:
           // txtCancel.text = "İptal"
            txtEnsure.text = "Onayla"
            break;
        case ltuerqi1:
           // txtCancel.text = "İptal"
            txtEnsure.text = "Onayla"
            break;
        case lputaoya:
           // txtCancel.text = "Cancelar"
            txtEnsure.text = "Confirme"
            break;
        case lxibanya:
           // txtCancel.text = "Cancelar"
            txtEnsure.text = "Confirmar"
            break;
        case lfayu:
            //txtCancel.text = "Annuler"
            txtEnsure.text = "Confirmer"
            break;
        case lniboer:
           // txtCancel.text = "रद्ध गर्नुहोस"
            txtEnsure.text = "निश्चित गर्नुहोस"
            break;
        case lKhmer:
          //  txtCancel.text = "បោះបង់"
            txtEnsure.text = "យល់ព្រម"
            break;
        }
    }
}

