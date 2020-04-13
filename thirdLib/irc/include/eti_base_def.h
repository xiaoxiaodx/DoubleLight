/*
* 版权所有：深圳市朗驰数字技术有限公司
* 作    者：HongJun Chen
* 设计日期: 2003-11-15
* $Id: eti_base_def.h,v 1.1.1.1 2006/06/29 12:41:28 administrator Exp $
*/
#ifndef __ETI_BASE_DEF_H__
#define __ETI_BASE_DEF_H__

#define ETI_MAX_CH_PERDSP       2
#define ETI_QCIF_CHANNELS       2

#define ETI_MAX_SUPPORTCH         16  //所有产品中支持的最大通道
#define APP_PASSWORD_LENGTH       20  //所有密码字符长度
#define APP_PTZ_STRING_LEN        60  //云台控制方法字符串长度
#define APP_TIMECHECK_LISTNUM     7   //报警、自动录象检测时间列表个数
#define APP_ZTE_TIMECHECK_LISTNUM 32  //ZTE 移动侦测/输入报警时间个数
#define APP_ADDRLIST_NUM          20  //
#define APP_DECLOOP_ITEMNUM       100
#define APP_PPPOE_STRING_SIZE     20
#define APP_PHONE_NUMBER_SIZE     20
#define APP_PPPOE_STRING_SIZE     20
#define APP_PHONE_NUMBER_SIZE     20

#define APP_TITALSTRING_COUNT     4
#define APP_ALARMNAMELENGTH       32

#define ETI_CHAR_816LIB_SIZE      4096    //8*16 ANSC字库大小
#define ETI_HANZI_1616LIB_SIZE    261696  //16*16 汉字库大小

#define ETI_MP4_STREAM_AV         0       //音、视复合数据流
#define ETI_MP4_STREAM_V          1       //视频数据流

#define ETI_MP4_BITCTRL_VBR       1       //视频码率定质量控制
#define ETI_MP4_BITCTRL_CBR       0       //视频码率定码率控制

#define ETI_MP4_QUANTIZER_MIN     2       //视频最小量化系数
#define ETI_MP4_QUANTIZER_MAX     31      //视频最大量化系数

#define ETI_MP4_BITRATE_MIN       32      //视频最小码率 32K
#define ETI_MP4_BITRATE_MAX       4000    //视频最大码率 4M

#define ETI_MP4_KEYINTERVAL_MIN   12      //最小关键帧间隔
#define ETI_MP4_KEYINTERVAL_MAX   200     //最大关键帧间隔

#define ETI_AUDIO_ENCTYPE_MP3   0x55    //MP3
#define ETI_AUDIO_ENCTYPE_G722  0XFE    //G.722
#define ETI_AUDIO_ENCTYPE_G711A 0x13    //G.711A
#define ETI_AUDIO_ENCTYPE_G711U 0x14    //G.711U
#define ETI_AUDIO_ENCTYPE_G726  0x15    //G.726
#define ETI_AUDIO_ENCTYPE_AAC   0x16    //AAC
#define ETI_AUDIO_ENCTYPE_AMR   0x17    //AMR
#define ETI_AUDIO_ENCTYPE_PCM   0x18    //PCM

enum{
    ETI_VIDEOTAG_MPEG4 = 0X58564944,    //DIVX
    ETI_VIDEOTAG_H264  = 0X34363248,    //(('4'<<24) | ('6'<<16) | ('2'<<8) |'H')                //H264
    ETI_VIDEOTAG_MJPEG = 0X47504A4D,    //(('G'<<24) | ('P'<<16) | ('J'<<8) |'M')                //MJPG
    ETI_VIDEOTAG_H265  = 0X35363248,     //(('5'<<24) | ('6'<<16) | ('2'<<8) |'H')                //H265
    ETI_VIDEOTAG_RAW  =  0X00574152,     //(('0'<<24) | ('W'<<16) | ('A'<<8) |'R')                //RAW
    ETI_VIDEOTAG_IR14    =  0x34315249        //('4'<< 24)| ('1' << 16) ('R'<<8) | 'I'            //IR14
};

#define ETI_AUDIO_SAMPLE_RATE_48K        48000
#define ETI_AUDIO_SAMPLE_RATE_44K        44100
#define ETI_AUDIO_SAMPLE_RATE_32K        32000
#define ETI_AUDIO_SAMPLE_RATE_24K        24000
#define ETI_AUDIO_SAMPLE_RATE_22K        22050
#define ETI_AUDIO_SAMPLE_RATE_16K        16000
#define ETI_AUDIO_SAMPLE_RATE_8K         8000

#define ETI_STREAM_STATUS_START          0X00000001    //启动码流
#define ETI_STREAM_STATUS_STOP           0X00000002    //停止码流
#define ETI_STREAM_STATUS_RECV           0X00000004    //发送码流
#define ETI_STREAM_STATUS_FTPBACK        0X00000008    //FTP备份

#define ETI_ALARM_STATUS_MOTION          0X00000001    //视频移动报警
#define ETI_ALARM_STATUS_LOSTVI          0X00000002    //视频丢失报警
#define ETI_ALARM_STATUS_LOSTAI          0X00000004    //音频丢失报警
/********************** 码流标示***************************/
typedef enum{
    ETI_STREAM_MAIN = 0,                 // 主码流标示
    ETI_STREAM_SUB1 = 1,                 // 子码流标示
    ETI_STREAM_SUB2 = 2,                 // 三码流标示
    ETI_STREAM_SUB3 = 3,                 // 四码流标示
}ENUM_EIT_STREAMID;

/********************** 断网续传标示***********************/
#define ETI_JPEG_CONTINUEUPLOADFTP    0           //ftp断网续传
#define ETI_JPEG_CONTINUEUPLOADSDK    1           //sdk断网续传
#define ETI_RVS_STREAM 100  //RVS上传码流

/********************** 缓冲大小***************************/
#define ETI_PREREC_BUFF_SIZE_CIF        0XA2800         //650*1024
#define ETI_PREREC_BUFF_SIZE_2CIF       0XE6000         //920*1024
#define ETI_PREREC_BUFF_SIZE_4CIF       0X14CCC0        //1024*1024*1.3(32的倍数)
/********************** 报警时间定义 ********************************/
#define ETI_ALARM_MIN_RECTIME            30            //最小报警持续(录象/备份)时间(s)
#define ETI_ALARM_MAX_RECTIME            3600          //最小报警持续(录象/备份)时间(s)
/********************** 视频格式定义 ********************************/

#define ETI_MP4_FORMAT_QCIF         0
#define ETI_MP4_FORMAT_CIF          1
#define ETI_MP4_FORMAT_2CIF         2
#define ETI_MP4_FORMAT_4CIF         3
#define ETI_MP4_FORMAT_DCIF         4
#define ETI_MP4_FORMAT_QVGA         5      //320*240
#define ETI_MP4_FORMAT_VGA_60HZ     6      //640*480
#define ETI_MP4_FORMAT_SVGA_60HZ    7      //800*600
#define ETI_MP4_FORMAT_XGA_60HZ     8      //1024*768
#define ETI_MP4_FORMAT_SXGA_60HZ    9      //1280*1024
#define ETI_MP4_FORMAT_UXGA_60HZ    10     //1600*1200
#define ETI_MP4_FORMAT_720P         11     //1280*720, no use
#define ETI_MP4_FORMAT_HDTV         12     //1920*1080, no use
#define ETI_MP4_FORMAT_SVGA_75HZ    13     //800*600
#define ETI_MP4_FORMAT_XGA_75HZ     14     //1024*768
#define ETI_MP4_FORMAT_720P_50HZ    15     //1280*720, 50HZ
#define ETI_MP4_FORMAT_720P_60HZ    16     //1280*720, 60HZ
#define ETI_MP4_FORMAT_1080P_50HZ   17     //1920*1080, 50HZ
#define ETI_MP4_FORMAT_1080P_60HZ   18     //1920*1080, 60HZ
#define ETI_MP4_FORMAT_LTF          19     //240*192
#define ETI_MP4_FORMAT_WQVGA1       20     //480*352
#define ETI_MP4_FORMAT_WQVGA2       21     //480*272
#define ETI_MP4_FORMAT_UVGA_50HZ    22     //1280*960,50HZ
#define ETI_MP4_FORMAT_UVGA_60HZ    23     //1280*960,60Hz
#define ETI_MP4_FORMAT_1080P_30HZ   24     //1920*1080, 30HZ
#define ETI_MP4_FORMAT_1080I_50HZ   25     //1920*1080, 50HZ
#define ETI_MP4_FORMAT_1080I_60HZ   26     //1920*1080, 60HZ
#define ETI_MP4_FORMAT_SXGA_75HZ    27     //1280*1024, 75HZ
#define ETI_MP4_FORMAT_WXGA_60HZ    28     //1280*800, 60HZ
#define ETI_MP4_FORMAT_WXGA_75HZ    29     //1280*800, 75HZ
#define ETI_MP4_FORMAT_SXGAP_60HZ   30     //1400*1050, 60HZ  SXGA+
#define ETI_MP4_FORMAT_SXGAP_75HZ   31     //1400*1050, 75HZ  SXGA+
#define ETI_MP4_FORMAT_WXGAP_60HZ   32     //1440*900, 60HZ  WXGA+
#define ETI_MP4_FORMAT_WSXGAP_60HZ  33     //1680*1050, 60HZ WSXGA+
#define ETI_MP4_FORMAT_WSUVGAP_60HZ 34     //1920*1080, 60HZ WSUVGA+
#define ETI_MP4_FORMAT_1366X768_60HZ 35    //1366*768, 60HZ 
#define ETI_MP4_FORMAT_WXGA_59HZ     36    //1280*800, 60HZ,非标
#define ETI_MP4_FORMAT_1280X720_59HZ 37    //1280*720, 60HZ 非标
#define ETI_MP4_FORMAT_1360X768_60HZ 38    //1360*768, 60HZ
#define ETI_MP4_FORMAT_720P_25HZ     39    //1280*720, 25HZ
#define ETI_MP4_FORMAT_720P_30HZ     40    //1280*720, 30HZ
#define ETI_MP4_FORMAT_1080P_25HZ    41    //1920*1080, 25HZ
#define ETI_MP4_FORMAT_UXGA_25HZ     42    //1600*1200, 25HZ
#define ETI_MP4_FORMAT_XGA_25HZ      43    //1024*768, 25HZ
#define ETI_MP4_FORMAT_SVGA_25HZ     44    //800*600, 25HZ
#define ETI_MP4_FORMAT_QQVGA         45    //160*120
#define ETI_MP4_FORMAT_QNHD          46    //320*180
#define ETI_MP4_FORMAT_NHD           47    //640*360
#define ETI_MP4_FORMAT_A720P_60HZ    48    //960*720, 60HZ
#define ETI_MP4_FORMAT_QXGA_15HZ     49    //2048*1536 15HZ
#define ETI_MP4_FORMAT_QXGA_20HZ     50    //2048*1536 20HZ
#define ETI_MP4_FORMAT_QSXGA_10HZ    51    //2592*1920 10HZ
#define ETI_MP4_FORMAT_UVGA_25HZ     52    //1280*960,25HZ
#define ETI_MP4_FORMAT_UVGA_30HZ     53    //1280*960,30Hz

#define ETI_MP4_FORMAT_VGAX          55     //640*352
#define ETI_MP4_FORMAT_DVGA          56     //960*576

#define ETI_MP4_FORMAT_720x576       58    //576I 25HZ
#define ETI_MP4_FORMAT_720x480       59    //480I 30HZ

#define ETI_MP4_FORMAT_2448x2048_15HZ 60    //抓拍机500万

#define ETI_MP4_FORMAT_384x288       61    //384*288 

#define ETI_MP4_FORMAT_8M            70   ///* 3840 * 2160 */
#define ETI_MP4_FORMAT_12M           71   ///* 4000 * 3000 */
#define ETI_MP4_FORMAT_IR_VGA        72   ///* 640 * 512 */
#define ETI_MP4_FORMAT_IR_QVGA       73   ///* 320 * 256 */
#define ETI_MP4_FORMAT_IR_MVGA       74   ///* 336 * 256 */

#define ETI_MP4_FORMAT_CUSTOM_QVGA   75   ///* 880 * 496 */
#define ETI_MP4_FORMAT_CUSTOM_1080   76   ///* 1440 * 1080*/


/********************** 视频支持格式定义 ****************************/
#define ETI_FORMAT_SUPPORT_QCIF     0X00000001
#define ETI_FORMAT_SUPPORT_CIF      0X00000002
#define ETI_FORMAT_SUPPORT_2CIF     0X00000004
#define ETI_FORMAT_SUPPORT_4CIF     0X00000008
#define ETI_FORMAT_SUPPORT_8CIF     0X00000010

#define ETI_FORMAT_VI_NTSC 0
#define ETI_FORMAT_VI_PAL  1

/************************OSD透明模式******************************/
enum{
    ETI_OSD_MODE_TRANSPARENT = 0,  //透明
    ETI_OSD_MODE_TRANSLUCENT,      //半透明
    ETI_OSD_MODE_OPAQUE            //不透明
};

/********************** 宽带接入类型 ********************************/
#define ETI_ROUTEDEV_ADSL    0    //ADSL
#define ETI_ROUTEDEV_WBN    1    //长城宽带
/********************** 视频尺寸定义 ********************************/
#define ETI_MAX_422_STIDE            1440

#define ETI_MAX_IMAGE_WIDTH          704
#define ETI_MAX_IMAGE_HEIGHT_PAL     576
#define ETI_MAX_IMAGE_HEIGHT_NTSC    480

#define ETI_IMAGE_WIDTH_QCIF        176
#define ETI_IMAGE_WIDTH_CIF         352
#define ETI_IMAGE_WIDTH_2CIF        ETI_MAX_IMAGE_WIDTH
#define ETI_IMAGE_WIDTH_DCIF        528
#define ETI_IMAGE_WIDTH_4CIF        ETI_MAX_IMAGE_WIDTH

#define ETI_IMAGE_HEIGHT_QCIF_PAL   144
#define ETI_IMAGE_HEIGHT_QCIF_NTSC  120
#define ETI_IMAGE_HEIGHT_CIF_PAL    288
#define ETI_IMAGE_HEIGHT_CIF_NTSC   240
#define ETI_IMAGE_HEIGHT_2CIF_PAL   ETI_IMAGE_HEIGHT_CIF_PAL
#define ETI_IMAGE_HEIGHT_2CIF_NTSC  ETI_IMAGE_HEIGHT_CIF_NTSC
#define ETI_IMAGE_HEIGHT_DCIF_PAL   384
#define ETI_IMAGE_HEIGHT_DCIF_NTSC  320
#define ETI_IMAGE_HEIGHT_4CIF_PAL   ETI_MAX_IMAGE_HEIGHT_PAL
#define ETI_IMAGE_HEIGHT_4CIF_NTSC  ETI_MAX_IMAGE_HEIGHT_NTSC

#define ETI_MAX_IMAGE_WIDTH_QVGA    320
#define ETI_MAX_IMAGE_HEIGHT_QVGA   240
#define ETI_MAX_IMAGE_WIDTH_VGA     640
#define ETI_MAX_IMAGE_HEIGHT_VGA    480
#define ETI_MAX_IMAGE_WIDTH_SVGA    800
#define ETI_MAX_IMAGE_HEIGHT_SVGA   600
#define ETI_MAX_IMAGE_WIDTH_XGA     1024
#define ETI_MAX_IMAGE_HEIGHT_XGA    768
#define ETI_MAX_IMAGE_WIDTH_SXGA    1280
#define ETI_MAX_IMAGE_HEIGHT_SXGA   1024
#define ETI_MAX_IMAGE_WIDTH_UXGA    1600
#define ETI_MAX_IMAGE_HEIGHT_UXGA   1200

#define ETI_MAX_IMAGE_WIDTH_720P    1280
#define ETI_MAX_IMAGE_HEIGHT_720P   720
#define ETI_MAX_IMAGE_WIDTH_HDTV    1920
#define ETI_MAX_IMAGE_HEIGHT_HDTV   1080
#define ETI_MAX_IMAGE_WIDTH_QXGA    2048
#define ETI_MAX_IMAGE_HEIGHT_QXGA   1536
#define ETI_MAX_IMAGE_WIDTH_QSXGA   2592
#define ETI_MAX_IMAGE_HEIGHT_QSXGA  1920
#define ETI_MAX_IMAGE_WIDTH_UVGA    1280
#define ETI_MAX_IMAGE_HEIGHT_UVGA   960

/*************************最大视频帧大小**************************/
#define ETI_MAX_IMAGE_FRAMESIZE        (1536*1024)    //最大视频帧大小1.5MB
/*************************最大音频码率****************************/
#define ETI_MAX_AUDIO_BITRATE        64            //最大音频码率64Kb
/****************通道最大图象屏蔽块数*****************/
#define ETI_MAX_COVER_REGION 4
/******************最大图象屏蔽尺寸*******************/
#define ETI_VIDEOMASK_MAXWIDTH        352
#define ETI_VIDEOMASK_MAXHEGITH       288
/*****************************************************/
#define ETI_USER_OPERIAL        10    //操作员个数
#define ETI_DVR_USER_OPERIAL    32    //操作员个数
#define ETI_GROUP_MAXNUM        8     //8个组
/********************** 用户权限定义 ********************************/
#define ETI_USER_RIGHT_ADLOG    0     //登陆用户
#define ETI_USER_RIGHT_ADMIN    1     //管理员权限
#define ETI_USER_RIGHT_HIOPER   2     //高级操作员
#define ETI_USER_RIGHT_OPERIAL  3     //操作员
#define ETI_USER_RIGHT_SUPER    4     //特殊管理员

#define ETI_RIGHT_LOCALPLAY     ((unsigned int)(1<<0))        //本地回放
#define ETI_RIGHT_NETPLAY       ((unsigned int)(1<<1))        //远程回放
#define ETI_RIGHT_NETVIEW       ((unsigned int)(1<<2))        //远程预览
#define ETI_RIGHT_NETTALK       ((unsigned int)(1<<3))        //对讲
#define ETI_RIGHT_LOCALPTZ      ((unsigned int)(1<<4))        //本地云台控制
#define ETI_RIGHT_NETPTZ        ((unsigned int)(1<<5))        //远程云台控制
#define ETI_RIGHT_HANDRECORD    ((unsigned int)(1<<6))        //手动录象
#define ETI_RIGHT_BKFILE        ((unsigned int)(1<<7))        //录象文件备份
#define ETI_RIGHT_LOGMANAGE     ((unsigned int)(1<<8))        //日志管理
#define ETI_RIGHT_PARAMSET      ((unsigned int)(1<<9))        //系统参数设置
#define ETI_RIGHT_UPDATE        ((unsigned int)(1<<10))       //升级系统
#define ETI_RIGHT_SHUTDOWN      ((unsigned int)(1<<11))       //关闭设备
#define ETI_RIGHT_USERMANAGE    ((unsigned int)(1<<12))       //用户帐号管理
#define ETI_RIGHT_ALL           0xFFFFFFFF  //所有权限

typedef enum
{
    ETI_NETVIEW_GROUP = 0,                    //预览组
    ETI_OPERATOR_GROUP,                       //普通操作员组
    ETI_ADVANCE_OPERATOR_GROUP,               //高级操作员组
    ETI_ADMIN_GROUP                           //管理员组
}ETI_RIGHT_GROUP;

/**************************权限信息结构****************************/
typedef struct
{
    unsigned int      m_right;                //按位表示权限，由低到高依次为，本地回放、远程回放、远程预览、对讲、
                                              //本地云台控制、远程云台控制、手动录象、录象文件备份、日志管理、
                                              //系统参数设置、升级系统、关闭设备、用户帐号管理
    unsigned int      m_right1;               //保留
    unsigned int      m_netview;              //远程预览的0~31通道
    unsigned int      m_netview1;             //远程预览的32~63通道
    unsigned int      m_netplay;              //远程回放的0~31通道
    unsigned int      m_netplay1;             //远程回放的32~63通道
    unsigned int      m_localplay;            //本地回放的0~31通道
    unsigned int      m_localplay1;           //本地回放的32~63通道
}ETI_RIGHT_T;

/**************************组信息结构****************************/
typedef struct
{
    ETI_RIGHT_T    m_right;
    char       m_groupname[APP_PASSWORD_LENGTH];
}ETI_GROUP_T;

/**************************用户信息结构****************************/
typedef struct
{
    char            m_username[APP_PASSWORD_LENGTH];
    char            m_password[APP_PASSWORD_LENGTH];
    int             m_groupnum;                         //用户所属的权限组
    ETI_RIGHT_T     m_right;
}ETI_USER_T;

/**************************用户信息参数****************************/
typedef struct
{
    int             m_usernum;                          //用户个数
    int             m_groupnum;                         //有效组个数
    ETI_GROUP_T     m_group[ETI_GROUP_MAXNUM];          //8个组，初始化4个，预留4个
    ETI_USER_T      m_user[ETI_DVR_USER_OPERIAL];       //32个用户，初始化4个
}ETI_SYSUSER_T;

/********************** user info extent(add user stauts) *********/
typedef struct
{
    char m_status[ETI_DVR_USER_OPERIAL];              // the status of user(0:disbale, 1:enable)
    ETI_SYSUSER_T m_user;
}ETI_SYSUSER_EX_T;                                    // extent user info(add the stauts of user)


/********************** 动环监控报警主机用户管理新增定义 ********************************/
#define ETI_USERS_OPERIAL       20  //最大支持20个用户(含超级管理员、一般管理员和操作员3种用户)

/* 动环报警主机权限定义 */
#define ETI_RIGHT_QUERYSTATE    ((unsigned int)(1<<0))        //状态查询
#define ETI_RIGHT_QUERYENV      ((unsigned int)(1<<1))        //环境信息查询
#define ETI_RIGHT_QUERYLOG      ((unsigned int)(1<<2))        //日志查询
#define ETI_RIGHT_SETPARAM      ((unsigned int)(1<<3))        //参数设置
#define ETI_RIGHT_ADMUSER       ((unsigned int)(1<<4))        //用户管理
#define ETI_RIGHT_DEFAULTSYS    ((unsigned int)(1<<5))        //恢复出厂设置
#define ETI_RIGHT_UPDATESYS     ((unsigned int)(1<<6))        //系统升级
#define ETI_RIGHT_SHUTSYS       ((unsigned int)(1<<7))        //关闭系统

#define ETI_RIGHT_NUMS          8                             //总权限数

typedef enum
{
    ETI_OPERATORS_GROUP = 0,                  //操作员组
    ETI_COMADMIN_GROUP,                       //普通管理员组
    ETI_SUPADMIN_GROUP                        //超级管理员组
}ETI_RIGHTS_GROUP;

typedef struct
{
/*按位表示权限，由低到高依次为：状态查询/环境信息查询/日志
 *查询/参数设置/用户管理/恢复出厂设置/系统升级*/
    unsigned int    m_right;                //权限
    unsigned int    m_right1;               //保留
    unsigned int    m_right2;               //保留
    unsigned int    m_right3;               //保留
}ETI_RIGHTS_T;

typedef struct
{
    char            m_groupname[APP_PASSWORD_LENGTH];
    ETI_RIGHTS_T    m_right;
}ETI_GROUPS_T;

typedef struct
{
    char            m_username[APP_PASSWORD_LENGTH];//用户名
    char            m_password[APP_PASSWORD_LENGTH];//密码
    int             m_groupnum;                     //用户所属的权限组
    ETI_RIGHTS_T    m_right;                        //权限
}ETI_USERS_T;

typedef struct
{
    char            m_status[ETI_USERS_OPERIAL];    //the status of user(0:disbale,1:enable)
    int             m_groupnum;                     //有效组个数
    int             m_usernum;                      //用户个数
    ETI_GROUPS_T    m_group[ETI_GROUP_MAXNUM];      //3个组，初始化3个，预留5个
    ETI_USERS_T     m_user[ETI_USERS_OPERIAL];      //20个用户，初始化1个，剩余用户由有添加用户权限的用户添加
}ETI_SYSUSERS_T;
/********************** 动环监控报警主机用户管理新增定义结束 ********************************/


/********************** 串口属性定义***************************/
//数据位
enum{UART_DATABIT_5 = 5U,UART_DATABIT_6 = 6U,UART_DATABIT_7 = 7U,UART_DATABIT_8 = 8U};
//停止位
enum{UART_STOPBIT_0 = 0U,UART_STOPBIT_1 = 1U,UART_STOPBIT_2 = 2U};
//校验位
enum{UART_VERIFY_NONE = 0U,UART_VERIFY_ODD = 1U,UART_VERIFY_EVEN = 2U,UART_VERIFY_MASK1 = 3U,UART_VERIFY_MASK0 = 4U};
//流控
enum{UART_STREAMCTRL_NONE = 0U,UART_STREAMCTRL_SOFT = 1U,UART_STREAMCTRL_HARD = 2U};
/**************错误代码定义***************************/
#define ETI_OK        0
#define ETI_ERROR    -1

/**************程序升级错误代码***************/
#define ETI_UDPATE_ERR_MASKID     -1
#define ETI_UPDATE_ERR_SIZE       -2
#define ETI_UPDATE_ERR_OFFSET     -3
#define ETI_UPDATE_ERR_WRITEFLASH -4
/*****************************************************/

/**************系统语言类型定义***************************/
#define ETI_SYSLANGUAGE_CHINA    1   //中文
#define ETI_SYSLANGUAGE_ENGLISH  2   //英文
/*****************************************************/

/***************云台控制协议定义********************/
#define ETI_PTZTYPE_NONE        0    //无
#define ETI_PTZTYPE_PELCOP      1
#define ETI_PTZTYPE_PELCOD      2
#define ETI_PTZTYPE_HUNTERD     3
#define ETI_PTZTYPE_YINGKE      4
#define ETI_PTZTYPE_SONY100     5

#define ETI_PTZCMDMAXSIZE       4096
#define ETI_PTZCMDMAXSIZE_EX    16384
#define ETI_LICENSEMAXSIZE      1024

/*****************************************************/

/**************网络传输类型和传输协议支持*************/
#define ETI_TRAN_TYPE_UDP        1
#define ETI_TRAN_TYPE_MULTI      2
#define ETI_TRAN_TYPE_TCP        3
#define ETI_TRAN_TYPE_RTP        4

#define LAN_NET_TRANTYPE_UDP    ETI_TRAN_TYPE_UDP
#define LAN_NET_TRANTYPE_MULTI  ETI_TRAN_TYPE_MULTI
#define LAN_NET_TRANTYPE_TCP    ETI_TRAN_TYPE_TCP
#define LAN_NET_TRANTYPE_RTP    ETI_TRAN_TYPE_RTP    

#define ETI_TRAN_SUPPROT_UDP    0X01
#define ETI_TRAN_SUPPORT_MULTI  0X02
#define ETI_TRAN_SUPPORT_TCP    0X04
#define ETI_TRAN_SUPPORT_RTP    0X08
/*****************************************************/
enum{
    ETI_VOVIEWMODE_1 = 1,
    ETI_VOVIEWMODE_2 = 2,
    ETI_VOVIEWMODE_4 = 4,
    ETI_VOVIEWMODE_6 = 6,
    ETI_VOVIEWMODE_8 = 8,
    ETI_VOVIEWMODE_9 = 9,
};
/***************移动报警、图象屏蔽********************/
/*
 * 对于PAL制式,高能被32整除
 * 对于NTSC制式,高不能被32整除,最后一行采用32*16的块大小
*/
#define ETI_MOTIONAREA_SIZE     396    //(704/32)*(576/32) = 18 * 22
/*****************************************************/

#define ETI_MAX_OSD_SIZE        32

/************报警发生时通过网络备份文件选项**************/
#define ETI_ALARM_BACK_FTP      0X01    //通过FTP备份
#define ETI_ALARM_BACK_MODEM    0X02    //通过拨号备份

/********************** 帧头ID定义 ********************************/
#define ETI_FRAME_HEAD_ID       0XB2010000  // 老帧头部标示，9000\8000\5000\5100系列使用
#define ETI_DVR_FRAME_HEAD_ID   0XB3010000  // 新帧头部标示
#define ETI_VCA_FRAME_HEAD_ID   0XB4010000  // 智能分析帧头标示

#define ETI_BRV_FRAME_HEAD_ID        ETI_FRAME_HEAD_ID + 1  // 博维帧头ID，阿尔卡特平台定制版本

/********************** 帧类型定义 ********************************/
#define ETI_FRAME_CH_MASK        0X000F       //0X0001111B
#define ETI_FRAME_TIMERESET      0X0010       //0X0010000B
#define ETI_FRAME_KEY_MASK       0X0020       //0X0100000B
#define ETI_FRAME_VIDEOSIZE_MAXK 0X0040       //0X1000000B图像尺寸标志
#define ETI_FRAME_DATA_MASK      0XFF80
#define ETI_FRAME_VIDEOSIZE_LEN  4            //(width + height)

/********************** DVR帧类型定义 *****************************/
#define    ETI_DVR_FRAME_KEY_MASK    1<<0        //视频I帧
#define ETI_DVR_FRAME_PFRAME_MASK    1<<1        //视频P帧
#define ETI_DVR_FRAME_BFRAME_MASK    1<<3        //视频B帧
#define ETI_DVR_FRAME_AUDIO          1<<4        //包含音频数据
#define ETI_DVR_FRAME_FILEHEAD       1<<5        //包含文件头
#define ETI_VCA_FRAME_INFOHEAD       1<<6        //包含智能分析信息头
#define ETI_DVR_FRAME_SYSHEAD        1<<7        //包含 系统头

/********************** 智能分析信息类型定义 **********************/
#define ETI_VCA_FRAME_INFO_TYPE_PLATE 0x01       //智能分析信息类型：车牌识别信息

/* 修改隐式转换的bug */
#if defined(DVR_BIG_ENDIAN)
#define ETI_DVR_FRAME_ISKEY(buff) \
    (lc_swab32((unsigned int)(((ETI_DVR_FRMAME_HEAD*)(buff))->nVideoSize)) > 0U && (lc_swab16(((ETI_DVR_FRMAME_HEAD*)(buff))->nMuxData) & (unsigned short)(ETI_FRAME_KEY_MASK)))
#define ETI_DVR_FRAME_ISFRAME(buff) \
    ((lc_swab32(((ETI_DVR_FRMAME_HEAD*)(buff))->nID) == ETI_DVR_FRAME_HEAD_ID) && ((lc_swab32((unsigned int)(((ETI_DVR_FRMAME_HEAD*)(buff))->nVideoSize)) + lc_swab32(((ETI_DVR_FRMAME_HEAD*)(buff))->nAudioSize) > 0U)))
#define ETI_DVR_FRAME_ISVIDEO(buff) (int)lc_swab32((unsigned int)(((ETI_DVR_FRMAME_HEAD*)(buff))->nVideoSize)) > 0 ? 1 : 0
#define ETI_DVR_FRAME_TICK(buff)    lc_swab32(((ETI_DVR_FRMAME_HEAD*)(buff))->nTimeTick)
#define ETI_DVR_FRAME_ISRESET(buff) (lc_swab16(((ETI_DVR_FRMAME_HEAD*)(buff))->nMuxData) & (unsigned short)(ETI_FRAME_TIMERESET))
#define ETI_DVR_FRAME_GMTTIME(buff) lc_swab32(((ETI_DVR_FRMAME_HEAD*)(buff))->nGMTTime)
#else
#define ETI_DVR_FRAME_ISKEY(buff) \
    ( ((ETI_DVR_FRMAME_HEAD*)(buff))->nVideoSize > 0 && (((ETI_DVR_FRMAME_HEAD*)(buff))->nFrameType & ETI_DVR_FRAME_KEY_MASK))
#define ETI_DVR_FRAME_ISFRAME(buff) \
    ((((ETI_DVR_FRMAME_HEAD*)(buff))->nID == ETI_DVR_FRAME_HEAD_ID) && ((((ETI_DVR_FRMAME_HEAD*)(buff))->nVideoSize + ((ETI_DVR_FRMAME_HEAD*)(buff))->nAudioSize > 0)))
#define ETI_DVR_FRAME_ISVIDEO(buff) ((ETI_DVR_FRMAME_HEAD*)(buff))->nVideoSize > 0 ? 1 : 0
#define ETI_DVR_FRAME_TICK(buff)    ((ETI_DVR_FRMAME_HEAD*)(buff))->nTimeTick
#define ETI_DVR_FRAME_ISRESET(buff) (((ETI_DVR_FRMAME_HEAD*)(buff))->nMuxData & ETI_FRAME_TIMERESET)
#define ETI_DVR_FRAME_GMTTIME(buff) ((ETI_DVR_FRMAME_HEAD*)(buff))->nGMTTime
#endif

#define ETI_FRAME_ISKEY(buff) \
    ( ((ETI_FRMAME_HEAD*)(buff))->nVideoSize > 0 && ((ETI_FRMAME_HEAD*)(buff))->nMuxData & ETI_FRAME_KEY_MASK)
#define ETI_FRAME_ISFRAME(buff)    \
    ((((ETI_FRMAME_HEAD*)(buff))->nID == ETI_FRAME_HEAD_ID) && ((((ETI_FRMAME_HEAD*)(buff))->nVideoSize + ((ETI_FRMAME_HEAD*)(buff))->nAudioSize > 0)))
#define ETI_FRAME_ISVIDEO(buff)     ((ETI_FRMAME_HEAD*)(buff))->nVideoSize > 0 ? 1 : 0
#define ETI_FRAME_TICK(buff)        ((ETI_FRMAME_HEAD*)(buff))->nTimeTick
#define ETI_FRAME_ISRESET(buff)     ( ((ETI_FRMAME_HEAD*)(buff))->nMuxData & ETI_FRAME_TIMERESET )

#define ETT_BRV_PACKETSEQSIZE(pframehead)    (pframehead->m_filehead.m_totalframes == 0?0:(pframehead->m_filehead.m_totalframes - 1)*sizeof(DATA_BRV_CHECK_HEND_T))

#define DVR_FILE_HEAD_SIZE(buff) (((ETI_DVR_FRMAME_HEAD*)(buff))->nFrameType & ETI_DVR_FRAME_FILEHEAD?(int)sizeof(ETI_FILE_HEAD):0)
#define DVR_SYS_HEAD_SIZE(buff) (((ETI_DVR_FRMAME_HEAD*)(buff))->nFrameType & ETI_DVR_FRAME_SYSHEAD?((ETI_DVR_FRMAME_HEAD*)(buff))->nVideoSize:0)

#define ETI_VCA_FRAME_ISKEY(buff)    ETI_DVR_FRAME_ISKEY(buff)
#define ETI_VCA_FRAME_ISFRAME(buff) \
((((ETI_VCA_FRMAME_HEAD*)(buff))->nID == ETI_VCA_FRAME_HEAD_ID) && ((((ETI_VCA_FRMAME_HEAD*)(buff))->nVideoSize + ((ETI_VCA_FRMAME_HEAD*)(buff))->nAudioSize > 0)))
#define ETI_VCA_FRAME_ISVIDEO(buff) ETI_DVR_FRAME_ISVIDEO(buff)
#define ETI_VCA_FRAME_TICK(buff)    ETI_DVR_FRAME_TICK(buff)
#define ETI_VCA_FRAME_ISRESET(buff) ETI_DVR_FRAME_ISRESET(buff)
#define ETI_VCA_FRAME_GMTTIME(buff) ETI_DVR_FRAME_GMTTIME(buff)

#define VCA_FILE_HEAD_SIZE(buff)    DVR_FILE_HEAD_SIZE(buff)
#define VCA_INFO_HEAD_SIZE(buff)    (((ETI_VCA_FRMAME_HEAD*)(buff))->nFrameType & ETI_VCA_FRAME_INFOHEAD?sizeof(ETI_VCA_INFO_HEAD):0)
#define VCA_INFO_DATA_TYPE(buff)    ((ETI_VCA_INFO_HEAD*)((char*)(buff) + sizeof(ETI_VCA_FRMAME_HEAD) + VCA_FILE_HEAD_SIZE(buff)))->vca_type
#define VCA_INFO_DATA_ISPLATE(buff)    (VCA_INFO_DATA_TYPE(buff) == ETI_VCA_FRAME_INFO_TYPE_PLATE)
#define VCA_INFO_DATA_SIZE(buff)    ((ETI_VCA_INFO_HEAD*)((char*)(buff) + sizeof(ETI_VCA_FRMAME_HEAD) + VCA_FILE_HEAD_SIZE(buff)))->vca_length

#define ETI_STREAMINFO_VIDEOID_DVS    0
#define ETI_STREAMINFO_VIDEOID_DVR    1

#define ETI_DVS_FILE_VERSION        1
#define ETI_DVR_FILE_VERSION        2
#define ETI_AUDIO_FILE_VERSION      3

typedef struct
{
/*
nMuxData Format :16 bits
最大用户数据长度为1024
-----------------------------------------------------
|0--4 通道号| 5 帧类型| 6 图像尺寸标志|7---15 用户数据长度|
如果有图像尺寸标志，在nVideoSize后加上4个字节的图像宽度和高度
-----------------------------------------------------
*/
    //unsigned char  nID[4];     //0X00,0X00,0X01,0XB2
    unsigned int   nID;          //0XB2010000
    unsigned int   nTimeTick;    //时间戳
    int               nVideoSize;//视频数据长度
    unsigned short nAudioSize;   //音频数据长度
    unsigned short nMuxData;     //复合数据：视频通道、帧类型、用户数据
}ETI_FRMAME_HEAD,*PETI_FRMAME_HEAD;

/*
1.在视频I帧或者音频格式变化时包含文件头信息,nFrameType = 0x20,文件头长度sizeof(ETI_FILE_HEAD).
2.ID定义为 ETI_DVR_FRAME_HEAD_ID,0XB3010000,此ID在MPEG4或者H264码流中不会存在.
3.如果码流中同时包含音视频数据和文件头信息,排列顺序为: 文件头信息,视频数据,音频数据,nFrameCount按视频数据进行填写.
4.nGMTTime按照GMT时间输入和输出,用于定位当前帧的绝对播放时间.
5.nFrameCount按照编码帧数进行累加,用于判断发送时是否存在丢帧,丢帧后不进行后面P帧解码,避免造成图像花屏.
6.nMuxData去掉原来功能,暂时保留,备用.
*/
typedef struct
{
    unsigned int    nID;            //ETI_DVR_FRAME_HEAD_ID,0XB3010000
                                    //ETI_VCA_FRAME_HEAD_ID,0XB4010000 智能分析帧头标示
    unsigned int    nTimeTick;      //时间戳,相对时间,毫秒为单位
    unsigned int    nGMTTime;       //绝对时间,自1970年1月1日 00:00:00 开始的秒数,时区按GMT时区计算.
    int             nFrameType;     //低4位表示视频帧类型: 0->I-FRAME/1->P-FRAME/3->B-FRAME
                                    //第5位表示是否包含音频: 0x10表示有音频帧
                                    //第6位表示是否包含文件头信息: 0x20:表示有文件头信息
                                    //第7位表示是否包含智能分析信息：0x40表示有智能分析信息
                                    //第8位表示是否包含用于解码初始化的系统头：ETI_DVR_FRAME_SYSHEAD 1<<7 使用nVideoSize 表示系统头大小
    int             nFrameCount;    //编码帧序号
    int             nVideoSize;     //视频数据长度
    unsigned short  nAudioSize;     //音频数据长度
    unsigned short  nMuxData;       //复合数据
}
ETI_DVR_FRMAME_HEAD,*PETI_DVR_FRMAME_HEAD,
ETI_VCA_FRMAME_HEAD,*PETI_VCA_FRMAME_HEAD; // 智能分析帧头

typedef struct
{
    unsigned int    nID;            //ETI_DVR_FRAME_HEAD_ID,0XB3010000
    unsigned int    nTimeTick;      //时间戳,相对时间,毫秒为单位
    unsigned int    nGMTTime;       //绝对时间,自1970年1月1日 00:00:00 开始的秒数,时区按GMT时区计算.
    int             nFrameType;     //第6位表示是否包含文件头信息: 0x20:表示有文件头信息
    int             nFrameCount;    //编码帧序号
    int             nAudioSizeOne;  //音频1长度
    unsigned short  nAudioSizeTwo;  //音频2长度
    unsigned short  nMuxData;        //复合数据
} ETI_AUDIO_FRMAME_HEAD,*PETI_AUDIO_FRMAME_HEAD; // 双音频帧头


#define ETI_FILE_MASKEND        0XABCEDF01
#define ETI_FILEMASK            "LAUNCHDIGITAL"
#define ETI_FILEMASK_NEW        "MP4RECORDFILE"
#define ETI_FILEMASK_DX_NOAUDIO "DXNOAUDIOFILE"
#define TARGET_DX4

typedef struct
{
    char            m_filemask[16]; //文件头MASK "LAUNCHDIGITAL"
    int             m_fileversion;  //文件版本号 ETI_DVR_FILE_VERSION为结构 ETI_DVR_FILE_HEAD
                                    //           ETI_DVS_FILE_VERSION为结构 ETI_FILE_HEAD
    int             videofoucc;     //视频FOURCC
    short           framerate;      //视频帧率
    short           videoid;        //视频ID 暂时保留 0
    short           m_width;        //图像宽度
    short           m_height;       //图像高度
    short           m_audiotag;     //音频标识
    short           m_haveaudio;    //是否包含音频标志
    short           m_auchs;        //音频通道号
    short           m_aurates;      //音频码率
    int             m_ausample;     //音频采样率
    unsigned int    m_totalframes;  //文件中包含的帧数
    unsigned int    m_firsttick;    //第一帧时间戳
    unsigned int    m_losttick;     //最后一帧时间戳
    int             maskend;        //结束标志
}ETI_FILE_HEAD;

typedef struct
{
    char            m_filemask[16]; //文件头MASK "LAUNCHDIGITAL"
    int             m_fileversion;  //文件版本号 ETI_DVR_FILE_VERSION为结构 ETI_DVR_FILE_HEAD
                                    //           ETI_DVS_FILE_VERSION为结构 ETI_FILE_HEAD
    int             videofoucc;     //视频FOURCC
    short           framerate;      //视频帧率
    short           videoid;        //视频ID 暂时保留 0
    short           m_width;        //图像宽度
    short           m_height;       //图像高度
    short           m_audiotag;     //音频标识
    short           m_haveaudio;    //是否包含音频标志
    short           m_auchs;        //音频通道号
    short           m_aurates;      //音频码率
    int             m_ausample;     //音频采样率
    unsigned int    m_totalframes;  //文件中包含的帧数
    unsigned int    m_firsttick;    //第一帧时间戳
    unsigned int    m_losttick;     //最后一帧时间戳
    unsigned int    m_reserved;     //保留位
    unsigned int    m_firstGMTTime; //文件开始绝对时间,时区按GMT时区计算.
    unsigned int    m_lastGMTTime;  //文件结束绝对时间,时区按GMT时区计算.
    int             maskend;        //结束标志
}ETI_DVR_FILE_HEAD;

typedef struct
{
    char            m_filemask[16];  //文件头MASK "LAUNCHDIGITAL"
    int             m_fileversion;   //文件版本号 ETI_DVR_FILE_VERSION 为结构 ETI_DVR_FILE_HEAD
                                     //           ETI_DVS_FILE_VERSION 为结构 ETI_FILE_HEAD
                                     //           ETI_AUDIO_FILE_VERSION 为结构 ETI_AUDIO_FILE_HEAD
    short           m_audiotag1;     //音频标识
    short           m_haveaudio1;    //是否包含音频标志
    short           m_auchs1;        //音频通道号
    short           m_aurates1;      //音频码率
    int             m_ausample1;     //音频采样率

    short           m_audiotag2;     //音频标识
    short           m_haveaudio2;    //是否包含音频标志
    short           m_auchs2;        //音频通道号
    short           m_aurates2;      //音频码率
    int             m_ausample2;     //音频采样率

    unsigned int    m_totalframes1;  //音频1文件中包含的帧数
    unsigned int    m_totalframes2;  //音频2文件中包含的帧数

    unsigned int    m_firsttick;     //第一帧时间戳
    unsigned int    m_losttick;      //最后一帧时间戳

    unsigned int    m_firstGMTTime;  //文件开始绝对时间,时区按GMT时区计算.
    unsigned int    m_lastGMTTime;   //文件结束绝对时间,时区按GMT时区计算.

    unsigned int    m_reserved;      //保留位
    int             maskend;         //结束标志
} ETI_AUDIO_FILE_HEAD;


//////////////////////////DXHJ BEGIN///////////////////////////////
typedef struct
{
    char            m_court[48];        //主机所在法院
    char            m_courtroom[48];    //主机所在法庭
    char            m_id[16];           //主机唯一识别号
    char            m_szCaseID[28];     //case id
    char            m_szCaseExecor[28]; //case 执行人
    char            m_recv1[32];        //保留1
    char            m_recv2[32];        //保留2
    char            m_recv3[32];        //保留3
}DX_FILE_HEAD;

typedef struct
{
    char            m_filemask[16]; //文件头MASK "LAUNCHDIGITAL"
    int             m_fileversion;  //文件版本号 ETI_DVR_FILE_VERSION为结构 ETI_DVR_FILE_HEAD
                                    //           ETI_DVS_FILE_VERSION为结构 ETI_FILE_HEAD
    int             videofoucc;     //视频FOURCC
    short           framerate;      //视频帧率
    short           videoid;        //视频ID 暂时保留 0
    short           m_width;        //图像宽度
    short           m_height;       //图像高度
    short           m_audiotag;     //音频标识
    short           m_haveaudio;    //是否包含音频标志
    short           m_auchs;        //音频通道号
    short           m_aurates;      //音频码率
    int             m_ausample;     //音频采样率
    unsigned int    m_totalframes;  //文件中包含的帧数
    unsigned int    m_firsttick;    //第一帧时间戳
    unsigned int    m_losttick;     //最后一帧时间戳
    unsigned int    m_reserved;     //保留位
    unsigned int    m_firstGMTTime; //文件开始绝对时间,时区按GMT时区计算.
    unsigned int    m_lastGMTTime;  //文件结束绝对时间,时区按GMT时区计算.
    DX_FILE_HEAD    m_dxfile;       //录像头中添加法院信息
    int             maskend;        //结束标志
}ETI_DX4_DVR_FILE_HEAD;

//////////////////////////DXHJ END/////////////////////////////////

typedef struct
{
    unsigned int    nID;            //ETI_BRV_FRAME_HEAD_ID,0XB2010001
    unsigned int    nTimeTick;      //时间戳
    int             nVideoSize;     //视频数据长度
    unsigned short  nAudioSize;     //音频数据长度
    unsigned short  nMuxData;       //复合数据：视频通道、帧类型、用户数据
    ETI_FILE_HEAD   m_filehead;     //文件头
}ETI_BRV_FRMAME_HEAD,*PETI_BRV_FRMAME_HEAD;

#define MIN_FRMAME_HEAD ETI_FRMAME_HEAD
typedef struct
{
    unsigned short    framenum;
    unsigned short    packnum;
}DATA_BRV_CHECK_HEND_T;

#define BRV_PACETSIZE        1116


// 智能分析信息结构
typedef struct
{
    unsigned int vca_type;          // 智能分析信息类型，0x01-车牌识别信息
    unsigned int vca_length;        // 智能分析信息长度，以字节为单位
}ETI_VCA_INFO_HEAD;

typedef struct
{
    short left;                     // 矩形区域的左上角X坐标   
    short right;                    // 矩形区域的右下角X坐标
    short top;                      // 矩形区域的左上角Y坐标
    short bottom;                   // 矩形区域的右下角Y坐标
}ETI_VCA_PLATERECT;

#ifndef MAX_VCA_PLATERECT_NUM
#define MAX_VCA_PLATERECT_NUM  4    // 车牌分析最多车牌数
#endif

#ifndef MAX_VCA_PLATERSTR_NUM
#define MAX_VCA_PLATERSTR_NUM 12    // 车牌分析车牌号码  
#endif

// 智能分析车牌信息
typedef struct 
{
    unsigned int        version;                                   // 车牌识别信息版本，0x01-V1.0
    unsigned int        time_stamp;                                // 分析时间戳，以毫秒为单位
    unsigned int        pos_num;                                   // 当前帧定位到的车牌数目
    ETI_VCA_PLATERECT   pos_rect[MAX_VCA_PLATERECT_NUM];           // 当前帧定位到的车牌在原始图像中的位置
    unsigned char       recog_num;                                 // 识别出的车牌号数目
    unsigned char       reserverd1[3];                             // 保留参数，置为0
    unsigned int        recog_gmttime[MAX_VCA_PLATERECT_NUM];      // 识别到的车牌最早出现的绝对时间，自1970年1月1日 00:00:00 开始的秒数,时区按GMT时区计算
    unsigned char       recog_number[MAX_VCA_PLATERECT_NUM][MAX_VCA_PLATERSTR_NUM];  // 识别出来的车牌字符
    unsigned char       recog_trust[MAX_VCA_PLATERECT_NUM];        // 车牌字符的置信度
    unsigned char       recog_brightness[MAX_VCA_PLATERECT_NUM];   // 车牌亮度值1 ~ 255
    unsigned char       recog_color[MAX_VCA_PLATERECT_NUM];        // 车牌颜色 0:蓝底1:黄底2:白底－军牌3:白底－警牌4:黑牌5:未知
    unsigned char       recog_direction[MAX_VCA_PLATERECT_NUM];    // 车牌运动方向 0:未知方向 1:车牌从上到下运动 2:表示车牌从下到上运动
    unsigned char       recog_type[MAX_VCA_PLATERECT_NUM];         // 车牌类型 1:大型汽车号牌 2:小型汽车号牌 16:教练汽车号牌 23:警用汽车号牌 99:未知
    unsigned int        recog_speeds[MAX_VCA_PLATERECT_NUM];       // 车牌运动速度
    unsigned char       recog_carcolor[MAX_VCA_PLATERECT_NUM];     // 车身颜色 0:未知 1:蓝色 2:黑色 3:白色 4:黄色 5:红色 6:绿色 -1:失败
}ETI_VCA_PLATEINFO;

/********************** 日期时间信息格式定义 ********************************/
typedef enum{
    ETI_TIME_FORMAT_OFF = 0X00,     //关闭
    ETI_TIME_FORMAT_1    = 0X01,    //2003-09-12 15:23:33
    ETI_TIME_FORMAT_2    = 0X02,    //2003-09-12 星期五 15:23:33
    ETI_TIME_FORMAT_3    = 0X03,    //09-12-2003 15:23:33
    ETI_TIME_FORMAT_4    = 0X04,    //09-12-2003 星期五 15:23:33
    ETI_TIME_FORMAT_5    = 0X05,    //12-09-2003 15:23:33
    ETI_TIME_FORMAT_6    = 0X06,    //12-09-2003 星期五 15:23:33
    ETI_TIME_FORMAT_7    = 0X07,    //2003/09/12 15:23:33
    ETI_TIME_FORMAT_8    = 0X08,    //09/12/2003 15:23:33
    ETI_TIME_FORMAT_9    = 0X09,    //12/09/2003 15:23:33
    ETI_TIME_FORMAT_A    = 0X0A,    //2003.09.12 15:23:33
    ETI_TIME_FORMAT_B    = 0X0B,    //09.12.2003 15:23:33
    ETI_TIME_FORMAT_C    = 0X0C     //12..09.2003 15:23:33
}ETI_TIME_FORMAT;
/*****************************************************/

/**************时间表日期***************************/
typedef enum
{
    ETI_DAY_MONDAY        = 0,    //星期一
    ETI_DAY_TUESDAY       = 1,    //星期二
    ETI_DAY_WEDNESDAY     = 2,    //星期三
    ETI_DAY_THURSDAY      = 3,    //星期四
    ETI_DAT_FRIDAY        = 4,    //星期五
    ETI_DAY_SATURDAY      = 5,    //星期六
    ETI_DAY_SUNDAY        = 6,    //星期天
    ETI_DAY_MON_FRI       = 7,    //星期一-星期五
    ETI_DAY_SAT_SUN       = 8,    //星期六-星期天
    ETI_DAY_DAILY         = 9,    //每天
    ETI_DAY_OFF           = 10,   //关闭
}ETI_TIME_DAYTYPE;
/*****************************************************/

//码率控制模式，我公司与海思定义的RC_MODE_E不同，CBR和VBR相反(兼容以前设计)
typedef enum
{
    ETI_MODE_CBR = 0,
    ETI_MODE_VBR,
    ETI_MODE_ABR,
    ETI_MODE_FIXQP,
}ETI_MODE_E;


#define ETI_MAX_JPEG_SIZE 2000*1024
#define ETI_PLATE_STR_SIZE 12

typedef struct
{
    int     m_filesize;         //文件大小
    int     m_year;             //年
    char    m_month;            //月
    char    m_day;              //日
    char    m_hour;             //时
    char    m_captype;          //抓拍类型:0x01-手动抓拍;0x02-超速抓拍;0x04-闯红灯抓拍;0x08-卡口抓拍;
    char    m_filename[60];     //文件名
    char    m_tollgateid[32];   //卡口编号
    char    m_deviceid[32];      //设备编号
} ETI_JPEG_HEAD;

typedef struct
{
    char    m_roadwaynum;       //车道号
    char    m_color;            //车牌颜色
    char    m_ptype;            //车牌类型
    char    m_car_color;        //车身颜色
    char    m_direction;        //方向
    char    m_reserved;         //保留    
    short   m_speed;            //车速
    int     m_captype;          //抓拍类型:0x01-手动抓拍;0x02-超速抓拍;0x04-闯红灯抓拍;0x08-卡口抓拍;
    char    m_filename[60];     //文件名
    char    m_tollgateid[32];   //卡口编号
    char    m_deviceid[32];     //设备编号
    char    m_platestr[ETI_PLATE_STR_SIZE];     //车牌号
    char    m_reserve[32];      //保留，用于扩展
} ETI_JPEG_HEAD_EX;

#define ETI_MAX_JPEG_NUM        10
typedef struct
{
    short   m_year;             //年
    short   m_msecond;          //毫秒
    char    m_month;            //月
    char    m_day;              //日
    char    m_hour;             //时
    char    m_minute;           //分
    char    m_second;           //秒
    char    m_is_plate;         //是否为车牌图片
    char    m_seqnum;           //抓拍图片序号
    char    m_reserve;          //保留
    int     m_size;             //图片大小
    int     m_offset;           //图片偏移
} JPEG_INFO;

typedef struct
{
    int                     m_jpgnum;                       //图片数量
    int                     m_totalsize;                    //图片总大小
    JPEG_INFO               m_jpeg_info[ETI_MAX_JPEG_NUM];  //偏移和大小
    ETI_JPEG_HEAD_EX        m_jpeg_head;                    //图片头信息
    char                    m_reserve[32];                  //保留位
} ETI_JPEG_PACK;

/* tyto 抓拍图片信息*/
typedef struct
{
    unsigned int            m_doffset;          //图片数据偏移
    unsigned int            m_dlen;             //图片数据长度
    short                   m_width;            //图像宽度
    short                   m_height;           //图像高度
    unsigned int            m_timestamp;        //时间戳
    short                   m_year;             //年
    short                   m_msecond;          //毫秒
    char                    m_month;            //月
    char                    m_day;              //日
    char                    m_hour;             //时
    char                    m_minute;           //分
    char                    m_second;           //秒
    char                    m_reserve[3];       //保留
} ETI_JPEG_INFO;

typedef enum
{
    WEB_MSG_OPT_RESTART         = 0,   //服务器正在重启 未区分恢复默认重启 需要就添加
    WEB_MSG_OPT_RELOGIN         = 1,   //请重新登录设备
    WEB_MSG_OPT_UPDATA          = 2,   //系统正在升级,请稍候
    WEB_MSG_OPT_UPDATASUCCESS   = 3,   //程序升级成功,请重新登录
    WEB_MSG_OPT_UPDATAILLEGAL   = 4,   //您上传的文件格式不正确,无法升级,但是系统还是需要重启...
    WEB_MSG_OPT_USERERROR       = 5,   //用户名或密码错误
    WEB_MSG_OPT_NORIGHT         = 6,   //您没有权限 
    WEB_MSG_OPT_FORMAT          = 7,   //格式化硬盘成功，设备需要重启
    WEB_MSG_OPT_FORMATERROR     = 8,   //格式化硬盘失败，设备需要重启
    WEB_MSG_OPT_DEFAULT         = 9,   //恢复出厂设置
    WEB_MSG_OPT_USERCHANGE      = 10,  //密码修改成功，请重新登录
    WEB_MSG_OPT_DISABLED        = 11,  //用户已禁用
    WEB_MSG_OPT_OLDPASSWD_ERR   = 12,  //原密码错误
    WEB_MSG_OPT_SETUSER_ERR     = 13,  //设置用户(增加/删除/修改用户)失败
    WEB_MSG_OPT_RESTARTFAILED   = 14,  //服务器重启失败
    WEB_MSG_OPT_LOOPING         = 15,  //正在轮巡
}ETI_WEB_RETURN_ORDER;

/*-------------------IR Raw extra data----------------------*/
typedef enum
{
	ETI_EXDATA_SENSOR_INVALID		= 0,
	ETI_EXDATA_SENSOR_START_FPA		= 1,
	ETI_EXDATA_SENSOR_CURRENT_FPA	= 2,
	ETI_EXDATA_SENSOR_HOUSE_TEMP	= 3,
	ETI_EXDATA_SENSOR_SHUTTER_TEMP	= 4,
	ETI_EXDATA_SENSOR_OTHER			= 5
}ETI_EXDATA_TEMPSENSORID;

typedef struct  
{
	unsigned char	temp_sensor_id;//TempSensor类型 
	unsigned char	resverve[3];
	/*1-start FPA:启动FPA
	  2-Current FPA:当前FPA
	  3-House Temp:外壳温度
	  4-Shutter Temp:快门温度
	  5-其他*/
	float			temprature;//温度(K)
}ETI_EXDATA_TEMPSENSOR;

typedef enum
{
	ETI_EXDATA_CHIP_INVALID = 0,
	ETI_EXDATA_CHIP_LP2_0	= 1,
	ETI_EXDATA_CHIP_LP2_5	= 2,
	ETI_EXDATA_CHIP_LP3_0	= 3,
	ETI_EXDATA_CHIP_LP3_5	= 4,
	ETI_EXDATA_CHIP_TAU2	= 5,
	ETI_EXDATA_CHIP_OTHER	= 6,
}ETI_EXDATA_SENSOR_CHIP;

typedef struct  
{
	unsigned char			sensor_type;//0-无效 1-lepton2.0 2-lepton2.5 3-lepton3.0 4-lepton3.5 5-tau2 6-其他
	unsigned char			reserve_1[3];//
	float					planck_r;//普朗克常量
	float					planck_b;
	float					planck_f;
	float					planck_o;
	unsigned char			temp_sensor_cnt;//温度传感器个数 最多支持20个
	unsigned char			reserve_2;
	unsigned short			temp_sensor_offset;
	ETI_EXDATA_TEMPSENSOR	temp_sensor[0];//
}ETI_EXTRA_DATA;
/*-------------------IR Raw extra data end------------------*/
#endif  //__ETI_BASE_DEF_H__

