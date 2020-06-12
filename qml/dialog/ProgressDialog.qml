import QtQuick 2.0
import QtGraphicalEffects 1.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
Popup {
    id: root
    x: parent.width/2 - root.width/2
    y: parent.height/2 - root.height/2

    modal: true
    focus: true
    //设置窗口关闭方式为按“Esc”键关闭
    closePolicy: Popup.NoAutoClose//Popup.OnEscape
    //设置窗口的背景控件，不设置的话Popup的边框会显示出来
    background: rect

    Rectangle{
        id:rect
        anchors.fill: parent
        radius: 10

        ProgressBar {
            id:progressbar
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 80
            height: 14
            minimumValue: 0
            maximumValue: 100
            value: 0
            style: ProgressBarStyle{
                background: Rectangle{

                    //border.color: control.hovered?"green":"#25b1e8";
                    radius: 7
                    color:"#EFEFF0";
                }
                progress: Rectangle{
                    radius: 7
                    color: "#0486FE"
                }

            }
        }

        Text {
            id: txttitle
            anchors.bottom: progressbar.top
            anchors.bottomMargin: 8
            anchors.left: progressbar.left
            color: "#6B6D70"
            font.pixelSize: 14
            text: qsTr("正在升级,请等待…")
        }

        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            horizontalOffset: 4
            verticalOffset: 4
            color:"#19000000";
        }
    }
    function setProgressValue(value)
    {
        progressbar.setValue(value)
    }



    Connections{
        target: main
        onS_setLanguage:setLanguage(typeL);
    }

    function setLanguage(type){
        switch(type){
        case lEnglish:
            txttitle.text = "Pls wait for device upgrading "
            break;
        case lChinese:
            txttitle.text = "正在升级,请等待…"
            break;
        case lRussian:
            txtCancel.text = "Пожалуйста, дождитесь обновления устройства"
            break;
        case ltuerqi:
            txtCancel.text = "Pls cihaz yükseltmesini bekleyin"
            break;
        case lxibanya:
            txtCancel.text = "Por favor espere la actualización del dispositivo"
            break;
        case lfayu:
            txtCancel.text = "Veuillez attendre la mise à niveau de l'appareil"
            break;
        }
    }
}
