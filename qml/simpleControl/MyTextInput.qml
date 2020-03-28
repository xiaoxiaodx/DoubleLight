import QtQuick 2.0

TextInput {
    id: control

    property string cursorColor: "black"

    selectByMouse: true
    color:"red"
    focus: false
    cursorDelegate:cursor
    cursorVisible: false
    Component
    {
        id:cursor
        Rectangle
        {
            width: 0
            visible: false
            height: 0
            color:cursorColor
        }
    }
}
