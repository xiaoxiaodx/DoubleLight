import QtQuick 2.0

Image {

    id: mimg


    //属于图片的属性
    property string imgSourseNormal: ""
    property string imgSourseHover: ""
    property string imgSoursePress: ""

    width: 36
    height: 36
    source: imgSourseNormal
    signal click();
    MouseArea{

        anchors.fill:parent
        propagateComposedEvents: true;
        hoverEnabled: true

        onEntered:{
            mouse.enabled = true
            mimg.source = imgSourseHover
        }

        onExited: {
            mouse.enabled = true
            mimg.source = imgSourseNormal
        }

        onPressed: mimg.source = imgSoursePress


        onReleased: mimg.source = imgSourseNormal

        onClicked: click()

    }
}








