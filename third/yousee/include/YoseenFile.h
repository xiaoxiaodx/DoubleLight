/*
@file YoseenFile.h
@author yoseen
@date 2018-04-01
*/
#ifndef YOSEENFILE_H_
#define YOSEENFILE_H_

#include "YoseenTypes.h"

#pragma pack(push, 1)
typedef struct _xxxbmp_file_header{
	u16 bfType;
	u32 bfSize;
	u16 bfReserved1;
	u16 bfReserved2;
	u32 bfOffBits;
} xxxbmp_file_header;

typedef struct _xxxbmp_info_header{
	u32 biSize;
	s32 biWidth;
	s32 biHeight;
	u16 biPlanes;
	u16 biBitCount;
	u32 biCompression;
	u32 biSizeImage;
	s32 biXPelsPerMeter;
	s32 biYPelsPerMeter;
	u32 biClrUsed;
	u32 biClrImportant;
} xxxbmp_info_header;

/**
bmp文件头
*/
typedef struct _bmp_file_header{
	xxxbmp_file_header bfHeader;
	xxxbmp_info_header biHeader;
	u8 pad[2];
	u32 version;
	u8 reserved[4];
}bmp_file_header;


/**
帧文件头
*/
typedef struct _frame_file_header{
	char camera_id[32];			///< 热像仪序列号
	s64 captured_time;			///< 拍摄时间

	u16 width;					///< 数据宽度
	u16 height;					///< 数据高度

	FixInfo fix_info;			///< 修正信息

	u8 palette;					///< 调色板
	u8 reserved[11];
}frame_file_header;


/**
流文件头
*/
typedef struct _stream_file_header{
	char camera_id[32];			///< 热像仪序列号
	s64 captured_time;			///< 拍摄时间
	u16 width;					///< 数据宽度
	u16 height;					///< 数据高度

	FixInfo fix_info;			///< 修正信息

	u16 fps_num;				///< 帧率分子
	u16 fps_den;				///< 帧率分母
	u32 frame_count;			///< 帧数

	u8 palette;					///< 调色板
	u8 pad;
	s16 record_ratio;			///< 录制率, >0 每隔多少帧保存一帧; <0 每隔多少秒保存一帧
}stream_file_header;


/*
bmp
*/
#define Bmp_FileHeaderSize						64
#define Bmp_FileSize(width,height)				(Bmp_FileHeaderSize+width*height*4+1024+128+128+width*height*2)


/*
* png
*/
#define Png_FileVersion					20160901
#define Png_TempChunkType				(0x704d6574)//teMp
#define Png_TempChunkSize(pixels)			(4+4+128+16 * 1024+128+pixels*2+32+4)
#define Png_TempChunkDataSize(pixels)		(128+16 * 1024+128+pixels*2+32)

#define Png_VisiChunkType				(0x69536976)//viSi

/**
png温度数据块teMp
内存布局
===========================================
length		4
type		4
data		ffh+16k+dfh+dfd+pth
crc			4
===========================================
*/
struct png_temp_chunk{
	s32		length;					///< 长度
	s32		type;					///< 类型
	frame_file_header ffh;			///< 帧文件头
	u8		custom[16 * 1024];				///< 用户数据	
	DataFrameHeader dfh;			///< 温度数据帧头
};

/**
png温度数据头
*/
struct png_temp_header{
	u16 width;				///< 宽度
	u16 height;				///< 高度
	s32 version;			///< 版本
	u8 reserved[24];
};

/*
png可见光数据块viSi
内存布局
===========================================
length		4
type		4
data		visiPngFile+pvh
crc			4
===========================================
*/

/**
png可见光数据头
*/
struct png_visi_header{
	u16 width;				///< 宽度
	u16 height;				///< 高度
	s32 size;				///< 可见光数据大小
	u8 reserved[24];
};


/**
温度文件数据
*/
typedef struct _TempFrameFile{
	frame_file_header* ffh;				///< 帧文件头或流文件头, 128
	u8* custom;							///< 帧文件用户区, 16k
	DataFrameHeader* dfh;				///< 温度帧头, 128
	s16* dfd;							///< 温度帧数据区, pixels*2

	//
	u8* visiImage;						///< 保留
	u8* coverImage;						///< 封面数据, 标准bmp或jpg文件的全部字节
	s32 visiImageSize;					///< 保留
	s32 coverImageSize;					///< 封面数据大小
}TempFrameFile;

#pragma pack(pop)

#endif