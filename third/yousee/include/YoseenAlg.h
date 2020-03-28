/*
@file YoseenAlg.h
@author yoseen
@date 2018-04-01
*/
#ifndef YOSEENALG_H_
#define YOSEENALG_H_

#include "YoseenTypes.h"


/**
测温模板头
*/
typedef struct _MeasureTemplateHead {
	s32		Version;						///< 版本
	u16		Width;							///< 宽度
	u16		Height;							///< 高度
	s32		ShieldFlags;					///< 全局屏蔽区标志
	s32		LMOFlags;						///< 局部测温对象标志
	u8		pad[112];					///< 预留
}MeasureTemplateHead;


/**
测温模板
*/
typedef struct _MeasureTemplate {
	MeasureTemplateHead		Head;					///< 模板头

	xxxshield				ShieldArray[8];			///< 全局屏蔽区, 0-8个
	xxxmeasure				GMO;					///< 全局测温对象
	xxxmeasure				LMOArray[16];			///< 局部测温对象, 0-16个
}MeasureTemplate;

/**
模板结果
*/
typedef struct _MeasureTemplateResult {
	xxxresult			GMR;				///< 全局结果
	xxxresult			LMRArray[16];		///< 局部结果
}MeasureTemplateResult;

struct _MTContext;
typedef struct _MTContext MTContext;

extern "C"{

	/**
	获取测温结果, 支持点、折线、多边形、线、矩形、椭圆
	@param measure 测温对象
	@param dfh 温度数据帧头
	@param dfd 温度数据区
	@param result 测温结果
	@return 错误码
	*/
	SDK_API s32 mtGetResult(xxxmeasure* measure, DataFrameHeader* dfh, s16* dfd, xxxresult* result);

	/**
	建立测温模板上下文
	@param width 数据宽度
	@param height 数据高度
	@return 测温模板上下文
	*/
	SDK_API MTContext* mtCreate(s32 width, s32 height);

	/**
	释放测温模板上下文
	@param pp 温模板上下文
	*/
	SDK_API void mtFree(MTContext** pp);

	/**
	重载测温模板上下文
	@param context 上下文
	@param templ 测温模板
	@return 0成功,x失败
	*/
	SDK_API s32 mtReload(MTContext* context, MeasureTemplate* templ);


	/**
	获取测温模板的测量结果
	@param context 上下文
	@param dfh 温度数据帧头
	@param dfd 温度数据区
	@param result 测量结果
	@return 0成功,x失败
	*/
	SDK_API s32 mtCheck(MTContext* context, DataFrameHeader* dfh, s16* dfd, MeasureTemplateResult* result);

	/**
	测量点
	@param point 点
	@param dfh 温度数据帧头
	@param dfd 温度数据区
	@param [out] result 测量结果
	*/
	SDK_API void Alg_MeasurePoint(const xxxpoint* point, DataFrameHeader* dfh, s16* dfd, xxxmeasure_result* result);

	/**
	测量线
	@param line 线
	@param dfh 温度数据帧头
	@param dfd 温度数据区
	@param [out] result 测量结果
	*/
	SDK_API void Alg_MeasureLine(const xxxline* line, DataFrameHeader* dfh, s16* dfd, xxxmeasure_result* result);

	/**
	测量矩形
	@param rectangle 矩形
	@param dfh 温度数据帧头
	@param dfd 温度数据区
	@param [out] result 测量结果
	*/
	SDK_API void Alg_MeasureRectangle(const xxxrectangle* rectangle, DataFrameHeader* dfh, s16* dfd, xxxmeasure_result* result);

	/**
	测量椭圆
	@param ellipse 椭圆外接矩形
	@param dfh 温度数据帧头
	@param dfd 温度数据区
	@param [out] result 测量结果
	*/
	SDK_API void Alg_MeasureEllipse(const xxxrectangle* ellipse, DataFrameHeader* dfh, s16* dfd, xxxmeasure_result* result);


	/**
	分配调色板数据

	@param count 调色板数量
	@return 调色板数据, 内存布局
	======================================
	调色板0名称, 调色板0数据; 调色板1名称, 调色板1数据; ...
	======================================
	*/
	SDK_API xxxpalettedata* Yoseen_AllocPaletteData(s32& count);

	/**
	释放调色板数据
	@param paletteData 调色板数据
	*/
	SDK_API void Yoseen_FreePaletteData(xxxpalettedata** paletteData);


	/*
	放大温度数据, 双线性插值方式
	@param scale 放大倍数, 支持2,3,4倍放大
	@param srcWidth 源宽度
	@param srcHeight 源高度
	@param srcData 源数据
	@param dstData 目的数据
	@return 错误码
	*/
	SDK_API s32 Alg_ScaleTemp(s32 scale, u16 srcWidth, u16 srcHeight, s16* srcData, s16* dstData);
}

#endif