/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : avi_adapt.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2007/06/23
  Description   : avi_adapt.c header file
  History       :
  1.Date        : 2011/02/21
    Author      : l59217
    Modification: Created file

******************************************************************************/

#ifndef __AVI_ADAPT_H__
#define __AVI_ADAPT_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

int reader_destroy_avi(void *handle);
int reader_freeframe_avi(AV_FRAME_S *frame);
int reader_readfileinfo_avi(void *handle, AV_FILE_INFO_S *fileinfo);
int reader_readframe_avi(void *handle, AV_FRAME_S *frame);
int reader_seekframe_avi(void *handle, int idx, long long ts, int flag);
int reader_parsenal_avi(char *pData, int nLen, struct iovec *pVector, int *pNalNum);  //??

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __AVI_ADAPT_H__ */

