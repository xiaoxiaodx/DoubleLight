import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
    id: control

    property string bgColor: "red"
    property string indicatorColor: "white"

    property bool checked: true

    property int radiusX: checked?(width-height/2):height/2


    signal checkedChange(bool value)


    property int bgHeight: 2

    property int deXY: 2

    property bool isSurround:true



    color: "#0000ff00"
    Rectangle{

        height: isSurround?parent.height:2*parent.height/3
        width: parent.width
        color: "white"
        radius: height/2
        anchors.verticalCenter: parent.verticalCenter
        Canvas {
            id: mycanvas
            width: parent.width
            height: parent.height


            // renderStrategy:Canvas.Threaded
            onPaint: {
                var ctx = getContext("2d")

                ctx.clearRect(0,0,width,height)
                ctx.fillStyle = bgColor
                ctx.beginPath();
                ctx.arc(parent.height/2,parent.height/2,parent.height/2,Math.PI/2,3*Math.PI/2);
                ctx.arc(radiusX,parent.height/2,parent.height/2,3*Math.PI/2,5*Math.PI/2);
                ctx.fill();

            }


        }
    }
    Rectangle{
        id:rectIndicator
        x:checked?(parent.width - parent.height):0
        width:parent.height
        height:parent.height
        radius: parent.height/2
        color: indicatorColor
        border.width: 1
        border.color:"#cccccc"
    }




    MouseArea{
        anchors.fill: parent
        onClicked: timer.start()

    }
    Timer{
        id:timer
        repeat: true
        interval: 5

        onTriggered:{


            if(!checked){
                radiusX += deXY;
                rectIndicator.x += deXY
                if(radiusX >= (control.width - control.height/2)){


                    timer.stop()
                    checked = !checked
                    checkedChange(checked)
                }
            }else{
                radiusX -= deXY;
                rectIndicator.x -= deXY
                if(radiusX <= control.height/2){


                    timer.stop()
                    checked = !checked
                    checkedChange(checked)
                }
            }
            mycanvas.requestPaint()

        }


    }


}
