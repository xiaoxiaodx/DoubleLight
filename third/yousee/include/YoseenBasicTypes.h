/*
@file YoseenBasicTypes.h
@author yoseen
@date 2018-04-01
*/
#ifndef YOSEENBASICTYPES_H_
#define YOSEENBASICTYPES_H_

/**
基本类型
*/
typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;

#define S8_MIN			(-128)
#define S8_MAX			127
#define S16_MIN			(-32768)
#define S16_MAX			32767
#define S16_RANGE		(65536)
#define U16_MAX			(65535)
#define U16_RANGE		(65536)

#define S32_MIN			(-2147483647 - 1)
#define U32_MAX			0xffffffff

typedef struct _bgra {
	u8 b;
	u8 g;
	u8 r;
	u8 a;
}bgra;

/**
*/
#ifdef SDK_EXPORT
#define SDK_API __declspec(dllexport)
#else
#define SDK_API __declspec(dllimport)
#endif

#define _CRT_SECURE_NO_WARNINGS
#define ARRAY_SIZE(x)						(sizeof(x)/sizeof(x[0]))
#define MIN(a,b)				((a)<(b)?(a):(b))
#define MAX(a,b)				((a)>(b)?(a):(b))
#define ABS(a)					((a)>0?(a):-(a))

#define XMALLOC(size,alignment)	_aligned_malloc(size,alignment)
#define XFREE(x)				_aligned_free(x)
#define xmalloc(size,alignment)	_aligned_malloc(size,alignment)
#define xfree(x)				_aligned_free(x)

//
#ifdef __linux__
#define SOCKET				int
#define INVALID_SOCKET		(-1)
#define HWND				(void*)
#define __stdcall
#endif

/*
错误码
*/
enum YoseenErrorType {
	YET_None = 0,					///< 正常
	YET_Undefined = -1,				///< 未定义
	YET_NotImplemented = -2,			///< 未实现
	YET_NotSupported = -3,			///< 不支持
	YET_InvalidState = -4,			///< 无效状态
	YET_InvalidArgs = -5,			///< 无效参数

	//
	YET_SDKUninited = -10,			///< 未初始化
	YET_InvalidHandle = -11,			///< 无效句柄
	YET_NoMemory = -12,			///< 无内存可用
	YET_NoHandle = -13,			///< 无句柄可用

	YET_PreviewOpenBuffer = -14,		///< 预览打开缓冲区失败
	YET_PreviewOpenInput = -15,		///< 预览打开输入失败
	YET_PreviewOpenDisplay = -16,		///< 预览打开显示失败
	YET_PreviewRecoverBegin = -17,		///< 预览自动恢复开始
	YET_PreviewRecoverEnd = -18,			///< 预览自动恢复结束

	//
	YET_SocketOpen = -100,					///< Socket打开失败
	YET_SocketConn = -101,					///< Socket连接失败
	YET_SocketSend = -102,					///< Socket发送失败				
	YET_SocketRecv = -103,					///< Socket接收失败
	YET_SocketData = -104,					///< Socket数据有错

	//
	YET_FileOpen = -201,						///< 文件打开失败
	YET_FileWrite = -202,					///< 文件写失败
	YET_FileRead = -203,						///< 文件读失败
	YET_FileType = -204,						///< 文件类型有误
	YET_FileData = -205,						///< 文件数据有误

	//
	YET_FfmpegDecode = -301,				///< ffmpeg解码失败
	YET_FfmpegMuxOpen = -302,			///< ffmpeg muxer打开失败
	YET_FfmpegMuxWrite = -303,			///< ffmpeg muxer写入失败
	YET_FfmpegMuxClose = -304,			///< ffmpeg muxer关闭失败
};

/*
数据类型
*/
enum xxxdatatype {
	xxxdatatype_vol = 0,		///< 
	xxxdatatype_voldelta = 1,		///< 
	xxxdatatype_temp = 2,		///< 温度流
	xxxdatatype_video = 3,		///< 视频流
	xxxdatatype_rtsp = 4,		///< 视频流rtsp
	xxxdatatype_mix = 5,		///< 混合流
};

/*
数据文件类型
*/
enum xxxmediafile {
	xxxmediafile_bmpx = 0,			///< 单帧温度文件, bmp扩展
	xxxmediafile_pngx,				///< 单帧温度文件, png扩展
	xxxmediafile_stream,			///< 多帧温度文件
	xxxmediafile_mp4,				///< mp4文件
	xxxmediafile_jpgx,				///< 单帧温度文件, jpg扩展
};

/**
设备文件类型
*/
enum xxxcamerafile {
	xxxcamerafile_bin = 0,			///< 设备程序包
	xxxcamerafile_cali,				///< 校准表
	xxxcamerafile_com,				///< 
	xxxcamerafile_log,				///< 日志

	xxxcamerafile_cali2,			///< 校准表2
	xxxcamerafile_cali3,			///< 校准表3

	xxxcamerafile_bad,				///< 坏点表

	xxxcamerafile_c2m,				///< 二次多段校准
	xxxcamerafile_c2m2,				///< 二次多段校准2
};

enum xxxcameramem {
	xxxcameramem_measure = 0,
};

/**
调色板类型
*/
enum xxxpalette {
	xxxpalette_Autumn = 0,
	xxxpalette_BlackHot,
	xxxpalette_GlowBow,
	xxxpalette_HighContrast,
	xxxpalette_HotMetal,
	xxxpalette_IronBow,
	xxxpalette_Jet,
	xxxpalette_RainBow,
	xxxpalette_RedSaturation,
	xxxpalette_WhiteHot,
	xxxpalette_Winter,
	xxxpalette_IronBow2,
};

/**
红外调色板
*/
typedef struct _xxxpalettedata {
	char name[32];				///< 名称
	bgra data[256];				///< 数据
}xxxpalettedata;

/**
发现热像仪方式
*/
enum xxxdiscoverflags {
	xxxdiscoverflags_broadcast = 0x0001,		///< 广播
	xxxdiscoverflags_multicast = 0x0002,		///< 组播
	xxxdiscoverflags_broadcast2 = 0x0004,	///< 广播2
};

/**
*/
typedef struct _xxxfpavol4 {
	u16 gsk;
	u16 gfid;
	u16 vsk;
	u16 vbus;
}xxxfpavol4;

typedef struct _xxxfpavol4f {
	float gsk;
	float gfid;
	float vsk;
	float vbus;
}xxxfpavol4f;

enum xxxfpacint {
	xxxfpacint_0 = 0,
	xxxfpacint_1,
	xxxfpacint_2,
	xxxfpacint_3,
	xxxfpacint_4,
	xxxfpacint_5,
	xxxfpacint_6,
	xxxfpacint_7,
};

/**
测温结果
*/
typedef struct _xxxmeasure_result {
	float amin;			///< 最小值
	float amax;			///< 最大值
	float aavg;			///< 平均值
	s16 min;			///< 最小值, 整数
	s16 max;			///< 最大值, 整数
	s16 avg;			///< 平均值, 整数

	u16 max_x;			///< 最大值坐标x
	u16 max_y;			///< 最大值坐标y

	u16 min_x;			///< 最小值坐标x
	u16 min_y;			///< 最小值坐标y
}xxxmeasure_result;

/**
点
*/
typedef struct _xxxpoint {
	u16 x;			///< 坐标x
	u16 y;			///< 坐标y
	u16 radius;		///< 半径, 以中心(x,y), 边长(radius*2+1)的方块代表点
}xxxpoint;

/**
线
*/
typedef struct _xxxline {
	u16 x0;			///< 起点x
	u16 y0;			///< 起点y
	u16 x1;			///< 终点x
	u16 y1;			///< 终点y
	u16 radius;		///< 线宽, 未使用
}xxxline;

/**
矩形
*/
typedef struct _xxxrectangle {
	u16 xmin;			///< x最小
	u16 ymin;			///< y最小
	u16 xmax;			///< x最大
	u16 ymax;			///< y最大
}xxxrectangle;

/**
测温对象类型
*/
enum xxxmeasuretype {
	xxxmeasuretype_point = 0,
	xxxmeasuretype_line,
	xxxmeasuretype_rectangle,
	xxxmeasuretype_ellipse,
	xxxmeasuretype_polyline,
	xxxmeasuretype_polygon,
};

/**
测温标志
*/
enum xxxmeasureflags {
	xxxmeasureflags_track_max = 0x01,	///< 高温追踪
	xxxmeasureflags_track_min = 0x02,	///< 低温追踪

	xxxmeasureflags_plot_max = 0x08,	///< 高温曲线
	xxxmeasureflags_plot_min = 0x10,	///< 低温曲线
	xxxmeasureflags_plot_avg = 0x20		///< 平均温曲线
};

/**
报警类型
*/
enum xxxalarmtype {
	xxxalarmtype_none = 0,
	xxxalarmtype_max,
	xxxalarmtype_min,
	xxxalarmtype_mid,
	xxxalarmtype_exmid,
};

/**
测温对象
*/
typedef struct _xxxmeasure_object {
	char measure_id[16];				///< 名称
	u8 measure_type;					///< 类型
	u8 alarm_type;						///< 报警类型
	u8 pad[2];
	float alarm_min;				///< 报警最小值
	float alarm_max;				///< 报警最大值
	float emissivity;				///< 发射率

	union {
		xxxpoint point;			///< 点
		xxxline line;				///< 线
		xxxrectangle rectangle;	///< 矩形
	}geometry;							///< 几何对象

	u8 reserved[14];
}xxxmeasure_object;

/**
测温对象X
*/
typedef struct _xxxmeasure {
	u8	id;						///< 序号
	u8	measuretype;			///< 类型
	u8	points;					///< 点数
	u8	emmi;					///< 发射率, 单位0.01
	u16 xydata[64];				///< 点数据, x0,y0,x1,y1,...

	u8	alarmtype;				///< 报警类型
	u8	measureflags;			///< 测量标志
	u8	pad[2];
	float alarmdata[4];			///< 报警温度值t0,t1,t2

}xxxmeasure;

/**
测温结果X
*/
typedef struct _xxxresult {
	s32 alarmLevel;				///< 报警等级
	u16 maxX;					///< 最高温X
	u16 maxY;					///< 最高温Y

	u16 minX;					///< 最低温X
	u16 minY;					///< 最低温Y

	float max;					///< 最高温
	float min;					///< 最低温
	float avg;					///< 平均温
}xxxresult;

/**
屏蔽区X
*/
typedef struct _xxxshield {
	u8		id;					///< 序号			
	u8		pad[3];				///< 预留
	u16		xydata[8];			///< 点数据p0,p1,p2,p3
}xxxshield;

/**
串口数据位
*/
enum xxxspdatabits {
	xxxspdatabits_5 = 5,
	xxxspdatabits_6,
	xxxspdatabits_7,
	xxxspdatabits_8,
};

/**
串口校验位
*/
enum xxxspparity {
	xxxspparity_none = 0,
	xxxspparity_odd,
	xxxspparity_even,
};

/**
串口停止位
*/
enum xxxspstopbits {
	xxxspstopbits_1 = 0,
	xxxspstopbits_2,
};

/**
串口波特率
*/
enum xxxspbaudrate {
	xxxspbaudrate_2400 = 0,
	xxxspbaudrate_4800,
	xxxspbaudrate_9600,

	xxxspbaudrate_19200,
	xxxspbaudrate_38400,
	xxxspbaudrate_57600,
	xxxspbaudrate_115200,
};

/**
串口用法
*/
enum xxxspusage {
	xxxspusage_custom = 0,
	xxxspusage_pelcod,
	xxxspusage_pelcop
};

/**
GPIO输入类型
*/
enum xxxgpioinputtype {
	xxxgpioinputtype_none = 0,			///< 关闭
	xxxgpioinputtype_check,				///< 检查
	xxxgpioinputtype_capture,			///< 截图
};

/**
GPIO输出类型
*/
enum xxxgpiooutputtype {
	xxxgpiooutputtype_close = 0,		///< 关闭
	xxxgpiooutputtype_open,				///< 打开
};

/**
对焦区域
*/
typedef struct _xxxfocusrect {
	u16 xmin;		///< x最小
	u16 ymin;		///< y最小
	u16 xmax;		///< x最大
	u16 ymax;		///< y最大
}xxxfocusrect;

/**
GPIO报警类型
*/
enum xxxgpioalarmtype {
	xxxgpioalarmtype_manual = 0,		///< 手动
	xxxgpioalarmtype_auto				///< 自动
};

/**
GPIO报警值
*/
enum xxxgpioalarmvalue {
	xxxgpioalarmvalue_low = 0,			///< 低
	xxxgpioalarmvalue_high,				///< 高
	xxxgpioalarmvalue_ignore,			///< 无
};

/**
GPIO报警
*/
typedef struct _xxxgpioalarm {
	u8 value0;			///< 报警值0
	u8 type0;			///< 报警类型0
	u8 time0;			///< 报警时间0

	u8 value1;			///< 报警值1
	u8 type1;			///< 报警类型1
	u8 time1;			///< 报警时间1
}xxxgpioalarm;

/**
串口数据
*/
typedef struct _xxxserialportdata {
	u8 size;				///< 数据大小
	u8 data[31];			///< 数据区
}xxxserialportdata;

/**
时间
*/
typedef struct _xxxdatetime {
	s32 year;			///< 年
	s32 mon;			///< 月
	s32 day;			///< 日

	s32 hour;			///< 时
	s32 min;			///< 分
	s32 sec;			///< 秒
}xxxdatetime;


typedef struct _xf_config {
	short ms_full;//ms
	short fc_full;//fc
	short fc1;//fc, cc0
	short fc2;//fc, cc1000

	short fc_delta;//fc
	short fc_small;//fc, 300
	u8	pad[4];
}xf_config;

/**
温度转位图算法配置修改标志
*/
enum StrechControlFlags {
	SCF_StrechType = 0x0001,	///< 算法类型
	SCF_Contrast = 0x0002,		///< 对比度
	SCF_Brightness = 0x0004,	///< 亮度
	SCF_PHEGain = 0x0008,		///< PHE增益
	SCF_Linear = 0x0010,		///< LINEAR参数
	SCF_PHEStream = 0x0020,		///< PHE流控
	SCF_DDELevel = 0x0040,		///< DDE等级

	SCF_ColorTemp = 0x0080,		///< 温度标记
};

/**
温度转位图算法类型
*/
enum StrechType {
	StrechType_PHE = 1,			///< PHE
	StrechType_LINEAR,			///< LINEAR
};

/*
温度标记类型
*/
enum ColorTempType {
	ColorTempType_None,			///< 无
	ColorTempType_High,			///< 高温, 
	ColorTempType_Low,			///< 低温
	ColorTempType_Mid,			///< 区间内
	ColorTempType_ExMid,		///< 区间外
};

/**
温度转位图算法配置
*/
typedef struct _strech_control {
	s32 flags;						///< 标志

	u8 strech_type;					///< 算法类型
	u8 contrast;					///< 对比度
	u8 brightness;					///< 亮度
	u8 phe_stream;					///< PHE流控
	float phe_gain;					///< PHE增益

	s16	linear_temp[2];				///< 分段线性温度, 单位0.1C
	u8	linear_gray[2];				///< 分段线性灰度

	u8	dde_level;					///< DDE等级0-8, 0关闭DDE


	/*
	高温:	温度高于ct_temp0, 用ct_color0
	低温：	温度低于ct_temp0, 用ct_color0
	区间内： 温度介于[ct_temp0,ct_temp1], 用ct_color0
	区间外:	温度低于ct_temp0, 用ct_color0; 温度高于ct_temp1,用ct_color1
	*/
	u8 ct_type;						///< 温度标记类型
	s16 ct_temp0;					///< 温度标记温度0, 单位0.1C
	s16 ct_temp1;					///< 温度标记温度1, 单位0.1C
	s32 ct_color0;					///< 温度标记颜色0, bgr格式
	s32 ct_color1;					///< 温度标记颜色1, bgr格式
}strech_control;

#endif
