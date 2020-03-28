/*
@file YoseenTypes.h
@author yoseen
@date 2018-04-01
*/
#ifndef YOSEENTYPES_H_
#define YOSEENTYPES_H_

#include "YoseenBasicTypes.h"

/*
设备基本信息
*/
enum TransformType {
	TransformType_None = 0,
	TransformType_Rotate90,
	TransformType_Rotate180,
	TransformType_Rotate270,

	//
	TransformType_MirrorX,
	TransformType_MirrorY,
};

typedef struct _CameraBasicInfo {
	char CameraId[32];				///< 序列号
	char CameraName[32];			///< 名称
	char CameraType[16];			///< 类型

	char FPAId[32];					///< 探测器序列号

	char HardwareId[32];			///< 硬件版本
	char SoftwareId[32];			///< 软件版本

	u16 DataWidth;			///< 数据宽度
	u16 DataHeight;			///< 数据高度

	/**数据帧率
	网络温度数据帧率=DataFps/(DataRatio*SendRatio)
	网络视频数据帧率=DataFps/DataRatio
	模拟视频帧率=DataFps/DataRatio
	*/
	u16 DataFps;

	u16 DataRatio;			///< 发送率, 设备每隔多少帧取一帧数据
	u16 SendRatio;			///< 数据率, 设备每隔多少帧发一帧数据

	u8 DataTransform;		///< 数据变换类型
	u8 pad[25];

	/*
	为保证视频上绘制的文字、测温对象、调色板清晰,
	温度数据分辨率低于640x480的图像放大到640x480, 再在此基础上绘制, H264编码, JPEG编码.
	*/
	u16 VideoWidth;			///< 视频输出宽度
	u16 VideoHeight;		///< 视频输出高度
}CameraBasicInfo;

/**
设备网络信息
*/
typedef struct _CameraNetworkInfo {
	bool UseStaticIp;				///< 是否使用静态IP
	u8 pad;
	u16 MulticastPort;				///< 组播端口, 未使用

	u32 StaticIp;					///< 静态IP
	u32 SubnetMask;					///< 子网掩码
	u32 Gateway;					///< 网关

	u32 MulticastIp;				///< 组播IP, 未使用

	u8 MacAddr[6];					///< MAC地址

	u8 pad2[2];
	u32 Dns;						///< 主DNS
	u32 Dns2;						///< 次DNS
	u8 reserved[16];
}CameraNetworkInfo;

/**
探测器信息
*/
typedef struct _FPAInfo {
	xxxfpavol4 vol4;
	u16 tint;
	u8  cint;
	u8 pad;
}FPAInfo;

typedef struct _FPAInfoHK160 {
	u8		data[80];	//40used

	u8		res64;	//res64
	u8		cint;
	u16		tint;
	u16		vtemp;	//140
	u8		bias;	//0-7v, 1-5v
	u8		pad[41];
}FPAInfoHK160;

/**
挡板校零信息
*/
typedef struct _FFCInfo {
	u8 frames_skipped_after_close;			///< 挡板关闭后忽略帧数
	u8 frames_accumlated_when_closed;		///< 挡板关闭后累积帧数
	u8 frames_skipped_after_open;			///< 挡板打开后忽略帧数
	u8 nst_enable;							///< 无挡片模式开关
	u16 time_interval;						///< 时间触发间隔, 单位帧数
	u16 temp_interval;						///< 温度触发间隔, 单位mK

	s8 af_rebound;							///< 自动对焦回弹, 单位帧数
	u8 fpa_shutter;							///< 有挡片1/(1C/60s), 内部参数
	s16 nst_delta;							///< 无挡片模式漂移
	u8 reserved[4];
}FFCInfo;

/**
测温修正信息
*/
typedef struct _FixInfo {
	float AtmosphericTemperature;		///< 气温
	float RelativeHumidity;				///< 相对湿度
	float Visibility;					///< 能见度
	float RainfallIntensity;			///< 降雨强度
	float SnowfallIntensity;			///< 降雪强度
	float TargetDistance;				///< 目标距离

	float GlobalEmissivity;				///< 全局发射率
	float InfraredWindowTrans;			///< 红外窗口透过率
	float TempOffset;					///< 温度漂移

	bool EnableAtmFix;					///< 是否开大气修正
	u8 reserved[35];
}FixInfo;

/**
测温对象信息
*/
typedef struct _MeasureInfo {
	s32					MOC;		///< 测温对象数量
	xxxmeasure_object	MOS[8];		///< 测温对象数组
}MeasureInfo;

/**
模拟视频信息
*/
enum XuiDisplayFlags {
	XDF_Palette = 0x0001,				///< 显示调色板
	XDF_TrackHigh = 0x0002,				///< 显示高温追踪
	XDF_TrackLow = 0x0004,				///< 显示低温追踪

	XDF_NoGlobalMax = 0x0008,			///< 不显示全局最高温
	XDF_NoGlobalMin = 0x0010,			///< 不显示全局最低温
	XDF_NoLocalMeasures = 0x0020,		///< 不显示局部测温对象
};

typedef struct _TvoutInfo {
	bool				EnableTvout;		///< 是否开启模拟视频
	u8					PaletteType;		///< 调色板类型
	u8					pad;
	u8					Contrast;			///< 对比度
	u8					Brightness;			///< 亮度
	u8					Zoom;				///< 数字变焦
	u16					XuiDisplayFlags;	///< 显示标志位, 参见XuiDisplayFlags

	float				Gain;				///< 增益
	u32					H264_Bitrate;		///< 流媒体比特率
	u16					H264_GopSize;		///< 流媒体图像组大小

	//add20180809
	u8					StrechType;			///< 算法类型
	u8					DDELevel;			///< DDE等级
	s16					LinearTemp[2];		///< 线性温度, 0.1C
	u8					LinearGray[2];		///< 线性灰度
	u8					HDMIPreset;

	/*
	add20191204
	高温:	温度高于ct_temp0, 用ct_color0
	低温：	温度低于ct_temp0, 用ct_color0
	区间内： 温度介于[ct_temp0,ct_temp1], 用ct_color0
	区间外:	温度低于ct_temp0, 用ct_color0; 温度高于ct_temp1,用ct_color1
	*/
	u8 CT_Type;								///< 温度标记类型
	s16 CT_Temp0;							///< 标记温度0, 单位0.1C
	s16 CT_Temp1;							///< 标记温度1, 单位0.1C
	s32 CT_Color0;							///< 标记颜色0
	s32 CT_Color1;							///< 标记颜色1
}TvoutInfo;

/**
串口信息
*/
typedef struct _SerialPortInfo {
	u8 Usage;					///< 用法
	u8 PortAddr;				///< 串口地址
	u8 BaudRate;				///< 波特率
	u8 DataBits;				///< 数据位
	u8 StopBits;				///< 停止位
	u8 Parity;					///< 校验位
	u8 reserved[10];
}SerialPortInfo;

/**
GPIO信息
*/
typedef struct _GpioInfo {
	u8 Input0;		///< 输入0
	u8 Input1;		///< 输入1
	u8 Output0;		///< 输出0
	u8 Output1;		///< 输出0
	u8 reserved[12];
}GpioInfo;

/**
设备时间信息
*/
typedef struct _CameraTimeInfo {
	char	NtpServer[32];			///< 主NTP服务器
	char	NtpServer2[32];			///< 次NTP服务器
	char	TimeZone[32];			///< 时区
	u8		reserved[32];
}CameraTimeInfo;

/*
设备验证信息
*/
typedef struct _AuthInfo {
	char	AccessKey[16];
	char	RtspUsername[16];
	char	RtspPassword[16];
	u8		reserved[16];
}AuthInfo;


/*
设备OSD信息
*/
typedef struct _CameraOSDInfo {
	u16 X1;				///< 坐标X1
	u16 Y1;				///< 坐标Y1
	char Text1[64];		///< 文本1, utf-8编码

	u16 X2;				///< 坐标X2
	u16 Y2;				///< 坐标Y2
	char Text2[64];		///< 文本2

	u16 X3;				///< 坐标X3
	u16 Y3;				///< 坐标Y3
	char Text3[64];		///< 文本3

	u16 X4;				///< 坐标X4
	u16 Y4;				///< 坐标Y4
	char Text4[64];		///< 文本4
}CameraOSDInfo;

/*
双光混合参数
*/
typedef struct _CameraOutputInfo {
	//mix
	u8 MixMode;					///< 混合模式
	u8 MixRatio;				///< 混合比例, 取值范围[1,8]
	u16 CropX;					///< 可见光裁剪X
	u16 CropY;					///< 可见光裁剪Y
	u16 CropWidth;				///< 可见光裁剪宽度
	u16 CropHeight;				///< 可见光裁剪高度

	//
	u8					StrechType;				///< 红外算法类型
	u8					DDELevel;				///< 红外DDE等级, 取值范围[0,8], 0代表关闭
	float				Gain;					///< 红外图像增益
	s16					LinearTemp[2];			///< 红外线性分段温度, 单位: 0.1度
	u8					LinearGray[2];			///< 红外线性分段灰度
	u8					PaletteType;			///< 红外调色板
	u8					Contrast;				///< 红外对比度, 默认50
	u8					Brightness;				///< 红外亮度, 默认50

	//
	u8					EnableHdmi;				///< 是否开启模拟视频
	u16					H264_GopSize;			///< 流媒体图像组大小
	u32					H264_Bitrate;			///< 流媒体码率, 单位: bps

	//
	u8					FlipIR;					///< 红外翻转
	u8					FlipVIS;				///< 可见光翻转
	u16					XStartVIS;				///< 可见光视场X, 内部参数
	u16					YStartVIS;				///< 可见光视场Y, 内部参数
	u8					reserved[10];
}CameraOutputInfo;

/*
混合模式
*/
enum MixMode {
	MixMode_IR = 0,		///< 单红外
	MixMode_VIS,		///< 单可见
	MixMode_DSX,		///< 双波段
	MixMode_BLEND,		///< alpha混合
	MixMode_PIP,		///< 画中画
	MixMode_FUSION,		///< 

	//
	MixMode_MAX,
};

/*
翻转类型
*/
enum MixFlip {
	MixFlip_NN = 0,		///< 无翻转
	MixFlip_XN,			///< X翻转
	MixFlip_NY,			///< Y翻转
	MixFlip_XY,			///< XY翻转
};

/*
温度帧头
*/
typedef struct _DataFrameHeader {
	u16 Width;						///< 宽度
	u16 Height;						///< 高度
	u32 ComSize;					///< 压缩大小
	u8 DataType;					///< 数据类型
	u8 ComType;						///< 压缩类型
	u16 Index;						///< 帧索引

	/**
	全帧温度以16位有符号整数数组表示, 温度浮点值=温度整数值/Slope+Offset
	*/
	u16 Slope;
	s16 Offset;

	s32 FPATemp;					///< 探测器温度, 内部使用
	s32 ShellTemp;					///< 等效机壳温度, 内部使用

	u8 pad;
	u8 GpioInput0;					///< GPIO输入0
	u8 GpioInput1;					///< GPIO输入1
	u8 pad2[5];
	s64 Timestamp;					///< 时间戳, 单位100ns
	u8 reserved[88];
}DataFrameHeader;
#define DATA_FRAME_HEADER_SIZE					128
#define DataFrameHeader_Offset_Timestamp				32


/**
YP
*/
#define YPCMD_DiscPort					55501
#define YPCMD_Port						55502
#define YPDATA_Port						55503

#define YPCMD_DiscMulticastPort			55504
#define YPCMD_DiscMulticastAddr			"239.255.255.251"

#define YPDATATM_Port					55505
#define YPDATAVM_Port					55506
#define YPDATAXM_Port					55507

#define YPCMD_DiscClientPort			55511

/**
控制类型
*/
enum CtlType {
	CtlType_Debug = 0,				///< 
	CtlType_ChangeDataType,			///< 
	CtlType_MoveShutter,			///< 
	CtlType_StoreFactory,			///< 
	CtlType_RestartCamera,			///< 重启设备


	CtlType_ManualFFC = 32,			///< 手动挡板校零
	CtlType_RestoreFactory,			///< 恢复出厂配置
	CtlType_FocusManual,			///< 手动对焦, 步进模式控制电动镜头电机
	CtlType_FocusAuto,				///< 自动对焦
	CtlType_SetAlarm,				///< 设置GPIO报警
	CtlType_DisableFFC,				///< 禁用挡板校零
	CtlType_FocusManual2,			///< 手动对焦, 启停模式控制电动镜头电机
};

/**
控制
*/
typedef struct _Ctl {
	u16 Type;					///< 类型
	union {
		u8 reserved[8];				///< 联合体大小
		u8 DataType;				///< 温度数据类型
		u8 ShutterState;			///< 0挡板开, 1挡板关
		s16 FocusDelta;				///< 步进模式, 电动镜头电机转动时间, 单位5ms, 正数-远焦, 负数-近焦
		xxxfocusrect FocusRect;		///< 自动对焦区域
		u8	FocusType;				///< 启停模式, 电动镜头电机转动方式, 0 停止, 1 远焦转动, 2 近焦转动
		xxxgpioalarm Alarm;			///< GPIO报警
		u8 DisableFFC;				///< 1禁用, 0启用
	}Data;
}Ctl;

/**
控制X类型
*/
enum CtlXType {
	CtlXType_GetTime = 0,	///< 获取设备时间, UTC
	CtlXType_SetTime,		///< 设置设备仪时间, UTC
	CtlXType_GetGear,		///< 获取测温档位
	CtlXType_SetGear,		///< 设置测温档位
	CtlXType_AutoFocus,		///< 自动对焦

	//add20180921
	CtlXType_XfGetConfig,	///< 位置记忆电动镜头, 获取配置
	CtlXType_XfSetConfig,	///< 位置记忆电动镜头, 设置配置,
	CtlXType_XfGetCur,		///< 位置记忆电动镜头, 获取位置
	CtlXType_XfMove,		///< 位置记忆电动镜头, 设置位置
	//CtlXType_XfReset,
	//CtlXType_XfCheckDelta,

	//
	CtlXType_SetGpioOutput = 128,
};

/*
GPIO输出标志
*/
enum xxxgpiooutputflags {
	xxxgpiooutputflags_light = (1 << 0),
	xxxgpiooutputflags_ledy = (1 << 1),
	xxxgpiooutputflags_ledb = (1 << 2),
	xxxgpiooutputflags_alarm = (1 << 3),
	xxxgpiooutputflags_buzz = (1 << 4),
};

/*
GPIO输出
*/
typedef struct _xxxgpiooutput {
	s32 flags;
	s32 value;
}xxxgpiooutput;

/**
控制操作
*/
typedef struct _CtlX {
	s32 Type;						///< 类型
	union {
		u8				Reserved[64];
		s32				Error;			///< 错误码
		xxxdatetime		Time;			///< 设备时间
		s32				GearLevel;		///< 测温档位
		xxxfocusrect	FocusRect;		///< 自动对焦区域
		xf_config	XfConfig;			///< 位置记忆电动镜头, 配置
		s32			XfMove;				///< 位置记忆电动镜头, 位置-刻度值, [0-100]
		s32			XfCurType;			///< 位置记忆电动镜头, 0-计数值, 1-刻度值
		xxxgpiooutput	GpioOutput;
	}Data;
}CtlX;

/**
发现热像仪响应2
*/
typedef struct _DiscoverCameraResp2 {
	CameraBasicInfo BasicInfo;				///< 基本信息
	u32 CameraIp;							///< 设备IP
	struct _DiscoverCameraResp2* pNext;		///< 下一条响应
}DiscoverCameraResp2;


/*
登入信息
*/
typedef struct _YoseenLoginInfo {
	char CameraAddr[128];			///< 设备地址
	char Username[32];				///< 用户名
	char Password[32];				///< 密码
}YoseenLoginInfo;

/*
H264起始帧
*/
typedef struct _H264FrameStart {
	u32 EncDataSize;				///< 编码器数据大小
	u16 DataWidth;						///< 数据宽度
	u16 DataHeight;						///< 数据高度
	u8 EncData[120];				///< 编码器数据

	u16 FpsNum;						///< 帧率分子
	u16 FpsDen;						///< 帧率分母
	u16 VideoWidth;
	u16 VideoHeight;
	u8 Reserved[120];
}H264FrameStart;
#define H264FrameStart_Size				256

/*
H264帧测温结果
*/
typedef struct _H264_MeasureResult {
	s8	Index;				//序号; 0:全局测温结果; 1:中心测温结果; [2-9]:局部测温结果; x无效测温结果
	s8	Pad[3];				//

	float Max;				//最高温
	float Min;				//最低温
	float Avg;				//平均温

	u16 MaxX;				//最高温点坐标X
	u16 MaxY;				//最高温点坐标Y
	u16 MinX;				//最低温点坐标X
	u16 MinY;				//最低温点坐标Y
}H264_MeasureResult;

/*
H264帧头, 视频流数据单帧帧头
*/
typedef struct _H264FrameHeader {
	s32 Size;						//帧大小
	u16 Width;						//数据宽度
	u16 Height;						//数据高度

	float FPATemp;					//探测器温度

	H264_MeasureResult	GlobalResult;			//全局测温结果
	H264_MeasureResult	CenterResult;			//中心测温结果
	H264_MeasureResult LocalResults[8];			//局部测温结果

	u8 Reserved[772];
}H264FrameHeader;
#define H264FrameHeader_Size			1024

/*
H264帧头V2, 混合流数据单帧帧头
*/
typedef struct _H264FrameHeaderV2 {
	s32 Size;						//帧大小
	u16 Width;						//数据宽度
	u16 Height;						//数据高度

	s32 ShellTemp;					//设备工作温度

	s32 LocalResultFlags;				//局部测温结果标志
	xxxresult GlobalResult;				//全局测温结果
	xxxresult LocalResultArray[16];		//局部测温结果

	u8 Reserved[668];
}H264FrameHeaderV2;

/*
数据帧
*/
typedef struct _DataFrame {
	void* Head;					///< 温度数据帧头
	void* Temp;					///< 温度数据
	void* Com;					///< 温度数据压缩
	void* Bmp;					///< 位图数据
	void* H264;					///< H264帧头
	struct _DataFrame* Prev;	///< 前一帧
}DataFrame;

/*
预览回调
@param errorCode 错误码
@param dataFrame 数据帧
@param customData 用户数据
*/
typedef void(__stdcall* YoseenPreviewCallback)(s32 errorCode, DataFrame* dataFrame, void* customData);

/*
预览信息
*/
typedef struct _YoseenPreviewInfo {
	s32 DataType;											///< xxxdatatype_video 视频流, xxxdatatype_temp 温度流
	void* Hwnd;												///< 显示窗口句柄, 可以为空
	YoseenPreviewCallback CustomCallback;					///< 预览回调, 可以为空
	void* CustomData;										///< 用户数据

	/*
	用户不用设置, 预览成功后, 用户据此调整显示窗口宽高比
	*/
	u16 OutputWidth;							///< 显示画面宽度
	u16 OutputHeight;							///< 显示画面高度
}YoseenPreviewInfo;

/*
*/
typedef struct _YoseenRtspInfo {
	char Url[256];

	u16 CropX;
	u16 CropY;
	u16 CropWidth;
	u16 CropHeight;

	//
	u16 DataWidth;
	u16 DataHeight;
}YoseenRtspInfo;

#endif