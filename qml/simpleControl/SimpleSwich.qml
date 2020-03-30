import QtQuick 2.0

Image {

    id:swichtBtn
    property bool checked: false
    width: 30
    height: 15
    source: checked?"qrc:/images/switch_off.png":"qrc:/images/switch_on.png"
    MouseArea{

        anchors.fill: parent

        onClicked: checked = !checked
    }

}
