import QtQuick 2.0

import QtQuick 2.0
Row {


    property alias barModel: mymodel

    property int curIndex: 0

    property string btnBgColor: ""
    property string btnBgSelectColor: ""
    property string mflagColor: ""
    property string textColor: ""
    property string textSelectColor: ""
    property int txtLeftMargin: 0
    property int imageLeftMargin: 0
    property string imageSrcEnter: ""
    property int textSize: 5
    ListModel{
        id:mymodel

    }
    spacing: 30
    Repeater{
        width: parent.width
        height: parent.height
        model: mymodel
        QmlTabButton{
            height: parent.height
            txtColor:curIndex == index?textSelectColor:textColor
            flagColor:curIndex == index?mflagColor:"#00000000"
            bgColor:curIndex == index?btnBgSelectColor:btnBgColor
            txtStr: model.txtStr
            textLeftMargin:txtLeftMargin
            imgLeftMargin: imageLeftMargin

            txtSize:textSize
            onClick:{
                curIndex = index

            }
        }
    }
}
