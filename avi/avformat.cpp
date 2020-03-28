/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : avformat.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2007/06/18
  Description   : 
  History       :
  1.Date        : 2007/06/18
    Author      : qushen
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avformat.h>
// cht #include "asfenc_adapt.h"
// cht #include "mp4enc_adapt.h"
#include <avienc_adapt.h>
// cht #include "priv_container_enc_adapt.h"
//#include "3gpenc_adapt.h"
//#include "file.h"
#include <adapt.h>

#include <debug.h>



static AV_OUTPUT_FORMAT_S *first_oformat = NULL;
FORMAT_TYPE_E file_name2format(char * filename);

/*****************************************************************************
 Prototype       : avformat_output_register_format
 Description     : ...
 Input           : AV_OUTPUT_FORMAT_S *fmt
 Output          : None
 Return Value    : int
 Global Variable   
    Read Only    : 
    Read & Write : 
  History         
  1.Date         : 2007/8/2
    Author       : qushen
    Modification : Created function

*****************************************************************************/
void avformat_output_register_format( AV_OUTPUT_FORMAT_S *fmt )
{
    adapt_register_format(first_oformat, AV_OUTPUT_FORMAT_S, fmt);
}


/*****************************************************************************
 Prototype       : avformat_output_find
 Description     : ...
 Input           : None
 Output          : None
 Return Value    : int
 Global Variable   
    Read Only    : 
    Read & Write : 
  History         
  1.Date         : 2007/8/2
    Author       : qushen
    Modification : Created function

*****************************************************************************/
static AV_OUTPUT_FORMAT_S* avformat_output_find_format( const int fmttype )
{
    AV_OUTPUT_FORMAT_S  *p;
    adapt_find_format(first_oformat, AV_OUTPUT_FORMAT_S, fmttype, p);//p为出参，找到的AV_OUTPUT_FORMAT_S
    return p;
}

/*****************************************************************************
 Prototype       : avformat_output_init
 Description     : ...
 Input           : void
 Output          : None
 Return Value    : int
 Global Variable   
    Read Only    : 
    Read & Write : 
  History         
  1.Date         : 2007/8/2
    Author       : qushen
    Modification : Created function

*****************************************************************************/
static int avformat_output_init( void )
{
    static int inited = 0;

    if (inited != 0)
        return 0;
    inited = 1;

    //cht writer_asf_init();
    // cht writer_mp4_init();    
    writer_avi_init();
    //writer_3gp_init();    
    // cht writer_priv_container_init();

    if ( first_oformat == NULL )
    {
        error("not regesister file format!\n");
        return -1;
    }    
    return 0;
}


int writer_create(void **handle, char *filename)//handle 是出参
{
    int ret;
    FORMAT_TYPE_E   fmttype;
    AV_OUTPUT_FORMAT_S  *fmt;
    ADAPT_S *pAdapt;

    /*init av output lib*/
    ret = avformat_output_init();//调用writer_create就会初始化lib avformat_output_init可重复调用，内有保护
    if ( ret != 0 )
    {
        return -1;
    }

    /*find match type*/
    fmttype = file_name2format(filename);//根据文件后缀判断文件类型
    fmt = avformat_output_find_format(fmttype);
    if ( fmt == NULL )
    {
        return -1;
    }

    pAdapt = (ADAPT_S *)malloc(sizeof(ADAPT_S));
    if ( pAdapt == NULL )
    {
        error("no mem!\n");
        return -1;
    }
    pAdapt->fmt = fmt;

    /*do create operation*/
    if ( fmt->writer_create )//writer_create_avi
    {
        //pAdapt->priv_data是出参, 存的是AviHandle_S的数据结构
        ret = fmt->writer_create(&(pAdapt->priv_data), filename);
        
        if ( ret != 0 )
        {
            error("create error!\n");
            free(pAdapt);
            return -1;
        }
    }
    
    /*return pointer to save*/
    *handle = pAdapt;    
    return 0;
}


int writer_destroy(void *handle)
{
    int ret = -1;
    ADAPT_S *pAdapt;
    AV_OUTPUT_FORMAT_S  *fmt;

    pAdapt = (ADAPT_S *)handle;
    fmt = (AV_OUTPUT_FORMAT_S*)pAdapt->fmt;
    if ( fmt->writer_destroy )//writer_destroy_avi
    {
        ret = fmt->writer_destroy(pAdapt->priv_data);
    }
    free(handle);
    return 0;
}
int writer_setvideoparam(void *handle, int *idx, VIDEO_PARAM_S *param)
{
    int ret = -1;
    ADAPT_S *pAdapt;
    AV_OUTPUT_FORMAT_S  *fmt;

    pAdapt = (ADAPT_S *)handle;
    fmt = (AV_OUTPUT_FORMAT_S*)pAdapt->fmt;
    if ( fmt->writer_setvideoparam )//writer_setvideoparam_avi
    {
        ret = fmt->writer_setvideoparam(pAdapt->priv_data, idx, param);
    }
    return ret;

}
int writer_setaudioparam(void *handle, int *idx, AUDIO_PARAM_S *param)
{
    int ret = -1;
    ADAPT_S *pAdapt;
    AV_OUTPUT_FORMAT_S  *fmt;

    pAdapt = (ADAPT_S *)handle;
    fmt = (AV_OUTPUT_FORMAT_S*)pAdapt->fmt;
    if ( fmt->writer_setaudioparam )//writer_setaudioparam_avi
    {
        ret = fmt->writer_setaudioparam(pAdapt->priv_data, idx, param);
    }
    return ret;

}

int writer_begin(void *handle)
{
    int ret = -1;
    ADAPT_S *pAdapt;
    AV_OUTPUT_FORMAT_S  *fmt;

    pAdapt = (ADAPT_S *)handle;
    fmt = (AV_OUTPUT_FORMAT_S*)pAdapt->fmt;

    if ( fmt->writer_begin )//writer_begin_avi cht
    {
        ret = fmt->writer_begin(pAdapt->priv_data);
    }

    return ret;

}
int writer_end(void *handle)
{
    int ret = -1;
    ADAPT_S *pAdapt;
    AV_OUTPUT_FORMAT_S  *fmt;

    pAdapt = (ADAPT_S *)handle;
    fmt = (AV_OUTPUT_FORMAT_S*)pAdapt->fmt;

    REC_DEBUG(DBG_INFO,"$$$$$ writer_end called\n");
    
    if ( fmt->writer_end ) //writer_end_avi
    {
        ret = fmt->writer_end(pAdapt->priv_data);
    }
    return ret;

}

//idx 写视频帧还是写音频帧
int writer_writeframe(void *handle, int idx, long long ts, unsigned char *pAddr, int len, int keyflag)
{
    int ret = -1;
    ADAPT_S *pAdapt;
    AV_OUTPUT_FORMAT_S  *fmt;//format中存放的是函数指针

    pAdapt = (ADAPT_S *)handle;
    fmt = (AV_OUTPUT_FORMAT_S*)pAdapt->fmt;

    
    if ( fmt->writer_writeframe )//writer_writeframe_avi
    {
        //printf("********  writer_writeframe  called at %s, line %d, the idx is %d\n", __FILE__,__LINE__, idx);
        ret = fmt->writer_writeframe(pAdapt->priv_data, idx, ts, pAddr, len, keyflag);
    }
    return ret;
}

int writer_writeframe2(void *handle, int idx, long long ts, struct iovec *pVector, int num, int keyflag)
{
    int ret = -1;
    ADAPT_S *pAdapt;
    AV_OUTPUT_FORMAT_S  *fmt;

    pAdapt = (ADAPT_S *)handle;
    fmt = (AV_OUTPUT_FORMAT_S*)pAdapt->fmt;
    if ( fmt->writer_writeframe2 )
    {
        ret = fmt->writer_writeframe2(pAdapt->priv_data, idx, ts, pVector, num, keyflag);
    }
    return ret;
}


void second2duration(int sec, AV_DURATION_S *pDuration)
{
    pDuration->hour = sec / 3600;
    pDuration->min = (sec%3600) / 60;
    pDuration->sec = (sec%3600) % 60;
}

int duration2second(AV_DURATION_S *pDuration)
{
    return pDuration->hour*3600 + pDuration->min*60 + pDuration->sec;   
}


/*****************************************************************************
 Prototype       : file_name2format
 Description     : 获取文件格式信息，如ASF, MP4, JPG等
 Input           : filename  **
 Output          : None
 Return Value    : 
 Global Variable   
    Read Only    : 
    Read & Write : 
  History         
  1.Date         : 2007/7/30
    Author       : qushen
    Modification : Created function

*****************************************************************************/
FORMAT_TYPE_E file_name2format(char * filename)
{
    char *p = NULL;
    p = strrchr(filename, '.');//返回.xxx
    if ( p == NULL )
    {
        return FORMAT_TYPE_BUTT;
    }

    if ( !strncasecmp(p, ".asf", 4))
    {
        return FORMAT_TYPE_ASF;
    }
    else if( !strncasecmp(p, ".avi", 4))
    {
    	  return FORMAT_TYPE_AVI;
    }
    else if( !strncasecmp(p, ".mp4", 4) )
    {
        return FORMAT_TYPE_MP4;
    }
    else if( !strncasecmp(p, ".3gp", 4) )
    {
        return FORMAT_TYPE_3GP;
    }
    else if( !strncasecmp(p, ".mp3", 4) )
    {
        return FORMAT_TYPE_MP3;
    }
    else if( !strncasecmp(p, ".jpg", 4) || !strncasecmp(p, ".jpeg", 5) )
    {
        return FORMAT_TYPE_JPG;
    }
    else if( !strncasecmp(p, ".png", 4) )
    {
        return FORMAT_TYPE_PNG;
    }
    else if( !strncasecmp(p, ".bmp", 4) )
    {
        return FORMAT_TYPE_BMP;
    }
    else if( !strncasecmp(p, ".gif", 4) )
    {
        return FORMAT_TYPE_GIF;
    }
    else if( !strncasecmp(p, ".pri", 4) )
    {
        return FORMAT_TYPE_PRIV;
    }

    return FORMAT_TYPE_BUTT;
}



