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
#include "common.h"
#include "avformat.h"
//#include "file.h"
//cht delete  #include "asf_adapt.h"
//cht delete #include "mp4dec_adapt.h"
//#include "3gp_adapt.h"
#include "adapt.h"

static AV_INPUT_FORMAT_S *first_iformat = NULL;


/*****************************************************************************
 Prototype       : avformat_input_register_format
 Description     : ...
 Input           : AV_INPUT_FORMAT_S *fmt
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
void avformat_input_register_format( AV_INPUT_FORMAT_S *fmt )
{
    adapt_register_format(first_iformat, AV_INPUT_FORMAT_S, fmt);
}


/*****************************************************************************
 Prototype       : avformat_input_find
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
static AV_INPUT_FORMAT_S* avformat_input_find_format( const int fmttype )
{
    AV_INPUT_FORMAT_S  *p;
    adapt_find_format(first_iformat, AV_INPUT_FORMAT_S, fmttype, p);
    return p;
}

/*****************************************************************************
 Prototype       : avformat_input_init
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
static int avformat_input_init( void )
{
    static int inited = 0;

    if (inited != 0)
        return 0;
    inited = 1;

    //reader_asf_init();
    //reader_mp4_init();
    //reader_3gp_init();

    if ( first_iformat == NULL )
    {
        error("not regesister file format!\n");
        return -1;
    }    
    return 0;
}


int reader_create(void **handle, char *filename)
{
    int ret;
    FORMAT_TYPE_E   fmttype;
    AV_INPUT_FORMAT_S  *fmt;
    ADAPT_S *pAdapt;

    /*init av output lib*/
    ret = avformat_input_init();
    if ( ret != 0 )
    {
        return -1;
    }

    /*find match type*/
    fmttype = file_name2format(filename);
    fmt = avformat_input_find_format(fmttype);
    if ( fmt == NULL )
    {
        return -1;
    }

    pAdapt = (ADAPT_S*)malloc(sizeof(ADAPT_S));
    if ( pAdapt == NULL )
    {
        error("no mem!\n");
        return -1;
    }
    pAdapt->fmt = fmt;

    /*do create operation*/
    if ( fmt->reader_create )
    {
        ret = fmt->reader_create(&(pAdapt->priv_data), filename);
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


int reader_destroy(void *handle)
{
    int ret = -1;
    ADAPT_S *pAdapt;
    AV_INPUT_FORMAT_S  *fmt;

    pAdapt = (ADAPT_S*)handle;
    fmt = (AV_INPUT_FORMAT_S *)pAdapt->fmt;
    if ( fmt->reader_destroy )
    {
        ret = fmt->reader_destroy(pAdapt->priv_data);
    }
    free(handle);
    return 0;
}

int reader_readfileinfo(void *handle, AV_FILE_INFO_S *fileinfo)
{
    int ret = -1;
    ADAPT_S *pAdapt;
    AV_INPUT_FORMAT_S  *fmt;

    pAdapt = (ADAPT_S*)handle;
    fmt = (AV_INPUT_FORMAT_S *)pAdapt->fmt;
    if ( fmt->reader_readfileinfo )
    {
        ret = fmt->reader_readfileinfo(pAdapt->priv_data, fileinfo);
    }
    return ret;

}

int reader_readframe(void *handle, AV_FRAME_S *frame)
{
    int ret = -1;
    ADAPT_S *pAdapt;
    AV_INPUT_FORMAT_S  *fmt;

    pAdapt = (ADAPT_S*)handle;
    fmt = (AV_INPUT_FORMAT_S *)pAdapt->fmt;
    if ( fmt->reader_readframe )
    {
        ret = fmt->reader_readframe(pAdapt->priv_data, frame);
    }
    return ret;

}

int reader_freeframe(void *handle, AV_FRAME_S *frame)
{
    int ret = -1;
    ADAPT_S *pAdapt;
    AV_INPUT_FORMAT_S  *fmt;

    pAdapt = (ADAPT_S*)handle;
    fmt = (AV_INPUT_FORMAT_S *)pAdapt->fmt;
    if ( fmt->reader_freeframe )
    {
        ret = fmt->reader_freeframe(frame);
    }
    return ret;

}

int reader_seekframe(void *handle, int idx, long long ts, int flag)
{
    int ret = -1;
    ADAPT_S *pAdapt;
    AV_INPUT_FORMAT_S  *fmt;

    pAdapt = (ADAPT_S*)handle;
    fmt = (AV_INPUT_FORMAT_S *)pAdapt->fmt;
    if ( fmt->reader_seekframe ) 
    {
        ret = fmt->reader_seekframe(pAdapt->priv_data, idx, ts, flag);
    }
    return ret;

}
int reader_parsenal(void *handle, char *pAddr, int len, struct iovec *pVector, int *num)
{
    int ret = -1;
    ADAPT_S *pAdapt;
    AV_INPUT_FORMAT_S  *fmt;

    pAdapt = (ADAPT_S*)handle;
    fmt = (AV_INPUT_FORMAT_S *)pAdapt->fmt;
    if ( fmt->reader_parsenal )
    {
        ret = fmt->reader_parsenal(pAddr, len, pVector, num);
    }
    return ret;
}




