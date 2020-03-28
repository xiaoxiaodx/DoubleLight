/*
  父类使用该组件格式,延迟加载

            Loader{
                id:loaderToast
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 100
                sourceComponent: null
            }

            Component {
                id: toast
                QmlToast{

                    txtStr:msgStrTip//"谁发的撒加咖啡大数据按快递费拉撒花赶快gfhd辅导辅导撒个梵蒂冈非官方大哥GV成本来"
                    backColor: "#ffffff00"
                    txtColor:"#ee555555"
                    maxWid: video.width/2

                }
            }
*/

import QtQuick 2.0
import QtQuick 2.12

Rectangle{
    id:toast

    property color backColor: "black"// "#aaaaaaaa"
    property color txtColor: "#ee222222"
    property string txtStr: ""
    property real maxWid: parent.width/2

    opacity: 0
    color: backColor
    width: txt.width
    height: txt.height
   // radius: height/2

    Text {
        id: txt
        text: qsTr(txtStr)
        padding: 12
        wrapMode:Text.WrapAnywhere
        color:txtColor

        //font.bold: true
        font.wordSpacing: 1
        font.pixelSize: 18

       // horizontalAlignment: Text.AlignHCenter//(1)
        verticalAlignment: Text.AlignVCenter//(2)

        onWidthChanged: {

            if(maxWid != 0 && width > maxWid){
                width = maxWid
            }


        }
    }

    SequentialAnimation on opacity {
        NumberAnimation {to: 1; easing.type: Easing.InQuad; duration: 500}
        PauseAnimation { duration: 2000}
        NumberAnimation { to: 0; easing.type: Easing.InQuad; duration: 500}
    }
}

