import QtQuick 2.12
import QtQuick.Controls 2.12


/*
    数据模型为多数据时会显示不正常
    model = [{tes:"ads"},{{tes:"ads"}}]//2个包含个数据的模型可以显示正常
    model = [{tes:"ads",tes1:"ads"},{{tes:"ads",tes1:"ads"}}]//2个包含多个数据的模型可以显示不正常
*/
ComboBox {
    id: control
    //model: [{showStr:"TCP",hello:0},{ showStr:"P2P",hello:2}]

    property string colorNor: "#999999"
    property string colorPressed: "#409EFF"
    property string fontColor: "#ffffff"
    property string bordColor: "transparent"

    property int itemHeight: 26
    property string itembordColor: bordColor //子项的边界宽不能设置
    property string itemColorNor: "#3A3D41"
    property string itemColorHoverd: "#272727"
    property int mRadius: 2
    property int indicatorW: 6
    property int indicatorH: 4


    delegate: ItemDelegate {
        width: control.width
        height:itemHeight
        contentItem: Text {
            text: showStr
            color: fontColor
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }
        highlighted: control.highlightedIndex === index
        background: Rectangle{
            width: parent.width
            height: parent.height
           // color: "#3A3D41"
            border.color: "#3A3D41"
            border.width: 0
            color: control.highlightedIndex === index?itemColorHoverd:itemColorNor
        }
    }

    indicator: Canvas {
        id: canvas
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: indicatorW
        height: indicatorH
        contextType: "2d"

        Connections {
            target: control
            onPressedChanged: canvas.requestPaint()
        }

        onPaint: {
            var context = canvas.getContext("2d");
            context.reset();
            context.moveTo(0, 0);
            context.lineTo(width, 0);
            context.lineTo(width / 2, height);
            context.closePath();
            context.fillStyle = control.pressed ? colorPressed :colorNor;
            context.fill();
        }
    }

    contentItem: Text {
        leftPadding: 10
        rightPadding: control.indicator.width + control.spacing
        text: control.displayText
        font: control.font
        color: fontColor
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: control.width
        implicitHeight: control.height
        //border.color: control.pressed ? colorPressed : bordColor
        border.width: control.visualFocus ? 2 : 1
        border.color: bordColor
        color: "#272727"
        radius: mRadius
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            //currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }


        }

        background: Rectangle {

            border.width: 1
            border.color: bordColor
            color: "transparent"
            //radius: mRadius
        }
    }
}
