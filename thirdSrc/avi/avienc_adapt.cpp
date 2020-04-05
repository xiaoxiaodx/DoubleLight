/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : asfenc_adpat.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/02/21
  Description   : 
  History       :
  1.Date        : 2011/02/21
    Author      : l59217
    Modification: Created file

******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "hi_avi.h"
#include "avienc_adapt.h"
#include "debug.h"

#define MAX_FILE_NAME_LEN		128


typedef struct stAviHandle
{
	HI_AVI_VIDEO_S 	stAviVideo;
	HI_AVI_AUDIO_S	stAviAudio;
	char			strFileName[MAX_FILE_NAME_LEN];
	void*			pHiAviHandle;
}AviHandle_S;


#define VIDEO_INDEX 0
#define AUDIO_INDEX 1

int writer_create_avi(void **handle, char *filename)
{
	AviHandle_S* pAviHandle = NULL;


	pAviHandle = (AviHandle_S*)malloc(sizeof(AviHandle_S));
	if (pAviHandle == NULL)
	{
		perror("writer_create_avi malloc err \n");
		return -1;
	}

	 pAviHandle->pHiAviHandle = NULL;
	 memcpy(pAviHandle->strFileName, filename, MAX_FILE_NAME_LEN);
	 
	*handle = pAviHandle;
	
	return 0;
}


int writer_destroy_avi(void *handle)//why writer_destroy_avi and writer_end_avi both call the HI_Avi_WriteClose??
{
    AviHandle_S* pAviHandle = NULL;
	int ret = 0;
	
    if (handle != NULL)
    {
		pAviHandle = (AviHandle_S*)handle;
		if (pAviHandle->pHiAviHandle != NULL)
		{
			ret = HI_Avi_WriteClose(pAviHandle->pHiAviHandle);
			pAviHandle->pHiAviHandle = NULL;
		}
  		free(handle);
		handle = NULL;
		return ret;
    }
	
    return ret;
}

//int (*writer_setparam)(void *handle, AV_PARAM_S *param);
//param入参,将param中的信息写到handle中去

//idx为出参，*idx = VIDEO_INDEX，比较奇怪，考虑去掉//这个是一个屏蔽内部数据的作用
int writer_setvideoparam_avi(void *handle, int *idx, VIDEO_PARAM_S *param)
{
	AviHandle_S* pAviHandle = NULL;

	if (handle == NULL)
	{
		printf("writer_setvideoparam_avi NULL handle \n");
		return -1;
	}
	pAviHandle = (AviHandle_S*)handle;

	memset(&pAviHandle->stAviVideo, 0, sizeof(HI_AVI_VIDEO_S));
	
    if ( param->eCodeid == CODEC_TYPE_H264 )
    {
        pAviHandle->stAviVideo.eVEncode = AVI_VIDEO_H264;//设置编码格式
    }
    else if( param->eCodeid == CODEC_TYPE_MJPEG)
    {
        pAviHandle->stAviVideo.eVEncode =AVI_VIDEO_MJPEG ;
    }
     
    pAviHandle->stAviVideo.lHeight = param->u32BiHeight;
    pAviHandle->stAviVideo.lWidth = param->u32BiWidth;
    pAviHandle->stAviVideo.lFrameRate = param->u32samplePerSec;
    pAviHandle->stAviVideo.lBitRate = (param->u32avgBitRate > 10);//?????
    #if 0
    printf("###############h:%d w:%d, fps %d, bps %d \n", pAviHandle->stAviVideo.lHeight, pAviHandle->stAviVideo.lWidth,
	pAviHandle->stAviVideo.lFrameRate, pAviHandle->stAviVideo.lBitRate);
    #endif
	*idx = VIDEO_INDEX;
	
    return 0;
}

int writer_setaudioparam_avi(void *handle, int *idx, AUDIO_PARAM_S *param)
{
	AviHandle_S* pAviHandle = NULL;

	if (handle == NULL)
	{
		printf("writer_setvideoparam_avi NULL handle \n");
		return -1;
	}
	pAviHandle = (AviHandle_S*)handle;

	memset(&pAviHandle->stAviAudio, 0, sizeof(HI_AVI_AUDIO_S));
	
	if ( param->eCodeid == CODEC_TYPE_G726)
	{
		pAviHandle->stAviAudio.eAEncode = AVI_AUDIO_G726;
        pAviHandle->stAviAudio.lBitsPerSample = 2;//g726位深是2
	}    
	else if(param->eCodeid == CODEC_TYPE_G711A)
	{
		pAviHandle->stAviAudio.eAEncode = AVI_AUDIO_G711A;
        pAviHandle->stAviAudio.lBitsPerSample = 8;//位深
	}
	else if(param->eCodeid == CODEC_TYPE_G711Mu)
	{
		pAviHandle->stAviAudio.eAEncode = AVI_AUDIO_G711U;
        pAviHandle->stAviAudio.lBitsPerSample = 8;//位深
	}

	pAviHandle->stAviAudio.lBitRate = pAviHandle->stAviAudio.lBitsPerSample * param->u32SamplesPerSec;
	pAviHandle->stAviAudio.lBlockAlign = param->u16BlockAlign;	
	pAviHandle->stAviAudio.lChannels = param->u32Channels;
	pAviHandle->stAviAudio.lSamplesPerSec = param->u32SamplesPerSec;
	pAviHandle->stAviAudio.lFrameFlag = 0;		//???
	*idx = AUDIO_INDEX;
    
    return 0;
}


int writer_begin_avi(void *handle)
{
	AviHandle_S* pAviHandle = NULL;

	if (handle == NULL)
	{
		REC_DEBUG(DBG_ERR,"writer_setvideoparam_avi NULL handle \n");
		return -1;
	}
	pAviHandle = (AviHandle_S*)handle;

	//pAviHandle->stAviVideo,pAviHandle->stAviAudio这两个参数是在调用REC_GetWriterIndex->writer_setvideoparam_avi
	//writer_setaudioparam_avi中赋值的
	pAviHandle->pHiAviHandle = HI_Avi_WriteOpen(pAviHandle->strFileName, &pAviHandle->stAviVideo, &pAviHandle->stAviAudio );
	if (pAviHandle->pHiAviHandle == NULL)
	{
		REC_DEBUG(DBG_ERR,"writer_begin_avi: HI_Avi_WriteOpen ERR \n");
		return -1;
	}
	
 	return 0;
}


int writer_end_avi(void *handle)
{
	AviHandle_S* pAviHandle = NULL;
	int ret = 0;
	
	if (handle == NULL)
	{
		REC_DEBUG(DBG_ERR,"writer_setvideoparam_avi NULL handle \n");
		return -1;
	}
	pAviHandle = (AviHandle_S*)handle;

	if (pAviHandle->pHiAviHandle != NULL)
	{
	        REC_DEBUG(DBG_INFO,"$_$_$_$  writer_end_avi called\n");
            
     		ret = HI_Avi_WriteClose(pAviHandle->pHiAviHandle);
	}
	pAviHandle->pHiAviHandle = NULL;

	return ret;
}

/*
    handle: IN AviHandle_S
    idx: IN 写视频还是写音频帧
    ts: IN time stamp
    pAddr: IN 待写入数据的内存地址
    len: IN 待写入数据的长度
    keyflag: IN 待写入数据是否很关键
*/

int writer_writeframe_avi(void *handle, int s32IsAudioFrame, long long ts, unsigned char *pAddr, int len, int keyflag)
{
	int ret = 0;
	HI_AVI_FRAME_S  frame;

	AviHandle_S* pAviHandle = NULL;

	if (handle == NULL)
	{
		printf("[error]:writer_writeframe_avi NULL handle at %s, %d\n", __FILE__, __LINE__ );
		return -1;
	}
	pAviHandle = (AviHandle_S*)handle;

	if (pAviHandle->pHiAviHandle == NULL)
	{
		printf("[error]:writer_writeframe_avi: NULL HiAvi handle for write not open at %s, %d\n", __FILE__, __LINE__);
		return -1;
	}
	
	memset(&frame, 0, sizeof(HI_AVI_FRAME_S));//u32StreamType怎么没有置位这个? cht
	frame.pu8Data = pAddr;
	frame.u32Size = len;
	frame.u64Pts = ts;//最终这个ts并没有被写入到avi文件中
    
	if ( s32IsAudioFrame > 0)
	{
		frame.u32StreamType = 1;//(以前这个地方是frame.u32StreamNum = 1)通过这个来告知是音频帧还是视频帧??? 确定不是u32StreamType?? //这里有问题，需要改动
	}
	else if( 0 == s32IsAudioFrame )
	{
		frame.u32StreamType = 2;
	}
	else
	{
	    printf("[error]:writer_writeframe_avi para error at %s, %d\n", __FILE__, __LINE__ );
		return -1;
	}
    
	frame.u32Flags = keyflag;

    //printf("####  writer_writeframe_avi  called at %s, line %d, the u32StreamType is %d\n", __FILE__,__LINE__, frame.u32StreamType);

	ret = HI_Avi_WriteFrame(pAviHandle->pHiAviHandle, &frame);
	if (ret != 0)
	{
		printf("#################### HI_Avi_WriteFrame ERR %x \n", ret);
		return -1;
	}
	return 0;
}

/*****************************************************************************
 Prototype       : writer_avi_init
 Description     : ...
 Input           : void
 Output          : None
 Return Value    : int
 Global Variable   
    Read Only    : 
    Read & Write : 
  History         
  1.Date         : 2011/2/21
    Author       : l59217
    Modification : Created function

*****************************************************************************/
int writer_avi_init()
{
    static AV_OUTPUT_FORMAT_S avi_format;//静态变量
  	
    avi_format.type = FORMAT_TYPE_AVI;
    avi_format.writer_create = writer_create_avi;
    avi_format.writer_destroy = writer_destroy_avi;
    avi_format.writer_setvideoparam = writer_setvideoparam_avi;
    avi_format.writer_setaudioparam = writer_setaudioparam_avi;
    avi_format.writer_begin = writer_begin_avi;
    avi_format.writer_end = writer_end_avi;
    avi_format.writer_writeframe = writer_writeframe_avi;
    avi_format.writer_writeframe2 = NULL;
	
    avformat_output_register_format(&avi_format);
    return 0;
}

