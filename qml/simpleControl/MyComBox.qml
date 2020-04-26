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
    //显示内容属性
    property int contentFontSize: 16
    property string contentFontColor: "#ffffff"
    property string contentBg: "#999999"

    //指示器属性
    property alias indicatorImgSrc: imgIndiator.source
    property int indicatorW: 6
    property int indicatorH: 4
    //子项显示内容属性
    property int itemLeftMargin: 5
    property int itemTopMargin: 5
    property int itemHeight: 34
    property string itembordColor: bordColor //子项的边界宽不能设置
    property string itemColorBgNor: "#3A3D41"
    property string itemColorBgHoverd: "#272727"
    property int itemFontSize: 14
    property string itemFontColor: "#5A5E66"
    //
    property string bordColor: "transparent"
    property int mRadius: 10

    delegate: ItemDelegate {
        width: control.width
        height:itemHeight
        contentItem: Text {
            text: showStr
            color: itemFontColor
            font.pixelSize: itemFontSize
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
            color: control.highlightedIndex === index?itemColorBgHoverd:itemColorBgNor
        }
    }

    indicator:Image {
        id: imgIndiator
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: indicatorW
        height: indicatorH
    }

        /*Canvas {
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
    }*/

    contentItem: Text {
        leftPadding: 10
        rightPadding: control.indicator.width + control.spacing
        text: control.displayText
        color: contentFontColor
        font.pixelSize: contentFontSize
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: control.width
        implicitHeight: control.height
        border.width: control.visualFocus ? 2 : 1
        border.color: bordColor
        color: contentBg
        radius: mRadius
    }

    popup: Popup {
        x:itemLeftMargin
        y: control.height + itemTopMargin
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1
        //height: 120
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
            radius: mRadius
        }
    }
}
