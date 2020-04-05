/*
@file YoseenPlayback.h
@author yoseen
@date 2018-04-01
*/
#ifndef YOSEENPLAYBACK_H_
#define YOSEENPLAYBACK_H_

#include "YoseenTypes.h"
#include "YoseenFile.h"

struct _YoseenPlaybackContext;
typedef struct _YoseenPlaybackContext YoseenPlaybackContext;

extern "C"{
	/*
	建立回放上下文
	@return 回放上下文
	*/
	SDK_API YoseenPlaybackContext* YoseenPlayback_Create();

	/*
	释放回放上下文
	@param pp 回放上下文
	*/
	SDK_API void YoseenPlayback_Free(YoseenPlaybackContext** pp);

	/*
	设置回放图像算法参数
	@param ctx 回放上下文
	@param sc 算法参数
	@param paletteType 调色板类型
	*/
	SDK_API void YoseenPlayback_GetImage(YoseenPlaybackContext* ctx, strech_control* sc, s32* paletteType);
	
	/*
	获取回放图像算法参数
	@param ctx 回放上下文
	@param sc 算法参数
	@param paletteType 调色板类型
	*/
	SDK_API void YoseenPlayback_SetImage(YoseenPlaybackContext* ctx, const strech_control* sc, const s32* paletteType);

	/*
	打开文件
	@param ctx 回放上下文
	@param fileName 文件名
	@param fileType 文件类型
	@return 错误码
	*/
	SDK_API s32 YoseenPlayback_OpenFile(YoseenPlaybackContext* ctx, const char* fileName, s32 fileType);

	/*
	关闭文件
	@param ctx 回放上下文
	*/
	SDK_API void YoseenPlayback_CloseFile(YoseenPlaybackContext* ctx);

	/*
	保存文件, 只支持单帧文件jpg
	@param ctx 回放上下文
	@param tff 封面数据
	@param fileName 文件名
	@return 错误码
	*/
	SDK_API s32 YoseenPlayback_SaveFile(YoseenPlaybackContext* ctx, TempFrameFile* tff, const char* fileName);


	/*
	读取封面数据,
	@param ctx 回放上下文
	@param tff 封面数据
	@return 错误码
	*/
	SDK_API s32 YoseenPlayback_ReadCover(YoseenPlaybackContext* ctx, TempFrameFile* tff);

	/*
	读取温度数据, 每次读取一帧, 位图数据重新生成.
	@param ctx 回放上下文
	@param frameIndex 帧序号
	@param dataFrame 数据帧
	@return 错误码
	*/
	SDK_API s32 YoseenPlayback_ReadFrame(YoseenPlaybackContext* ctx, s32 frameIndex, DataFrame* dataFrame);
}



#endif