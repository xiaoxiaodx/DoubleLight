/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : avformat.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2007/06/19
  Description   : avformat.c header file
  History       :
  1.Date        : 2007/06/19
    Author      : qushen
    Modification: Created file

******************************************************************************/

#ifndef __AVFORMAT_H__
#define __AVFORMAT_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

//#include <sys/uio.h>

#include "hi_type.h"
#include "hi_avi.h"

/***************************************************************/
#define AVI_FRAME_HDR_LEN      sizeof(AVI_FRAME_HEADER)


/***************************************************************/

typedef struct hiAV_DURATION_S
{
    int hour;
    int min;
    int sec;
} AV_DURATION_S;

typedef struct hiAV_FILE_INFO_S
{
    int stream_num;
    int video_index;
    int audio_index;    
    int binary_index;

    int vcodec_id;
    int width;
    int height;
    int bps;

    int acodec_id;
    int sample_rate;
    int channel;

    AV_DURATION_S duration;
} AV_FILE_INFO_S;

typedef struct hiAV_FRAME_S
{
    unsigned long long   pts;        /*presentation time stamp in time_base units*/
    unsigned char       *pAddr;      /*frame addr(or binary addr)*/
    unsigned int         len;        /*frame length*/
    unsigned int         avtype;     /*index video, audio or binary*/
    unsigned int         keyflay;    /*key flag, eg I frame*/
    unsigned int         duration;   /* presentation duration in time_base units (0 if not available)*/    
} AV_FRAME_S;

typedef enum hiFORMAT_TYPE_E
{
    /* video */
    FORMAT_TYPE_ASF = 0,
    FORMAT_TYPE_MP4,
    FORMAT_TYPE_3GP,
    FORMAT_TYPE_AVI,
    /* music */
    FORMAT_TYPE_MP3,

    /* image */
    FORMAT_TYPE_JPG,    /* including JPEG */
    FORMAT_TYPE_PNG,
    FORMAT_TYPE_BMP,
    FORMAT_TYPE_GIF,

    /*customer private container*/
    FORMAT_TYPE_PRIV,
  
    FORMAT_TYPE_BUTT
} FORMAT_TYPE_E;

typedef enum hiCODEC_TYPE_E
{
    CODEC_TYPE_H264 = 0,
    CODEC_TYPE_MJPEG,
    CODEC_TYPE_AAC ,
    CODEC_TYPE_MP3 ,
    CODEC_TYPE_G726 ,
    CODEC_TYPE_G711A ,
    CODEC_TYPE_G711Mu ,
    CODEC_TYPE_BUTT
} CODEC_TYPE_E;


typedef struct hiVIDEO_PARAM_S
{
	int                 eCodeid;
    unsigned int		u32BiWidth;
    unsigned int		u32BiHeight;
    unsigned int        u32BitCount;
    unsigned int        u32avgBitRate;
    unsigned int        u32samplePerSec;
    unsigned int        u32IsVBR ;
    unsigned int        u32NTSC ;    
} VIDEO_PARAM_S;

typedef struct hiAUDIO_PARAM_S
{
	int                   eCodeid;
	unsigned int		  u32Channels;
	unsigned int		  u32SamplesPerSec; 
	unsigned int		  u32AvgBytesPerSec;
	unsigned short		  u16BlockAlign; 
	unsigned short		  u16BitsPerSample;     
} AUDIO_PARAM_S;

typedef struct hiAV_OUTPUT_FORMAT_S
{
    int  type;
    char format_name[32];
    int (*writer_create)(void **handle, char *filename);
    int (*writer_destroy)(void *handle);
    int (*writer_setvideoparam)(void *handle, int *idx, VIDEO_PARAM_S *param);
    int (*writer_setaudioparam)(void *handle, int *idx, AUDIO_PARAM_S *param);    
    int (*writer_begin)(void *handle);
    int (*writer_end)(void *handle);    
    int (*writer_writeframe)(void *handle, int idx, long long ts, unsigned char *pAddr, int len, int keyflag);
    int (*writer_writeframe2)(void *handle, int idx, long long ts, struct iovec *pVector, int num, int keyflag);    
    struct hiAV_OUTPUT_FORMAT_S *next;
} AV_OUTPUT_FORMAT_S;

typedef struct hiAV_INPUT_FORMAT_S
{
    int  type;    
    char format_name[32];
    int (*reader_create)(void **handle, char *filename);
    int (*reader_destroy)(void *handle);
    int (*reader_readfileinfo)(void *handle, AV_FILE_INFO_S *fileinfo);
    int (*reader_readframe)(void *handle, AV_FRAME_S *frame);    
    int (*reader_freeframe)(AV_FRAME_S *frame);    
    int (*reader_seekframe)(void *handle, int idx, long long ts, int flag);
    int (*reader_parsenal)(char *pAddr, int len, struct iovec *pVector, int *num);
    struct hiAV_INPUT_FORMAT_S  *next;
} AV_INPUT_FORMAT_S;


#define ERR_AV_REGISTER_AV -1
#define ERR_AV_OPEN_FILE -2
#define ERR_AV_ENDFILE -3
#define ERR_AV_READ_FRAME -4


/*reader api*/
void avformat_input_register_format( AV_INPUT_FORMAT_S *fmt );
int reader_create(void **handle, char *filename);
int reader_destroy(void *handle);
int reader_freeframe(void *handle, AV_FRAME_S *frame);
int reader_parsenal(void *handle, char *pAddr, int len, struct iovec *pVector, int *num);
int reader_readfileinfo(void *handle, AV_FILE_INFO_S *fileinfo);
int reader_readframe(void *handle, AV_FRAME_S *frame);
int reader_seekframe(void *handle, int idx, long long ts, int flag);


/*writer api*/
void avformat_output_register_format( AV_OUTPUT_FORMAT_S *fmt );
int writer_begin(void *handle);
int writer_create(void **handle, char *filename);
int writer_destroy(void *handle);
int writer_end(void *handle);
int writer_setaudioparam(void *handle, int *idx, AUDIO_PARAM_S *param);
int writer_setvideoparam(void *handle, int *idx, VIDEO_PARAM_S *param);
int writer_writeframe(void *handle, int idx, long long ts, unsigned char *pAddr, int len, int keyflag);
int writer_writeframe2(void *handle, int idx, long long ts, struct iovec *pVector, int num, int keyflag);

/*tools function*/
int  duration2second(AV_DURATION_S *pDuration);
void second2duration(int sec, AV_DURATION_S *pDuration);
FORMAT_TYPE_E file_name2format(char * filename);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __AVFORMAT_H__ */
