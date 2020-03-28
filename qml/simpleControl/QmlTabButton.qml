import QtQuick 2.0



Rectangle{
    id:root
    property alias txtColor: txt.color
    property alias txtSize: txt.font.pixelSize
    property alias flagColor: flag.color
    property alias imgVisble: img.visible
    property string imgSrc:""
    property string imgSrcEnter: ""
    property alias txtStr: txt.text
    property color bgColor: "#000000"
    property color bgColorEnter: "#000000"
    property int textLeftMargin: 0
    property int imgLeftMargin: 0

    property string preBgColor: ""
    signal click()
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#5D9CFF"}
        GradientStop { position: 1.0; color: "#2D76E7"}
    }


    width: txt.width+ textLeftMargin *2

    Rectangle{
        id:flag
        width: parent.width
        height: 2
        anchors.bottom: parent.bottom
        //anchors.bottomMargin: 2
    }

    Text {
        id: txt
        anchors.left: parent.left
        anchors.leftMargin: textLeftMargin
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 18
    }

    Image {
        id: img
        anchors.left: txt.right
        anchors.leftMargin: imgLeftMargin
        anchors.verticalCenter: parent.verticalCenter
        source: imgSrc
    }

    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        onPressed: {


        }
        onReleased: {

        }

        onClicked: {
            //preBgColor = color
            click()
        }
//        onExited: {
//            color = preBgColor
//        }
//        onEntered: {
//            preBgColor = color
//            color = imgSrcEnter
//        }
    }

}
