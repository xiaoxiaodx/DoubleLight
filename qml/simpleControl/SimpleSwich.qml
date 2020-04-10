import QtQuick 2.0

Image {

    id:swichtBtn
    property bool checked: true
    width: 30
    height: 15
    source: checked?"qrc:/images/switch_on.png":"qrc:/images/switch_off.png"
    MouseArea{

        anchors.fill: parent

        onClicked: {
            if(checked)
                checked = false
            else
                checked = true


        }

    }

}
