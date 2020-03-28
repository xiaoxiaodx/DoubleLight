/******************************************************************************

  Copyright (C), 2009-2019, Hisilicon Tech. Co., Ltd.
  ******************************************************************************
  File Name     : hi_avi.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia
  Created       : 2009/08/15
  Description   :
  History       :
  1.Date        : 2009/09/09
  Author        : f65132
  Modification  : Created file
******************************************************************************/
#ifndef __AVI_INTF_H__
#define __AVI_INTF_H__
#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* End of #ifdef __cplusplus */

/*  */
#define	HI_AVI_OK	0

#define	HI_AVI_ERR_ENDFILE         	 0
#define	HI_AVI_ERR_READ_FRAME  		-1
#define HI_AVI_ERR_NULL_POINTER		-2

#if 0
typedef enum
{
    HI_AVI_OK = 0,
    HI_AVI_ERR_READ_FRAME,
    HI_AVI_ERR_ENDFILE,
    HI_AVI_ERR_NULL_POINTER,
} AVI_ERRNO_E;
#endif
typedef enum AVI_CHUNKID_E
{
    AVI_CHUNKID_UNKNOW = 0,
	/*以上是数据体信息*/
	AVI_CHUNKID_AUDIO,
	AVI_CHUNKID_VIDEO,
	AVI_CHUNKID_RGB,    
	AVI_CHUNKID_PC,//AVIPALCHANGE,AVI数据体中movi中的调色板信息
	AVI_CHUNKID_IX,
	AVI_CHUNKID_TXTS,
	AVI_CHUNKID_DRM,//Just for DivX
	AVI_CHUNKID_TXTCAPTION,
	AVI_CHUNKID_BITMAPCAPTION,
	AVI_CHUNKID_CHAPTER,
	AVI_CHUNKID_STANDARDINDEX,
	AVI_CHUNKID_METADATA,
	AVI_CHUNKID_JUNK,
	AVI_CHUNKID_RESERVED,
	/*以上是数据体信息*/
	AVI_CHUNKID_RIFF,
	AVI_CHUNKID_AVIHEADER,//AVI头信息
	AVI_CHUNKID_AVIH,
	AVI_CHUNKID_STRH,
	AVI_CHUNKID_STRF,
	AVI_CHUNKID_STRD,
	AVI_CHUNKID_STRN,
	AVI_CHUNKID_INDX,
	//AVI_CHUNKID_PC,//
	AVI_CHUNKID_LIST,
	AVI_CHUNKID_IDX1,//AVI索引idx1信息
	AVI_CHUNKID_REC,
	//Find now ListChunk
	AVI_CHUNKID_LISTHDRL,
	AVI_CHUNKID_LISTSTRL,
	AVI_CHUNKID_LISTMOVI,
	AVI_CHUNKID_LISTIDX1,
	//AVI_CHUNKID_IDX1;
	
	AVI_CHUNKID_NONE
} AVI_CHUNKID_E;


typedef enum
{
    AVI_VIDEO_H264,
    AVI_VIDEO_MPEG4,
    AVI_VIDEO_MJPEG,
    AVI_VIDEO_UNKNOW,

    AVI_AUDIO_AMR,
    AVI_AUDIO_G711A,
    AVI_AUDIO_G711U,
    AVI_AUDIO_G726,
    AVI_AUDIO_PCM,
    AVI_AUDIO_ADPCM,
    AVI_AUDIO_UNKNOW,

} AVI_CODECTYPE_E;




//////////////////////////////////////////////////////////////////////////
//外部定义结构体

typedef struct
{
    AVI_CODECTYPE_E eVEncode;  //Video encode format
    long lHeight;        //video height
    long lWidth;         //video width
    long lBitRate;       //video bit rate //是有有用到? 视频码率
    long lFrameRate;     //video frame rate// 视频帧率
} HI_AVI_VIDEO_S;

typedef struct
{
    AVI_CODECTYPE_E eAEncode;  //audio encode format
    long lSamplesPerSec;         //audio's samples per second
    long lBitsPerSample;         //bits per sample
    long lBitRate;               //audio's bit rate
    long lBlockAlign;            //if block align
    long lChannels;              //audio's channels
    long lFrameFlag;             //audio's frame flag
    long length;                 //audio's size
    void *pReserved;
} HI_AVI_AUDIO_S;

//file attr
typedef struct
{
    unsigned char u8FormatName[256]; //file format,such as: .avi
    unsigned char u8FileName[256];     //file name
    unsigned long struDuration;                     //file duration
    unsigned int u32StreamNum;                          //the number of stream num（mix stream,single audio,single video stream）
    int s32VideoIndex;                                  //video stream's index
    int s32AudioIndex;                                  //audio stream's index
    HI_BOOL bSeekEnable;                                //the file is whether can be seeked
} HI_AVI_FILE_S;

typedef struct hiAVI_FRAME_S 
{
    HI_U64 u64Pts;       // 时间戳
    HI_U64 u64Dts;
    HI_U8 *pu8Data;//待写的数据
    HI_U32 u32Size;//待写的数据的长度
    HI_U32 u32StreamType; // 帧类型: 1: audio; 2: video
    HI_U32 u32StreamNum;
    HI_U32 u32Duration;   // 单位: 100 ns
    HI_U32 u32Flags;      // 1: key frame;        0: other
    HI_U32 u32OutPutFlag; // 1: video or audio;   0: other
} HI_AVI_FRAME_S;

typedef struct 
{
	HI_U32 ckid;
	HI_U32 FrameLen;
} AVI_FRAME_HEADER;

/************************************************************************/

/************************************************************************/
/* avi文件解析API函数                                                   */
/************************************************************************/
void   HI_Avi_ReadClose(void *pHandle);
void*  HI_Avi_ReadOpen(char *pFileName);
HI_S32 HI_Avi_ReadFrame(void *handle, HI_AVI_FRAME_S *frame);
HI_S32 HI_Avi_SeekNextKeyFrame(void *handle, HI_U64 pts);
HI_S32 HI_Avi_SeekPrevKeyFrame(void *handle, HI_U64 pts);
HI_S32 HI_Avi_GetFileInfo(void * handle, HI_AVI_FILE_S *pInfo);
HI_S32 HI_Avi_GetVideoInfo(void * handle, HI_AVI_VIDEO_S *pVideo);
HI_S32 HI_Avi_GetAudioInfo(void * handle, HI_AVI_AUDIO_S *pAideo);

void* HI_Avi_vReadOpen(char *pFileName, int bufSize, HI_U64* audioPts, HI_U64* vedioPts, HI_U64* audioDeltaPts, HI_U64* vedioDeltaPts );


/************************************************************************/
/* avi 文件打包API函数                                                  */
/************************************************************************/


/***************************************************************************
* Function:      HI_Avi_WriteOpen
* Description:   create avi package handle  
* Input:         * pDecState:     decoder device handle
*                  nCoder:         decoder typeption 
* Output:        none
* Return:        AVI package handle      success
*                NULL                    malloc failed or invalid input parameters
******************************************************************************/
void * HI_Avi_WriteOpen(char *pFileName, HI_AVI_VIDEO_S *pVideo, HI_AVI_AUDIO_S *pAudio );

/***************************************************************************
* Function:      HI_AVI_WriteClose
* Description:   release avi package handle 
* Input:         handle:     AVI package handle
* Output:        none
* Return:        none 
******************************************************************************/
int   HI_Avi_WriteClose(void *handle);

/***************************************************************************
* Function:      HI_Avi_WriteFrame
* Description:   Input a frame  
* Input:         handle:     AVI package handle
*                frame:      audio or video frame
* Output:        none
* Return:        HI_SUCCESS         success
*                HI_FAILURE         invalid input parameters
******************************************************************************/
HI_S32 HI_Avi_WriteFrame(void *handle, HI_AVI_FRAME_S *frame);



void HI_AVI_Parse_Header_StreamData(HI_U32 FourCC, HI_U32 *pStreamNum,AVI_CHUNKID_E  *pStreamType);


#if 0
HI_S32 HI_Avi_ReadLarge(void *handle, HI_CHAR* buffer, HI_CHAR** pOffset, HI_S32 bufSize, HI_S32* leftSize,HI_AVI_FRAME_S *frame);
#else
HI_S32 HI_Avi_ReadLarge( void *handle, HI_CHAR* buffer, HI_S32 bufSize, HI_S32 adjOffset );
#endif
HI_S32 HI_Avi_ReadIdx( void *handle, HI_CHAR* idxBuffer, HI_S32 bufSize, HI_S32 adjOffset );

HI_S32 HI_Avi_Jump2_Vedio( void *handle, HI_U32 offsetSecs, HI_U64 *currAudioPts, HI_U64 *currVedioPts );


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __AVI_INTF_H__ */
