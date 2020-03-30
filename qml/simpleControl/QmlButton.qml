import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
Button{

    property color colorNor: "#476BFD"
    property color colorPressed:"#aa476BFD"
    property color fontColor: "#ffffff"
    property int borderW: 0
    property string borderColor: ""
    property int mRadius: 10
    property int fontsize: 10

    style:ButtonStyle{

        background: Rectangle {
            width: 360
            height: 50

            radius: mRadius
            border.width: borderW
            border.color: borderColor
            color: control.pressed?colorPressed:colorNor
        }
        label: Text {
            id: txt
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text:control.text
            font.pixelSize: fontsize
            font.family: "Microsoft Yahei"
            color: fontColor
        }

    }

}
