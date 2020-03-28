#ifndef __COMMON_H__
#define _COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <sys/wait.h>
//#include <sys/uio.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
//#include <sys/ioctl.h>
#include <sys/time.h>

#include <pthread.h>
//#include <sys/mman.h>
#include <stdlib.h>
//#include <asm/io.h>
//#include <linux/ioctl.h>

//#include "hi_common_api.h"


#define _ASSERT(ret_want, ret)      \
if(ret_want != ret)                 \
{                                   \
    printf("[ASSERT]%s(%s: %d) error=0x%08x\n", __FUNCTION__, __FILE__, __LINE__, ret);\
    return ret;             \
}

#if(defined DEBUG_ON )
#define DEBUG_POS \
    printf("%s(%s: %d)\n", __FUNCTION__, __FILE__, __LINE__);

#define DEBUG_POS_DELAY \
    printf("%s(%s: %d)\n", __FUNCTION__, __FILE__, __LINE__); sleep(1);

#define error(args...)   \
{   \
    printf("[error]%s(%s: %d) : ", __FUNCTION__, __FILE__, __LINE__);  \
    printf(args);  \
}

#define warn(args...)   \
{   \
    printf("[warn]%s(%s: %d) : ", __FUNCTION__, __FILE__, __LINE__);  \
    printf(args);    \
}

#define info(args...)   \
{   \
    printf("[info]%s(%s: %d) : ", __FUNCTION__, __FILE__, __LINE__);  \
    printf(args);    \
}
#else
#define DEBUG_POS 

#define DEBUG_POS_DELAY 

#define error(args...)   \
{   \
    printf("[error] (%s: %d) : ", __FUNCTION__,  __LINE__);  \
    printf(args);  \
}

#define warn(args...)   \
{   \
    printf("[warn] (%s: %d) : ", __FUNCTION__, __LINE__);  \
    printf(args);    \
}
#define info(args...)

#endif

#endif

