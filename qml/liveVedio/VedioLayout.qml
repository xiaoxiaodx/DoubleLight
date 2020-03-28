import QtQuick 2.0

Rectangle {

    id:root

    property int rowNum: 1
    property int columnNum: 2
    property alias myModel: repeater.model

    property int currentIndex: -1



    signal s_doubleClick(var index,var ismax);
    signal s_click(var clickIndex);
    signal s_showToastMsg(string str)
    signal s_deleteObject()
    signal s1_authenticationFailue(string str)

    color: "#252525"

    Repeater{
        id:repeater


        VideoLivePlay{
            id:video
            width: (index === currentIndex && model.isMax >0)?root.width:((root.width)/columnNum)
            height: (index === currentIndex && model.isMax>0)?root.height:((root.height)/rowNum)

           // x:(index === currentIndex && model.isMax >0)?0:(index* width)
            //y:(index === currentIndex && model.isMax >0)?0:Math.floor(index/rowNum) * height
            x:model.isMax >0?0:index* width
            y:0
            z:model.isMax >0?1:0
            videoType:model.videoType
            color: "#3A3D41"
            mIsSelected: index === currentIndex

          //  belongDeviceName:model.deviceName
            onClick: {



                currentIndex = index

                s_click(currentIndex)
            }

            onDoubleClick: {
                //console.debug("VideoLivePlay onDoubleClick")

                if( model.isMax > 0 )
                     model.isMax = 0;
                else
                    model.isMax = 1;

                s_doubleClick(index,model.isMax)
            }


        }
    }




}
