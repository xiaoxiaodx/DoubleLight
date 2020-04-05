/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : asf_adapt.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/02/21
  Description   : 
  History       :
  1.Date        : 2011/02/21
    Author      : l59217
    Modification: Created file

******************************************************************************/
//#include <sys/uio.h>
#include <string.h>
#include "common.h"
#include "hi_avi.h"
#include "avformat.h"


int reader_destroy_avi(void *handle)
{
    HI_Avi_ReadClose(handle);
	
    return 0;
}

int reader_readfileinfo_avi(void *handle, AV_FILE_INFO_S *fileinfo)
{
	HI_AVI_FILE_S	stFileInfo;
	HI_AVI_VIDEO_S 	stVideo;
	int ret;
	
	ret = HI_Avi_GetFileInfo(handle, &stFileInfo);
	if (ret != 0)
	{
		return -1;
	}

	fileinfo->stream_num = stFileInfo.u32StreamNum;

	ret = HI_Avi_GetVideoInfo(handle, &stVideo);
	if (ret != 0)
	{
		return -1;
	}
	
    if ( stVideo.eVEncode == AVI_VIDEO_H264 )
    {
        fileinfo->vcodec_id = CODEC_TYPE_H264;        
    }
    else if ( stVideo.eVEncode == AVI_VIDEO_MJPEG)
    {
        fileinfo->vcodec_id = CODEC_TYPE_MJPEG;        
    }

    fileinfo->width = stVideo.lWidth;
    fileinfo->height = stVideo.lHeight;
    fileinfo->sample_rate = stVideo.lFrameRate;

    second2duration(stFileInfo.struDuration, &fileinfo->duration); 
   
    return 0;
}


int reader_readframe_avi(void *handle, AV_FRAME_S *frame)
{
    int ret;
    HI_AVI_FRAME_S aviframe;
    
    ret = HI_Avi_ReadFrame(handle, &aviframe);
    if(HI_AVI_ERR_ENDFILE == ret)
    {
        return ERR_AV_ENDFILE;
    }
    else if ( ret != 0 )
    {
        return -1;
    }
	
    frame->duration = (aviframe.u32Duration /10000);
    frame->avtype = aviframe.u32StreamType;
    frame->keyflay = aviframe.u32Flags;
    frame->pAddr = aviframe.pu8Data;
    frame->len = aviframe.u32Size;
    frame->pts = aviframe.u64Pts;
	
    return 0;
}


int reader_freeframe_avi(AV_FRAME_S *frame)
{
    return 0;
}


int reader_seekframe_avi(void *handle, int idx, long long ts, int flag)
{
   
    return HI_Avi_SeekNextKeyFrame(handle, (ts*10000));

}


int reader_parsenal_avi(char *pData, int nLen, struct iovec *pVector, int *pNalNum)
{
    
    return 0;
}



