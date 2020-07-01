import QtQuick 2.0
import QtQuick.Controls 2.5
import "../simpleControl"
import QtGraphicalEffects 1.12
import QtQuick.Dialogs 1.3
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

    signal s_startImport()





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
                onClicked: root.close();
            }
        }

        
        
        Image{
            id:imgimport
            width: 16
            height: 16
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.top: parent.top
            anchors.topMargin: 120
            source: "qrc:/images/importfloder.png"
        }
        
        Text {
            id: imgiport
            color: "#333333"
            font.pixelSize: 14
            anchors.verticalCenter: imgimport.verticalCenter
            anchors.left: imgimport.right
            anchors.leftMargin: 6
            text: qsTr("导入文件")
        }
        
        Text {
            id: txttip
            color: "#999999"
            anchors.left: imgimport.left
            anchors.top: imgimport.bottom
            anchors.topMargin: 7
            font.pixelSize: 12
            text: qsTr("仅支持jpeg格式")
        }
        
        
        Rectangle{
            id:rectUpdatePath
            color: "#F8FAFD"
            border.color: "#DEDFE3"
            border.width: 1
            width: 280
            height: 32
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.top: txttip.bottom
            anchors.topMargin: 20
            LineEdit {
                id: inputUpdatePath
                width: rectUpdatePath.width -  22
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
        }
        
        Rectangle{
            id:selectFileRect
            width: txtUpdate.width + 24
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
                text: qsTr("选择文件")
            }
            MouseArea{
                anchors.fill: parent
                onClicked: fileDialog.open();
                onPressed: ;
                onReleased:;
            }
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
                text: qsTr("开始导入")
            }
            MouseArea{
                anchors.fill: parent
                onClicked:{
                    

                    dataModel.funImportBatch(inputUpdatePath.text);

                        s_startImport();
                    root.close()
                    
                }
            }
        }
        
        

        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            horizontalOffset: 4
            verticalOffset: 4
            color:"#20000000"
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
            var str = fileDialog.fileUrl.toString();
            inputUpdatePath.text = str.replace('file:///','');
        }
        onRejected: {
        }
    }

    Connections{
        target: main
        onS_setLanguage:setLanguage(typeL);
    }

    function setLanguage(type){
        switch(type){
        case lEnglish:

            break;
        case lKorean:

            break;
        case lItaly:

            break;
        case lChinese:

            break;
        case lRussian:

            break;
        case lLithuanian:


            break;
        case ltuerqi:

            break;
        case ltuerqi1:

            break;
        case lputaoya:

            break;
        case lxibanya:

            break;
        case lfayu:


            break;
        case lniboer:

            break;
        case lKhmer:

            break;
        }
    }
}

