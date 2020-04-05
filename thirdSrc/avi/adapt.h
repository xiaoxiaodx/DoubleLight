/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : adapt.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2007/08/02
  Description   : adapt.c header file
  History       :
  1.Date        : 2007/08/02
    Author      : qushen
    Modification: Created file

******************************************************************************/

#ifndef __ADAPT_H__
#define __ADAPT_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include "common.h"

typedef struct hiADAPT_S
{
    void *fmt;
    void *priv_data;        
} ADAPT_S;


/*****************************************************************************
 Prototype       : adapt_register_format
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

#define adapt_register_format( first, type, format ) \
do \
{   \
    type **__p;   \
    __p = &(first); \
    while (*__p != NULL) __p = &(*__p)->next; \
    *__p = (format);    \
    (format)->next = NULL;    \
}while(0)


/*****************************************************************************
 Prototype       : adapt_find_format
 Description     : ...
 Input           : None
 Output          : None
 Return Value    : 
 Global Variable   
    Read Only    : 
    Read & Write : 
  History         
  1.Date         : 2007/8/2
    Author       : qushen
    Modification : Created function

*****************************************************************************/
#define adapt_find_format( first, stype, value, format ) \
do \
{   \
    stype *__fmt;    \
    for(__fmt = first, format = NULL; __fmt != NULL; __fmt = __fmt->next)  \
    {   \
        if ( __fmt->type == value )  \
        {   \
            info("find file type: %d\n", value);    \
            format = __fmt; \
            break;\
        }   \
    }   \
}while(0)



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifndef __ADAPT_H__ */


