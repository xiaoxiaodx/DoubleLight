#ifndef __DEBUG_APP_H__
#define __DEBUG_APP_H__

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include "hi_type.h"


#define MAX_LENGTH_OF_FUNC	32//13	//函数名长度，可作修改
#define MAX_LENGTH_OF_LEVEL	16	//level信息打印长度，一般不作修改
//#define MAX_LENGTH_OF_DATA	24	//日期长度，一般不作修改
//#define MAX_LENGTH_OF_FUNC_INFO	(MAX_LENGTH_OF_FUNC+MAX_LENGTH_OF_LEVEL+16)
#define MAX_CHAR_NUM_IN_LINE	1024

/* printf colour */
#define NONE_C       "\033[m"
#define RED          "\033[0;32;31m"
#define LIGHT_RED    "\033[1;31m"
#define GREEN        "\033[0;32;32m"
#define LIGHT_GREEN  "\033[1;32m"
#define BLUE         "\033[0;32;34m"
#define LIGHT_BLUE   "\033[1;34m"
#define DARY_GRAY    "\033[1;30m"
#define CYAN         "\033[0;36m"
#define LIGHT_CYAN   "\033[1;36m"
#define PURPLE       "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN        "\033[0;33m"
#define YELLOW       "\033[1;33m"
#define LIGHT_GRAY   "\033[0;37m"
#define WHITE        "\033[1;37m"

/*todo,后续需把下面两个重的调试定义级别整合 */
/*support DBG types*/
#define ERR			        2	//打印并写到log文件，红色字体，用于打印异常信息
#define PRI			        1	//打印并写到log文件，绿色字体，用于打印某些重要信息
#define DBG			        5	//只打印不写到log文件
#define WRN			        3	//打印并写到log文件，黄色字体，用于打印一些非致命的信息
#define MAIN_DBG	        3	//各个模块的调试信息
#define AV_DBG		        3	
#define NET_DBG		        3

/*调试信息 等级*/
#define DBG_FATAL           (0)   /* system is unusable, used for printing fatal error with red color */
#define DBG_CRIT            (1)   /* critical conditions，used for printing some important information with blue color */
#define DBG_ERR             (2)   /* error conditions, used for printing normal error, but the system can still run with light red color */
#define DBG_WARN            (3)   /* warning conditions, used for printing warning informatins with yellow color */
#define DBG_INFO            (4)   /* informational, used for printing informations with green color */
#define DBG_DEBUG           (5)   /* debug-level messages, used for printing debug messages with white color */

#define MAX_DEBUG_LEVEL    (6)   /* max debug level count */


/*模块调试开关:  0,关闭调试信息;
               非0,打开指定等级以上(小于指定等级)的调试信息*/

#if(defined DEBUG_ON )
#define DEBUG_MAIN              3       /*主函数*/
#define DEBUG_DEVS              3       /*设备搜索*/
#define DEBUG_DEVM              3       /*设备管理*/
#define DEBUG_MCTP              0       /*媒体控制传输*/
#define DEBUG_MBUF              0       /*mbuf*/
#define DEBUG_TRANS             0       /*私有协议-传输模块*/
#define DEBUG_OPRTM             0       /*私有协议-业务管理*/
#define DEBUG_INIT              0       /*初始化模块*/
#define DEBUG_OUT               0       /*输出模块*/
#define DEBUG_MD                0       /*MD模块*/
#define DEBUG_OD                3	     /*OD模块*/
#define DEBUG_OSD               3       /*OSD模块*/
#define DEBUG_SNAP              3       /*抓拍模块*/
#define DEBUG_REC               0       /*录像模块*/
#define DEBUG_AUTH              0       /*权限模块*/
#define DEBUG_PTZ               0       /*云台控制模块*/
#define DEBUG_ENC               3       /*音视频模块*/
#define DEBUG_AI                3      /*AI模块*/
#define DEBUG_VI                3       /*VI模块*/
#define DEBUG_AO                3       /*AI模块*/
#define DEBUG_VO                3       /*VO模块*/
#define DEBUG_VENC              3       /*视频编码模块*/
#define DEBUG_VDEC              3       /*视频解码模块*/
#define DEBUG_AENC              3       /*音频编码模块*/
#define DEBUG_ADEC              3       /*音频解码模块*/
#define DEBUG_VPSS              3       /*视频处理模块*/
#define DEBUG_MEDIA             3       /*媒体处理模块*/
#define DEBUG_COVER             0       /*视频遮挡模块*/
#define DEBUG_CMD               0       /*命令行模块*/
#define DEBUG_TCMD              0       /*telnet命令行UI模块*/
#define DEBUG_SYS               3       /*系统信息模块*/
#define DEBUG_USBDEV            0       /**/
#define DEBUG_MTRANS            3       /*点播业务模块*/
#define DEBUG_VOD               3       /*点播业务模块*/
#define DEBUG_HTTP_LIVE         3       /*点播业务模块*/
#define DEBUG_RTSP_LIVE         3      /*点播业务模块*/
#define DEBUG_TALK              0       /*对讲业务模块*/
#define DEBUG_MAIL              0       /*邮件发送模块*/
#define DEBUG_NTP               0       /*ntp模块*/
#define DEBUG_CGI               3       /*CGI模块*/
#define DEBUG_EVENT             0       /*事件管理*/
#define DEBUG_ALARM             0       /*告警管理*/
#define DEBUG_CONFACC           0       /*配置存取*/
#define DEBUG_NOTIFY            0       /*NOTIFY*/
#define DEBUG_TIMER             3       /*定时管理*/
#define DEBUG_LOG               3       /*日志*/
#define DEBUG_ISP               0       /*ISP*/
#define DEBUG_HAL               3       /*HAL*/
#define DEBUG_AUDIO             3       /*AUDIO*/
#define DEBUG_SCHE              3      /*日程管理*/
#define DEBUG_HTTP              3      /*HTTPD*/
#define DEBUG_UPGRADE           3       /*UPGRADE*/
#define DEBUG_VODTRANS_LIVE_LIVE 3
#define DEBUG_RTSP_O_HTTP_LIVE   3
#define DEBUG_VSCP               0
#define DEBUG_SLOG               4

#else
#define DEBUG_MAIN              0       /*主函数*/
#define DEBUG_DEVS              0       /*设备搜索*/
#define DEBUG_DEVM              0       /*设备管理*/
#define DEBUG_MCTP              0       /*媒体控制传输*/
#define DEBUG_MBUF              0       /*mbuf*/
#define DEBUG_TRANS             0       /*私有协议-传输模块*/
#define DEBUG_OPRTM             0       /*私有协议-业务管理*/
#define DEBUG_INIT              0       /*初始化模块*/
#define DEBUG_OUT               0       /*输出模块*/
#define DEBUG_MD                0       /*MD模块*/
#define DEBUG_OD                0	     /*OD模块*/
#define DEBUG_OSD               0       /*OSD模块*/
#define DEBUG_SNAP              0       /*抓拍模块*/
#define DEBUG_REC               0       /*录像模块*/
#define DEBUG_AUTH              0       /*权限模块*/
#define DEBUG_PTZ               0       /*云台控制模块*/
#define DEBUG_ENC               0       /*音视频模块*/
#define DEBUG_AI                0      /*AI模块*/
#define DEBUG_VI                0       /*VI模块*/
#define DEBUG_AO                0       /*AI模块*/
#define DEBUG_VO                0       /*VO模块*/
#define DEBUG_VENC              0       /*视频编码模块*/
#define DEBUG_VDEC              0       /*视频解码模块*/
#define DEBUG_AENC              0       /*音频编码模块*/
#define DEBUG_ADEC              0       /*音频解码模块*/
#define DEBUG_VPSS              0       /*视频处理模块*/
#define DEBUG_MEDIA             0       /*媒体处理模块*/
#define DEBUG_COVER             0       /*视频遮挡模块*/
#define DEBUG_CMD               0       /*命令行模块*/
#define DEBUG_TCMD              0       /*telnet命令行UI模块*/
#define DEBUG_SYS               0       /*系统信息模块*/
#define DEBUG_USBDEV            0       /**/
#define DEBUG_MTRANS            0       /*点播业务模块*/
#define DEBUG_VOD               0       /*点播业务模块*/
#define DEBUG_HTTP_LIVE         0       /*点播业务模块*/
#define DEBUG_RTSP_LIVE         0      /*点播业务模块*/
#define DEBUG_TALK              0       /*对讲业务模块*/
#define DEBUG_MAIL              0       /*邮件发送模块*/
#define DEBUG_NTP               0       /*ntp模块*/
#define DEBUG_CGI               0       /*CGI模块*/
#define DEBUG_EVENT             0       /*事件管理*/
#define DEBUG_ALARM             0       /*告警管理*/
#define DEBUG_CONFACC           0       /*配置存取*/
#define DEBUG_NOTIFY            0       /*NOTIFY*/
#define DEBUG_TIMER             0       /*定时管理*/
#define DEBUG_LOG               0       /*日志*/
#define DEBUG_ISP               0       /*ISP*/
#define DEBUG_HAL               0       /*HAL*/
#define DEBUG_AUDIO             0       /*AUDIO*/
#define DEBUG_SCHE              0      /*日程管理*/
#define DEBUG_HTTP              0      /*HTTPD*/
#define DEBUG_UPGRADE           0       /*UPGRADE*/
#define DEBUG_VODTRANS_LIVE_LIVE 0
#define DEBUG_RTSP_O_HTTP_LIVE   0
#define DEBUG_VSCP               0
#define DEBUG_SLOG               2
#endif  /* DEBUG_ON */

HI_S32 DEBUG_FLPrintf(const HI_CHAR* pszModeName, HI_U32 u32Level,
                                const HI_CHAR* pszFileName, HI_S32 s32Line, HI_CHAR* pszFmt, ...);

/*日志,为与linux系统的日志宏定义区别开来，本处log调测宏特别加上PRINT后缀以区分 */
#if (DEBUG_LOG == 0) 
    #define LOG_DEBUG_PRINT(u32Level, args ...)
#else
    #define LOG_DEBUG_PRINT(u32Level, args ...)   \
        do  \
        {   \
            if (DEBUG_LOG >= u32Level)  \
            {   \
                DEBUG_FLPrintf("LOG", u32Level, __FILE__, __LINE__, args);   \
            }   \
        }   \
        while(0)
#endif

/*抓拍功能*/
#if (DEBUG_SNAP == 0)
    #define SNAP_DEBUG(u32Level, args ...)
#else
    #define SNAP_DEBUG(u32Level, args ...)   \
        do  \
        {   \
            if (DEBUG_SNAP >= u32Level)  \
            {   \
                DEBUG_FLPrintf("SNAP", u32Level, __FILE__, __LINE__, args);   \
            }   \
        }   \
        while(0)
#endif

/*定时管理*/
#if (DEBUG_TIMER == 0)
    #define TIMER_DEBUG(u32Level, args ...)
#else
    #define TIMER_DEBUG(u32Level, args ...)   \
        do  \
        {   \
            if (DEBUG_TIMER >= u32Level)  \
            {   \
                DEBUG_FLPrintf("TIMER", u32Level, __FILE__, __LINE__, args);   \
            }   \
        }   \
        while(0)
#endif

/*主函数*/
#if (DEBUG_MAIN == 0) 
    #define MAIN_DEBUG(u32Level, args ...)
#else
    #define MAIN_DEBUG(u32Level, args ...)   \
        do  \
        {   \
            if (DEBUG_MAIN >= u32Level)  \
            {   \
                DEBUG_FLPrintf("MAIN", u32Level, __FILE__, __LINE__, args);   \
            }   \
        }   \
        while(0)
#endif

/*notify*/
#if (DEBUG_NOTIFY == 0) 
    #define NOTIFY_DEBUG(u32Level, args ...)
#else
    #define NOTIFY_DEBUG(u32Level, args ...)   \
        do  \
        {   \
            if (DEBUG_NOTIFY >= u32Level)  \
            {   \
                DEBUG_FLPrintf("NOTIFY", u32Level, __FILE__, __LINE__, args);   \
            }   \
        }   \
        while(0)
#endif

/*设备搜索*/
#if (DEBUG_DEVS == 0) 
    #define DEVS_DEBUG(u32Level, args ...)
#else
    #define DEVS_DEBUG(u32Level, args ...)   \
    do  \
    {   \
        if (DEBUG_DEVS >= u32Level)  \
        {   \
            DEBUG_FLPrintf("DEV_SEARCH", u32Level, __FILE__, __LINE__, args);    \
        }   \
    }   \
    while(0)
#endif

/*配置存取*/
#if (DEBUG_CONFACC == 0) 
    #define CONFACC_DEBUG(u32Level, args ...)
#else
    #define CONFACC_DEBUG(u32Level, args ...)   \
    do  \
    {   \
        if (DEBUG_CONFACC >= u32Level)  \
        {   \
            DEBUG_FLPrintf("CONFACC", u32Level, __FILE__, __LINE__, args);    \
        }   \
    }   \
    while(0)
#endif

/*设备管理*/
#if (DEBUG_DEVM == 0) 
    #define DEVM_DEBUG(u32Level, args ...)
#else
    #define DEVM_DEBUG(u32Level, args ...)   \
    do  \
    {   \
        if (DEBUG_DEVM >= u32Level)  \
        {   \
            DEBUG_FLPrintf("DEVMNG", u32Level, __FILE__, __LINE__,args);    \
        }   \
    }   \
    while(0)
#endif

/**/
#if (DEBUG_USBDEV == 0) 
    #define USBDEV_DEBUG(u32Level, args ...)
#else
    #define USBDEV_DEBUG(u32Level, args ...)   \
    do  \
    {   \
        if (DEBUG_USBDEV >= u32Level)  \
        {   \
            DEBUG_FLPrintf("USBDEV", u32Level, __FILE__, __LINE__,args);    \
        }   \
    }   \
    while(0)
#endif

/*媒体控制传输*/
#if (DEBUG_MCTP == 0) 
    #define MCTP_DEBUG(u32Level, args ...)
#else
    #define MCTP_DEBUG(u32Level, args ...)   \
    do  \
    {   \
        if (DEBUG_MCTP >= u32Level)  \
        {   \
            DEBUG_FLPrintf("MCTP", u32Level, __FILE__, __LINE__, args);    \
        }   \
    }   \
    while(0)
#endif

/*Mbuf*/
#if (DEBUG_MBUF == 0) 
    #define MBUF_DEBUG(u32Level, args ...)
#else
    #define MBUF_DEBUG(u32Level, args ...)  \
    do  \
    {   \
        if (DEBUG_MBUF >= u32Level)  \
        {   \
            DEBUG_FLPrintf("MBUF", u32Level, __FILE__, __LINE__, args);    \
        }   \
    }   \
    while(0)
#endif

/*MTRANS*/
#if (DEBUG_MTRANS == 0) 
    #define MTRANS_DEBUG(u32Level, args ...)
#else
    #define MTRANS_DEBUG(u32Level, args ...)   \
    do  \
    {   \
        if (DEBUG_MTRANS >= u32Level)    \
        {   \
            DEBUG_FLPrintf("MTRANS", u32Level, __FILE__, __LINE__, args);   \
        }   \
    }   \
    while(0)
#endif

/*VOD*/
#if (DEBUG_VOD == 0) 
    #define VOD_DEBUG(u32Level, args ...)
#else
    #define VOD_DEBUG(u32Level, args ...)   \
    do  \
    {   \
        if (DEBUG_VOD >= u32Level)    \
        {   \
            DEBUG_FLPrintf("VOD", u32Level, __FILE__, __LINE__, args);   \
        }   \
    }   \
    while(0)
#endif

/*http for live*/
#if (DEBUG_HTTP_LIVE == 0) 
    #define HTTP_LIVE_DEBUG(u32Level, args ...)
#else
    #define HTTP_LIVE_DEBUG(u32Level, args ...)   \
    do  \
    {   \
        if (DEBUG_HTTP_LIVE >= u32Level)    \
        {   \
            DEBUG_FLPrintf("HTTP_LIVE", u32Level, __FILE__, __LINE__, args);   \
        }   \
    }   \
    while(0)
#endif

/*VODTRANS for live*/
#if (DEBUG_VODTRANS_LIVE_LIVE == 0) 
    #define VODTRANS_LIVE_DEBUG(u32Level, args ...)
#else
    #define VODTRANS_LIVE_DEBUG(u32Level, args ...)   \
    do  \
    {   \
        if (DEBUG_VODTRANS_LIVE_LIVE >= u32Level)    \
        {   \
            DEBUG_FLPrintf("VODTRANS", u32Level, __FILE__, __LINE__, args);   \
        }   \
    }   \
    while(0)
#endif


/*rtsp over http for live*/
#if (DEBUG_RTSP_O_HTTP_LIVE == 0) 
    #define RTSP_O_HTTP_LIVE_DEBUG(u32Level, args ...)
#else
#define RTSP_O_HTTP_LIVE_DEBUG(u32Level, args ...)   \
    do  \
    {   \
        if (DEBUG_RTSP_O_HTTP_LIVE >= u32Level)    \
        {   \
            printf(args);   \
        }   \
    }   \
    while(0)
#endif

/*rtsp for live*/
#if (DEBUG_RTSP_LIVE == 0) 
    #define RTSP_DEBUG(u32Level, args ...)
#else
    #define RTSP_DEBUG(u32Level, args ...)   \
    do  \
    {   \
        if (DEBUG_RTSP_LIVE >= u32Level)    \
        {   \
            DEBUG_FLPrintf("RTSP", u32Level, __FILE__, __LINE__, args);   \
        }   \
    }   \
    while(0)
#endif

/*TALK*/
#if (DEBUG_TALK == 0) 
    #define TALK_DEBUG(u32Level, args ...)
#else
    #define TALK_DEBUG(u32Level, args ...)   \
    do  \
    {   \
        if (DEBUG_TALK >= u32Level)    \
        {   \
            DEBUG_FLPrintf("TALK", u32Level, __FILE__, __LINE__, args);   \
        }   \
    }   \
    while(0)
#endif

/*MD模块*/
#if (DEBUG_MD == 0) 
    #define MD_DEBUG(u32Level, args ...)
#else
    #define MD_DEBUG(u32Level, args ...)  \
    do  \
    {   \
        if (DEBUG_MD >= u32Level)  \
        {   \
            DEBUG_FLPrintf("MD", u32Level, __FILE__, __LINE__, args);    \
        }   \
    }   \
    while(0)
#endif

/*OD模块*/
#if (DEBUG_OD == 0) 
    #define OD_DEBUG(u32Level, args ...)
#else
    #define OD_DEBUG(u32Level, args ...)  \
     do  \
     {   \
         if (DEBUG_OD >= u32Level)  \
         {   \
             DEBUG_FLPrintf("OD", u32Level, __FILE__, __LINE__, args);    \
         }   \
     }   \
     while(0)
#endif

/*OSD模块*/
#if (DEBUG_OSD == 0) 
    #define OSD_DEBUG(u32Level, args ...)
#else
    #define OSD_DEBUG(u32Level, args ...) \
    do  \
    {   \
        if (DEBUG_OSD >= u32Level)  \
        {   \
            DEBUG_FLPrintf("OSD", u32Level, __FILE__, __LINE__, args);    \
        }   \
    }   \
    while(0)
#endif

/*录像模块*/
#if (DEBUG_REC == 0) 
    #define REC_DEBUG(u32Level, args ...)
#else
    #define REC_DEBUG(u32Level, args ...) \
    do  \
    {   \
        if (DEBUG_REC >= u32Level)  \
        {   \
            DEBUG_FLPrintf("REC", u32Level, __FILE__, __LINE__, args);    \
        }   \
    }   \
    while(0)
#endif

/*权限管理模块*/
#if (DEBUG_AUTH == 0) 
    #define AUTH_DEBUG(u32Level, args ...)
#else
    #define AUTH_DEBUG(u32Level, args ...) \
    do  \
    {   \
        if (DEBUG_AUTH >= u32Level)  \
        {   \
            DEBUG_FLPrintf("AUTH", u32Level, __FILE__, __LINE__, args);    \
        }   \
    }   \
    while(0)
#endif

/*云台控制模块*/
#if (DEBUG_PTZ == 0) 
    #define PTZ_DEBUG(u32Level, args ...)
#else
    #define PTZ_DEBUG(u32Level, args ...)    \
    do  \
    {   \
        if (DEBUG_PTZ >= u32Level)  \
        {   \
            DEBUG_FLPrintf("PTZ", u32Level, __FILE__, __LINE__, args);    \
        }   \
    }   \
    while(0)
#endif

/*AI模块*/
#if (DEBUG_AI == 0) 
    #define AI_DEBUG(u32Level, args ...)
#else
    #define AI_DEBUG(u32Level, args ...)    \
        do  \
        {   \
            if (DEBUG_AI >= u32Level)  \
            {   \
                DEBUG_FLPrintf("AI", u32Level, __FILE__, __LINE__, args);    \
            }   \
        }   \
        while(0)
#endif

/*AO模块*/
#if (DEBUG_AO == 0) 
    #define AO_DEBUG(u32Level, args ...)
#else
    #define AO_DEBUG(u32Level, args ...)    \
        do  \
        {   \
            if (DEBUG_AO >= u32Level)  \
            {   \
                DEBUG_FLPrintf("AO", u32Level, __FILE__, __LINE__, args);    \
            }   \
        }   \
        while(0)
#endif

/*AENC模块*/
#if (DEBUG_AENC == 0) 
    #define AENC_DEBUG(u32Level, args ...)
#else
    #define AENC_DEBUG(u32Level, args ...)    \
        do  \
        {   \
            if (DEBUG_AENC >= u32Level)  \
            {   \
                DEBUG_FLPrintf("AENC", u32Level, __FILE__, __LINE__, args);    \
            }   \
        }   \
        while(0)
#endif

/*ADEC模块*/
#if (DEBUG_ADEC == 0) 
    #define ADEC_DEBUG(u32Level, args ...)
#else
    #define ADEC_DEBUG(u32Level, args ...)    \
        do  \
        {   \
            if (DEBUG_ADEC >= u32Level)  \
            {   \
                DEBUG_FLPrintf("ADEC", u32Level, __FILE__, __LINE__, args);    \
            }   \
        }   \
        while(0)
#endif

/*VI模块*/
#if (DEBUG_VI == 0) 
    #define VI_DEBUG(u32Level, args ...)
#else
    #define VI_DEBUG(u32Level, args ...)    \
        do  \
        {   \
            if (DEBUG_VI >= u32Level)  \
            {   \
                DEBUG_FLPrintf("VI", u32Level, __FILE__, __LINE__, args);    \
            }   \
        }   \
        while(0)
#endif

/*VO模块*/
#if (DEBUG_VO == 0) 
    #define VO_DEBUG(u32Level, args ...)
#else
    #define VO_DEBUG(u32Level, args ...)    \
        do  \
        {   \
            if (DEBUG_VO >= u32Level)  \
            {   \
                DEBUG_FLPrintf("VO", u32Level, __FILE__, __LINE__, args);    \
            }   \
        }   \
        while(0)
#endif

/*VPSS模块*/
#if (DEBUG_VPSS == 0) 
    #define VPSS_DEBUG(u32Level, args ...)
#else
    #define VPSS_DEBUG(u32Level, args ...)    \
        do  \
        {   \
            if (DEBUG_VPSS >= u32Level)  \
            {   \
                DEBUG_FLPrintf("VPSS", u32Level, __FILE__, __LINE__, args);    \
            }   \
        }   \
        while(0)
#endif

/*VENC模块*/
#if (DEBUG_VENC == 0) 
    #define VENC_DEBUG(u32Level, args ...)
#else
    #define VENC_DEBUG(u32Level, args ...)    \
        do  \
        {   \
            if (DEBUG_VENC >= u32Level)  \
            {   \
                DEBUG_FLPrintf("VENC", u32Level, __FILE__, __LINE__, args);    \
            }   \
        }   \
        while(0)
#endif

/*VDEC模块*/
#if (DEBUG_VDEC == 0) 
    #define VDEC_DEBUG(u32Level, args ...)
#else
    #define VDEC_DEBUG(u32Level, args ...)    \
        do  \
        {   \
            if (DEBUG_VDEC >= u32Level)  \
            {   \
                DEBUG_FLPrintf("VDEC", u32Level, __FILE__, __LINE__, args);    \
            }   \
        }   \
        while(0)
#endif


/*MEDIA模块*/
#if (DEBUG_MEDIA == 0) 
    #define MEDIA_DEBUG(u32Level, args ...)
#else
    #define MEDIA_DEBUG(u32Level, args ...)    \
        do  \
        {   \
            if (DEBUG_MEDIA >= u32Level)  \
            {   \
                DEBUG_FLPrintf("MEDIA", u32Level, __FILE__, __LINE__, args);    \
            }   \
        }   \
        while(0)
#endif

/*视频遮挡模块*/
#if (DEBUG_COVER == 0) 
    #define COVER_DEBUG(u32Level, args ...)
#else
    #define COVER_DEBUG(u32Level, args ...)    \
        do  \
        {   \
            if (DEBUG_COVER >= u32Level)  \
            {   \
                DEBUG_FLPrintf("COVER", u32Level, __FILE__, __LINE__, args);    \
            }   \
        }   \
        while(0)
#endif

/*命令行模块*/
#if (DEBUG_CMD == 0) 
    #define CMD_DEBUG(u32Level, args ...)
#else
    #define CMD_DEBUG(u32Level, args ...)    \
    do  \
    {   \
        if (DEBUG_CMD >= u32Level)  \
        {   \
            DEBUG_FLPrintf("CMD", u32Level, __FILE__, __LINE__, args);    \
        }   \
    }   \
    while(0)
#endif

/*telnet命令行UI模块*/
#if (DEBUG_TCMD == 0) 
    #define TCMD_DEBUG(u32Level, args ...)
#else
    #define TCMD_DEBUG(u32Level, args ...)   \
    do  \
    {   \
        if (DEBUG_TCMD >= u32Level)  \
        {   \
            DEBUG_FLPrintf("TCMD", u32Level, __FILE__, __LINE__, args);    \
        }   \
    }   \
    while(0)
#endif

/*系统信息模块*/
#if (DEBUG_SYS == 0) 
    #define SYS_DEBUG(u32Level, args ...)
#else
    #define SYS_DEBUG(u32Level, args ...)    \
    do  \
    {   \
        if (DEBUG_SYS >= u32Level)  \
        {   \
            DEBUG_FLPrintf("SYS", u32Level, __FILE__, __LINE__,args);    \
        }   \
    }   \
    while(0)
#endif

/*事件管理*/
#if (DEBUG_EVENT == 0) 
    #define EVENT_DEBUG(u32Level, args ...)
#else
    #define EVENT_DEBUG(u32Level, args ...)     \
    do  \
    {   \
        if (DEBUG_EVENT >= u32Level)  \
        {   \
            DEBUG_FLPrintf("EVENT", u32Level, __FILE__, __LINE__, args);    \
        }   \
    }   \
    while(0)
#endif

/*告警管理*/
#if (DEBUG_ALARM == 0) 
    #define ALARM_DEBUG(u32Level, args ...)
#else
    #define ALARM_DEBUG(u32Level, args ...)     \
    do  \
    {   \
        if (DEBUG_ALARM >= u32Level)  \
        {   \
            DEBUG_FLPrintf("ALARM", u32Level, __FILE__, __LINE__, args);    \
        }   \
    }   \
    while(0)
#endif


/*邮件发送*/
#if (DEBUG_MAIL == 0) 
    #define MAIL_DEBUG(u32Level, args ...)
#else
    #define MAIL_DEBUG(u32Level, args ...)     \
    do  \
    {   \
        if (DEBUG_MAIL >= u32Level)  \
        {   \
            DEBUG_FLPrintf("MAIL", u32Level, __FILE__, __LINE__, args);    \
        }   \
    }   \
    while(0)
#endif

/*NTP*/
#if (DEBUG_NTP == 0) 
    #define NTP_DEBUG(u32Level, args ...)
#else
    #define NTP_DEBUG(u32Level, args ...)     \
    do  \
    {   \
        if (DEBUG_NTP >= u32Level)  \
        {   \
            DEBUG_FLPrintf("DEBUG", u32Level, __FILE__, __LINE__, args);    \
        }   \
    }   \
    while(0)
#endif

/*CGI*/
#if ((DEBUG_CGI == 0) )
    #define CGI_DEBUG(u32Level, args ...)
#else
    #define CGI_DEBUG(u32Level, args ...)   \
    do  \
    {   \
        if (DEBUG_CGI >= u32Level)  \
        {   \
            DEBUG_FLPrintf("CGI", u32Level, __FILE__, __LINE__, args);    \
        }   \
    }   \
    while(0)
#endif

/*ISP模块*/
#if (DEBUG_ISP == 0) 
    #define ISP_DEBUG(u32Level, args ...)
#else
    #define ISP_DEBUG(u32Level, args ...)    \
        do  \
        {   \
            if (DEBUG_ISP >= u32Level)  \
            {   \
                DEBUG_FLPrintf("ISP", u32Level, __FILE__, __LINE__, args);    \
            }   \
        }   \
        while(0)
#endif

/*HAL模块*/
#if (DEBUG_HAL == 0) 
    #define HAL_DEBUG(u32Level, args ...)
#else
    #define HAL_DEBUG(u32Level, args ...)    \
            do  \
            {   \
                if (DEBUG_HAL >= u32Level)  \
                {   \
                    DEBUG_FLPrintf("HAL", u32Level, __FILE__, __LINE__, args);    \
                }   \
            }   \
            while(0)
#endif

/*AUDIO模块*/
#if (DEBUG_AUDIO == 0) 
    #define AUDIO_DEBUG(u32Level, args ...)
#else
    #define AUDIO_DEBUG(u32Level, args ...)    \
            do  \
            {   \
                if (DEBUG_AUDIO >= u32Level)  \
                {   \
                    DEBUG_FLPrintf("AUDIO", u32Level, __FILE__, __LINE__, args);    \
                }   \
            }   \
            while(0)
#endif

/*SCHEDULE模块*/
#if (DEBUG_SCHE == 0) 
    #define SCHE_DEBUG(u32Level, args ...)
#else
    #define SCHE_DEBUG(u32Level, args ...)    \
            do  \
            {   \
                if (DEBUG_SCHE >= u32Level)  \
                {   \
                    DEBUG_FLPrintf("SCHE", u32Level, __FILE__, __LINE__, args);    \
                }   \
            }   \
            while(0)
#endif

/*HTTPD模块*/
#if (DEBUG_HTTP == 0) 
    #define HTTP_DEBUG(u32Level, args ...)
#else
    #define HTTP_DEBUG(u32Level, args ...)    \
            do  \
            {   \
                if (DEBUG_HTTP >= u32Level)  \
                {   \
                    DEBUG_FLPrintf("HTTPD", u32Level, __FILE__, __LINE__, args);    \
                }   \
            }   \
            while(0)
#endif

/*upgrade模块*/
#if (DEBUG_UPGRADE== 0) 
    #define UPGRADE_DEBUG(u32Level, args ...)
#else
    #define UPGRADE_DEBUG(u32Level, args ...)    \
            do  \
            {   \
                if (DEBUG_UPGRADE >= u32Level)  \
                {   \
                    DEBUG_FLPrintf("UPGRADE", u32Level, __FILE__, __LINE__, args);    \
                }   \
            }   \
            while(0)
#endif

/*设备搜索模块*/
#if (DEBUG_VSCP == 0) 
    #define VSCP_DEBUG(u32Level, args ...)
#else
    #define VSCP_DEBUG(u32Level, args ...)    \
            do  \
            {   \
                if (DEBUG_HTTP >= u32Level)  \
                {   \
                    DEBUG_FLPrintf("HTTPD", u32Level, __FILE__, __LINE__, args);    \
                }   \
            }   \
            while(0)
#endif



/*SLOG 接口*/
#if (DEBUG_SLOG == 0) 
#define SLOG(level, fmt...)	
#else
#define SLOG(level, fmt...)	\
            do  \
            {   \
                if (DEBUG_SLOG >= level)  \
                {   \
                    DEBUG_FLPrintf(NULL, level, __FUNCTION__, __LINE__, fmt);\
                }   \
            }   \
            while(0)
#endif

//#define PLOG(level, fmt...)	\
//	do {	\
//		DEBUG_FLPrintf(NULL, level, __FUNCTION__, __LINE__, fmt);\
//	}while(0)

#ifndef CHECK_ARG
#define CHECK_ARG(args) do{ \
	    if(args == NULL){printf("%serr: invalid parames.\n%s", LIGHT_RED, NONE_C);\
		return HI_FAILURE;} \
}while(0)
#endif


#endif //end of __DEBUG_APP_H__
