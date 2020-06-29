import QtQuick.Controls 2.5
import QtQuick 2.0
import QtGraphicalEffects 1.12
import "../playbackVideo"
import "../dialog"
import "../warnManager"
Rectangle {
    id: root
    property int checkedHeaderLeftMargin: 20
    property int avatarHeaderLeftMargin: 44
    property int numberingHeaderLeftMargin: 224//编号
    property int nameHeaderLeftMargin: 386//姓名
    property int timeHeaderLeftMargin: 548//time
    property int deleteHeaderLeftMargin: 806

    property int fontSize: lKhmer===curLanguage?28:14
    property int titlefontSize: lKhmer===curLanguage?36:18
    property int listviewClickIndex: -1
    property bool isAllSelect: false

    signal s_allselect(bool isSelect);

    onIsAllSelectChanged: {
        //warnmodel.set
    }
    property string curDateStr: ""



    //加个矩形是为了解决listview显示越界的问题
    Rectangle{
        id:rectTop
        width: parent.warnImgHeaderLeftMargin
        height: 20
        color: "#DFE1E6"
        anchors.top: parent.top
        z:2
    }
    Rectangle{
        id:rectWarnArea
        width: parent.width-40
        height: parent.height-40
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#F8FAFD"

        //加个矩形是为了解决listview显示越界的问题
        Rectangle{
            anchors.top: rectWarnArea.top
            width: parent.width
            height: 84
            color: "#F8FAFD"
            z:1
        }
        //第一排控件
        Text {
            id: textitle
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 60
            anchors.topMargin: 35
            font.pixelSize: lKhmer===curLanguage?36:18
            z:2
            text: qsTr("数据列表")
        }

        Rectangle{
            id:rectBatch
            width: txtBatchDelete.width + imgBatchDelete.width +27
            height: 36
            color: "#3B84F6"
            radius: 4
            anchors.left: textitle.right
            anchors.leftMargin: 39
            anchors.verticalCenter: textitle.verticalCenter
            z:2
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

                    batchImportState.visible = true
                    batchImportState.test()

                    return

                    if(curLanguage===lRussian)
                        askDialog.width = 500
                    else
                        askDialog.width = 427
                    askDialog.height = 176
                    askDialog.askStr = curLanguage=== lChinese?"确认要删除所选信息吗？":
                                                                curLanguage===lEnglish?"Confirm to delete?":
                                                                                        curLanguage===lKorean?"삭제하시겠습니까?":
                                                                                                               curLanguage===lItaly?"Cancella Tutta la Selezione?":
                                                                                                                                     curLanguage===lRussian?"Вы уверены, что хотите удалить информацию?":
                                                                                                                                                             curLanguage===lLithuanian?"Patvirtinti ištrynimą?":
                                                                                                                                                                                        curLanguage === ltuerqi?"Kapı Bilgileri?":
                                                                                                                                                                                                                 curLanguage === ltuerqi1?"Tüm Seçimleri Sil?":
                                                                                                                                                                                                                                           curLanguage === lputaoya?"Confirme a exclusão?":
                                                                                                                                                                                                                                                                     curLanguage === lxibanya?"Confirmar para eliminar?":
                                                                                                                                                                                                                                                                                               curLanguage === lfayu?"Confirmer la suppression?":
                                                                                                                                                                                                                                                                                                                      curLanguage === lniboer?"साँचै मेट्ने हो ?":
                                                                                                                                                                                                                                                                                                                                               curLanguage === lKhmer?"យល់ព្រមលុបចោល?":""

                    askDialog.imgSrc = "qrc:/images/ico_warn.png"
                    askDialog.curType = askDialog.warnInfoMutipleDelete
                    askDialog.open();
                }
                onReleased: rectBatch.color = "#3B84F6"
                onPressed: rectBatch.color = "#66B5FF"
            }
        }

        Rectangle{
            id:rectSingleImport
            width: imgSingleImport.width + txtSingleImport.width +27
            height: 36
            color: "#71C648"
            radius: 4
            anchors.left: rectBatch.right
            anchors.leftMargin: 20
            anchors.verticalCenter: rectBatch.verticalCenter
            z:2
            Image {
                id: imgSingleImport
                width: 14
                height: 15
                anchors.left: parent.left
                anchors.leftMargin: 9
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/images/dataAdd.png"

            }

            Text {
                id: txtSingleImport
                font.pixelSize: fontSize
                anchors.left: imgSingleImport.right
                anchors.leftMargin: 9
                anchors.verticalCenter: parent.verticalCenter
                color: "#ffffff"
                text: qsTr("单个导入")
            }
            MouseArea{
                anchors.fill: parent

                onClicked: {
                    popSingleImport.open();
                }
                onReleased: rectSingleImport.color = "#71C648"
                onPressed: rectSingleImport.color = "#99D67C"
            }
        }

        Rectangle{
            id:rectBatchImport
            width: imgBatchImport.width + txtBatchImport.width +27
            height: 36
            color: "#FB893F"
            radius: 4
            anchors.left: rectSingleImport.right
            anchors.leftMargin: 20
            anchors.verticalCenter: rectSingleImport.verticalCenter
            z:2
            Image {
                id: imgBatchImport
                width: 14
                height: 15
                anchors.left: parent.left
                anchors.leftMargin: 9
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/images/import.png"

            }
            Text {
                id: txtBatchImport
                font.pixelSize: fontSize
                anchors.left: imgBatchImport.right
                anchors.leftMargin: 9
                anchors.verticalCenter: parent.verticalCenter
                color: "#ffffff"
                text: qsTr("批量导入")
            }
            MouseArea{
                anchors.fill: parent

                onClicked: popBatchImport.open()
                onReleased: rectBatchImport.color = "#FB893F"
                onPressed: rectBatchImport.color = "#FCAA75"
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
            z:2
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
                font.pixelSize: 14
                color: "#909399"
                text:Qt.formatDate(calendar.getCurrentData(),"yyyy-MM-dd")
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
            z:2
            anchors.left: dateRect.right
            anchors.verticalCenter: rectBatch.verticalCenter
            anchors.leftMargin: 10

            Image {
                id: imgtime
                width: 14
                height: 14
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
                font.pixelSize: 14
                color: "#909399"
                text: qsTr("00:00:00")
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onPressed: {
                    selecttime.open()
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
            z:1
            color: "#EEF3FA"
            Image{
                id:imgSelect
                width: 14
                height: 14
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: checkedHeaderLeftMargin
                source: isAllSelect?"qrc:/images/btnSelect_s.png":"qrc:/images/btnSelect.png"

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        if(isAllSelect){
                            isAllSelect = false;
                        }else{
                            isAllSelect = true;
                        }
                        datamodel.funSetAllSelect(isAllSelect);
                    }
                }
            }
            Text {
                id: txtAvatar
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: avatarHeaderLeftMargin
                font.pixelSize: fontSize
                color: "#333333"
                font.bold: curLanguage===lKorean
                text: qsTr("头像")
            }
            Text {
                id: txtNumbering
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: numberingHeaderLeftMargin
                font.pixelSize: fontSize
                color: "#333333"
                font.bold: curLanguage===lKorean
                text: qsTr("编号")
            }
            Text {
                id: txtWarnName
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: nameHeaderLeftMargin
                font.pixelSize: fontSize
                color: "#333333"
                font.bold: curLanguage===lKorean
                text: qsTr("姓名")
            }
            Text {
                id: txtWarnTemp
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: timeHeaderLeftMargin
                font.pixelSize: fontSize
                color: "#333333"
                visible: false
                font.bold: curLanguage===lKorean
                text: qsTr("导入时间")
            }
            Text {
                id: txtDo
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: deleteHeaderLeftMargin
                font.pixelSize: fontSize
                font.bold: curLanguage===lKorean
                color: "#333333"
                text: qsTr("操作")
            }
        }
        ListView{
            id:warnList
            width: warnHeader.width
            height: parent.height - 210 -warnHeader.height
            anchors.top: warnHeader.bottom
            anchors.left: warnHeader.left
            model: dataModel
            z:0
            ScrollBar.vertical: ScrollBar {size:10}
            delegate: Rectangle{
                property bool enter: false
                width: parent.width
                height: 112
                // color:warnList.currentIndex === index?"#DFEAF8":(listviewClickIndex === index?"#DFEAF8":(enter?"#EEF3FA":"#F8FAFD"))
                color:(warnList.currentIndex === index?"#DFEAF8":(enter?"#EEF3FA":"#F8FAFD"))
                Image{
                    id:select
                    width: 14
                    height: 14
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: checkedHeaderLeftMargin
                    source:model.isSelect?"qrc:/images/btnSelect_s.png": "qrc:/images/btnSelect.png"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            model.isSelect = !model.isSelect;

                            if(!model.isSelect){
                                isAllSelect = false;
                                datamodel.funSetInitSelectFalse();
                            }
                        }
                    }
                }

                Image {
                    id: imgAvatar
                    width: 60
                    height: 72
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: avatarHeaderLeftMargin
                    source: model.avatarPath
                }

                Text {
                    id: numbering
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: numberingHeaderLeftMargin
                    font.pixelSize: 14
                    text: model.numbering;
                }

                Text {
                    id: name
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: nameHeaderLeftMargin
                    font.pixelSize: fontSize
                    text: model.name;
                }


                Text {
                    id: time
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: timeHeaderLeftMargin
                    font.pixelSize: fontSize
                    text: model.time;
                }

                Image {
                    id: imgDelete
                    anchors.left: parent.left
                    anchors.leftMargin: deleteHeaderLeftMargin
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/images/delete.png"
                }

                Rectangle{
                    id:doRect
                    width: 1
                    height: 12
                    anchors.left: imgDelete.right
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#0486FE"
                }

                Image {
                    id: imgRevise
                    anchors.left: doRect.right
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/images/revise.png"
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
            width:280
            height: 314
            dim:false
            x:dateRect.x
            y:67
            Component.onCompleted: {

                curDateStr = Qt.formatDate(calendar.getCurrentData(),"yyyyMMdd");
                //datamodel.funFlushWarnInfo(deviceconfig.getScrennShotPath(),curDateStr);
            }
            onS_dayChange:{
                curDateStr = value;
                datamodel.funFlushWarnInfo(deviceconfig.getScrennShotPath(),curDateStr);
            }

            onS_dayChange1: txtDate.text = value

            //onS_mouthChange:getRecordInfo(1,value)
            onS_yearChange: ;
            // Component.onCompleted:getRecordInfo(2, Qt.formatDate(calendar.getCurrentData(),"yyyyMMdd000000"))
        }


        SelectTime{
            id:selecttime
            x:timeRect.x
            y:timeRect.y + timeRect.height + 2
            z:1
            width: 120
            height: 245
            onS_ensure: {
                var timeStr = timeh+":"+timem+":"+times
                txttime.text = timeStr
                var curIndex = datamodel.funFindIndex(timeh,timem,times)
                console.debug("curIndex "+curIndex)

                warnList.positionViewAtIndex(curIndex,ListView.Beginning)
                warnList.currentIndex = curIndex;
                //warnList.currentIndex = curIndex
            }
        }
    }

    BatchImportState{
        id:batchImportState
        visible: false
        color: "#00000000"
        anchors.fill: parent
    }

    SingleImport{
        id:popSingleImport
        width: 465
        height:334
    }

    BatchImport{
        id:popBatchImport
        width: 465
        height:334

    }

    
    Popup{
        id:popimportimg
        width: 136
        height: 52
        x: parent.width/2 - popimportimg.width/2
        y: parent.height/2 - popimportimg.height/2
        property alias msgStr: txt.text
        property alias msgImg: img.source
        modal: true
        focus: true
        //设置窗口关闭方式为按“Esc”键关闭
        closePolicy: Popup.OnEscape
        //设置窗口的背景控件，不设置的话Popup的边框会显示出来
        background: rect
        dim:false

        onOpened: {
            poptimer.start();
        }
        onClosed: {
        }
        Rectangle {
            id: rect
            anchors.fill: parent
            color: "#ffffff"
            radius: 3

            //设置标题栏区域为拖拽区域
            Rectangle{

                width: parent.width
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    id: img
                    width: 20
                    height: 20
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    source: "qrc:/images/icon_msg.png"
                }

                Text {
                    id: txt
                    anchors.left: img.right
                    anchors.leftMargin: 12
                    anchors.verticalCenter: img.verticalCenter
                    font.pixelSize:16
                    text: qsTr("")
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

        Timer{
            id:poptimer
            interval: 600;
            repeat: false
            onTriggered: {
                poptimer.stop();
                popimportimg.close();
            }
        }
    }

    Connections{
        target: askDialog
        onS_CurTypeMsg:{
            if(askDialog.warnInfoMutipleDelete === type){
                datamodel.funDeleteSelect();
                isAllSelect = false;
            }else if(askDialog.warnInfoSingleDelete === type)
                datamodel.funDeleteIndex(warnList.currentIndex)
        }
    }

    Connections{
        target: main
        onS_setLanguage:setLanguage(typeL);
    }
    Connections{
        target: dataModel
        onSignal_singleAdd:{

            if(isSucc){
                popimportimg.msgImg = "qrc:/images/connect_succ.png"
                popimportimg.msgStr = "导入成功"
                popimportimg.open()
            }else{
                popimportimg.msgImg = "qrc:/images/connect_fail.png"
                popimportimg.msgStr = "导入失败"
                popimportimg.open()
            }
        }

        onSignal_batchAmount:{

            batchImportState.importAmount = amount
            batchImportState.importSuccCount = 0
            batchImportState.importFailCount = 0

        }

        onSignal_batchAdd:{
            if(isSucc)
                batchImportState.importSuccCount ++;
            else
                batchImportState.importFailCount ++;
        }

    }


    function setLanguage(type){

        switch(type){
        case lEnglish:
            textitle.text = "Log list";
            txtBatchDelete.text = "Batch Remove"
            txtDo.text = "Operation"

            break;
        case lKorean:
            textitle.text = "로그정보";
            txtBatchDelete.text = "전체삭제"
            txtDo.text = "설정"

            //deletetxt.text = "삭제"
            break;
        case lItaly:
            textitle.text = "Elenco eventi";
            txtBatchDelete.text = "Cancella tutti eventi"
            txtDo.text = "Operazione"


            break;
        case lChinese:
            textitle.text = "数据列表";
            txtBatchDelete.text = "批量删除"
            txtDo.text = "操作"

            break;
        case lRussian:
            textitle.text = "Список журналов";
            txtBatchDelete.text = "Очистить все события"
            txtDo.text = "Операция"

            break;
        case lLithuanian:
            textitle.text = "Įvykių žurnalas";
            txtBatchDelete.text = "Išvalyti visus pranešimas"
            txtDo.text = "Valdymas"

            break;
        case ltuerqi:
            textitle.text = "Etkinlik listesi";
            txtBatchDelete.text = "Toplu olarak sil"
            txtDo.text = "İşlem"

            break;
        case ltuerqi1:
            textitle.text = "Etkinlik Listesi";
            txtBatchDelete.text = "Toplu olarak Sil"
            txtDo.text = "İşlem"

            break;
        case lputaoya:
            textitle.text = "lista de registro";
            txtBatchDelete.text = "Excluir em lote"
            txtDo.text = "Operar"

            break;
        case lxibanya:
            textitle.text = "lista de registro";
            txtBatchDelete.text = "Eliminar por lotes"
            txtDo.text = "Funcionar"

            break;
        case lfayu:
            textitle.text = "Liste des journaux";
            txtBatchDelete.text = "Supprimer en lot"
            txtDo.text = "Fonctionnement"

            break;
        case lniboer:
            textitle.text = "लग विवरण ";
            txtBatchDelete.text = "एकैचोटी मेट्नुहोस् "
            txtDo.text = "चलाउनुहोस"

            break;
        case lKhmer:
            textitle.text = "កំណត់ហេតុការចូលប្រើប្រាស់ឧបករណ៍";
            txtBatchDelete.text = "លុបចោលទាំងអស់"
            txtDo.text = "ប្រតិបត្តិការ"

            break;
        }
    }
}
