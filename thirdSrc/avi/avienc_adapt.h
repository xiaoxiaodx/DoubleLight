/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : Avienc_adapt.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 
  Description   : Avienc_adapt.c header file
  History       :
  1.Date        : 
    Author      : 
    Modification: Created file

******************************************************************************/

#ifndef __AVIENC_ADAPT_H__
#define __AVIENC_ADAPT_H__


#ifdef __cplusplus
#if __cplusplus

#include "avformat.h"
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

int writer_create_avi(void **handle, char *filename);
int writer_destroy_avi(void *handle);
int writer_setvideoparam_avi(void *handle, int *idx, VIDEO_PARAM_S *param);
int writer_setaudioparam_avi(void *handle, int *idx, AUDIO_PARAM_S *param);
int writer_begin_avi(void *handle);
int writer_end_avi(void *handle);
int writer_writeframe_avi(void *handle, int idx, long long ts, unsigned char *pAddr, int len, int keyflag);
int writer_avi_init();


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __AVIENC_ADAPT_H__ */

