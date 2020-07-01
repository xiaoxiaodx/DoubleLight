import QtQuick 2.0
import QtQuick.Controls 2.5
import "../simpleControl"
import QtGraphicalEffects 1.12
import QtQuick.Dialogs 1.3
Popup {
    id: root
    x: parent.width/2 - root.width/2
    y: parent.height/2 - root.height/2

    //    width: 380
    //    height: 253
    modal: true
    focus: true
    //设置窗口关闭方式为按“Esc”键关闭
    closePolicy: Popup.OnEscape
    //设置窗口的背景控件，不设置的话Popup的边框会显示出来
    background: rect
    dim:false

    property alias imgSrc: imgAdd.source


    signal s_reviseFaceinfo(var name,var number);
    Rectangle {
        id: rect
        anchors.fill: parent
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
            text: qsTr("修改")
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
                onClicked: root.close();
            }
        }



        Text {
            id: txtnumber
            font.pixelSize: 14
            color: "#333333"
            anchors.right: inputnumber.left
            anchors.rightMargin: 8
            anchors.verticalCenter: inputnumber.verticalCenter
            text: qsTr("编号")
        }

        LineEdit {
            id: inputnumber
            width: 158
            height: 28
            anchors.left: parent.left
            anchors.leftMargin: 98
            anchors.top: parent.top
            anchors.topMargin: 64
            border.width: 1
            border.color: "#DCDFE6"
            radius: 1
            font.pixelSize: 14
            placeholderText: "请输入编号"
            placeholderTxtColor: "#DCDFE6"
            isNeedDoubleClickEdit: false
            textLeftPadding:0
            txtColor: Qt.rgba(0,0,0,0.65)
            color: "#ffffff"
        }

        Text {
            id: txtname
            font.pixelSize: 14
            color: "#333333"
            anchors.right: inputname.left
            anchors.rightMargin: 8
            anchors.verticalCenter: inputname.verticalCenter
            text: qsTr("姓名")
        }

        LineEdit {
            id: inputname
            width: 158
            height: 28
            anchors.left: inputnumber.left
            anchors.top: inputnumber.bottom
            anchors.topMargin: 10
            border.width: 1
            border.color: "#DCDFE6"
            radius: 1
            font.pixelSize: 14
            placeholderText: "请输入姓名"
            placeholderTxtColor: "#DCDFE6"
            isNeedDoubleClickEdit: false
            textLeftPadding:0
            txtColor: Qt.rgba(0,0,0,0.65)
            color: "#ffffff"
        }

        Text {
            id: txtAvatar
            font.pixelSize: 14
            color: "#333333"
            anchors.right: imgAdd.left
            anchors.rightMargin: 8
            anchors.top: imgAdd.top
            text: qsTr("照片")
        }

        Image {
            id: imgAdd
            width: 100
            height: 120
            anchors.left: inputname.left
            anchors.top: inputname.bottom
            anchors.topMargin: 10
            source: "qrc:/images/pictrueAdd.png"

//            MouseArea{
//                anchors.fill: parent
//                //onPressed: imgAdd.source = "qrc:/images/pictrueAdd_P.png"
//                //onReleased: imgAdd.source = "qrc:/images/pictrueAdd.png"
//                onClicked: {

//                    fileDialog.open();
//                }
//            }

        }

        Text {
            id: texttip
            font.pixelSize: 12
            color: "#999999"
            anchors.left: imgAdd.left
            anchors.top: imgAdd.bottom
            anchors.topMargin: 4
            text: qsTr("仅支持jpeg格式")
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
                font.pixelSize: lKhmer===curLanguage?28:14
                color: "#3B84F6"
                text: qsTr("确定")
            }
            MouseArea{
                anchors.fill: parent
                onClicked:{


                    s_reviseFaceinfo(inputname.text,inputnumber.text)

                    root.close()
                }
            }
        }

        Rectangle{
            id:btnCancel
            width: txtCancel.width + 24
            height: 34
            anchors.verticalCenter: btnEnsure.verticalCenter
            anchors.right: btnEnsure.left
            anchors.rightMargin: 14
            border.width: 1
            border.color: "#909399"
            Text {
                id: txtCancel
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideMiddle
                font.pixelSize: lKhmer===curLanguage?28:14
                color: "#909399"
                text: qsTr("取消")
            }
            MouseArea{
                anchors.fill: parent
                onClicked:root.close()
            }
        }

        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            horizontalOffset: 4
            verticalOffset: 4
            color:"#10000000"
        }
    }



//    FileDialog {
//        id: fileDialog
//        property string pathname:""
//        title: "Please choose a file path"
//        selectFolder:false
//        selectMultiple: false
//        //folder: shortcuts.home
//        onAccepted: {
//            var str = fileDialog.fileUrl.toString();
//            imgAdd.source = str

//        }
//        onRejected: {
//        }
//    }

    Connections{
        target: main
        onS_setLanguage:setLanguage(typeL);
    }

    function setLanguage(type){
        //        switch(type){
        //        case lEnglish:
        //            //txtCancel.text = "Cancel"
        //            txtEnsure.text = "Confirm "
        //            break;
        //        case lKorean:
        //            //txtCancel.text = "취소"
        //            txtEnsure.text = "확인"
        //            break;
        //        case lItaly:
        //            //txtCancel.text = "Annullato"
        //            txtEnsure.text = "Confermare"
        //            break;
        //        case lChinese:
        //           // txtCancel.text = "取消"
        //            txtEnsure.text = "确定"
        //            break;
        //        case lRussian:
        //           // txtCancel.text = "Отмена"
        //            txtEnsure.text = "Подтвердить"
        //            break;
        //        case lLithuanian:
        //           // txtCancel.text = "Taip"
        //            txtEnsure.text = "Ne"
        //            break;
        //        case ltuerqi:
        //           // txtCancel.text = "İptal"
        //            txtEnsure.text = "Onayla"
        //            break;
        //        case ltuerqi1:
        //           // txtCancel.text = "İptal"
        //            txtEnsure.text = "Onayla"
        //            break;
        //        case lputaoya:
        //           // txtCancel.text = "Cancelar"
        //            txtEnsure.text = "Confirme"
        //            break;
        //        case lxibanya:
        //           // txtCancel.text = "Cancelar"
        //            txtEnsure.text = "Confirmar"
        //            break;
        //        case lfayu:
        //            //txtCancel.text = "Annuler"
        //            txtEnsure.text = "Confirmer"
        //            break;
        //        case lniboer:
        //           // txtCancel.text = "रद्ध गर्नुहोस"
        //            txtEnsure.text = "निश्चित गर्नुहोस"
        //            break;
        //        case lKhmer:
        //          //  txtCancel.text = "បោះបង់"
        //            txtEnsure.text = "យល់ព្រម"
        //            break;
        //        }
    }
}

