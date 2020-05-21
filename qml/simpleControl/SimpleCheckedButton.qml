import QtQuick 2.0
import QtQuick.Controls 1.4
Rectangle {

    id:checkButton
    property string imgSrc: ""
    property string imgCheckSrc: ""

    property alias imgW: checkImg.width
    property alias imgH: checkImg.height
    property alias txtFont: checktxt.font
    property alias txtColor: checktxt.color
    property alias text: checktxt.text

    property bool isOnlySelect: false
    property bool checked: false
    property ExclusiveGroup exclusiveGroup: null //对外开放一个ExclusiveGroup接口，用于绑定同个组


    onExclusiveGroupChanged: {
        if (exclusiveGroup) {
            exclusiveGroup.bindCheckable(checkButton)
        }
    }

    //如果少了这个信号处理，则无法出现互斥效果
//    onCheckedChanged: {
//        checked ? checkImg.source = imgCheckSrc : checkPic.source = imgSrc
//    }

    width: checkImg.width + 6 + checktxt.width
    height: (checkImg.height > checktxt.height?checkImg.height:checktxt.height)
    Image {
        id: checkImg
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        source: checked ?imgCheckSrc:imgSrc
    }

    Text {
        id: checktxt
        anchors.left: checkImg.right
        anchors.leftMargin: 6
        anchors.verticalCenter: parent.verticalCenter
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            if(isOnlySelect)
                checked = true
            else
                checked = !checked
        }
    }
}
