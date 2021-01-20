/********************************************************************
* Copyright (C) 深圳市德力凯医疗设备股份有限公司
* All rights reserved.
*
* 文件名称：PlotDefs.h
* 摘    要：
*		绘图基础定义
* 历史记录：
*		<2020/04/26> dingmh, Created
*********************************************************************/
#pragma once
#include <QColor>
#include <QString>
#include "PublicDefs.h"
#include "Scale/PlotYMap.h"

const double C_TP_HEIGHT_FACTOR = 0.5;

// 图层信息 -----------------------------------------------
enum E_LAYER_INDEX
{
	E_LAYER_BOTTOM = 0,		// 底层，用来绘制网格线，不重绘
	E_LAYER_CURVE,			// 曲线层
	E_LAYER_SEG,			// 选段层
	E_LAYER_MARK,			// 标记层
	E_LAYER_VALUE,			// 指数图层
	E_LAYER_TOP
};



// 每导参数，将一导总体配置导入，并根据实际尺寸和数据得出--
struct ChannelParam
{
	ChannelConfig stConfig;
	QRect rectLeftLegend;		// 左侧曲线名称区
	QRect rectPlot;				// 绘图区
	QRect rectReservedWidget;	//预留窗口区域
	QRect rectRightLegend;		// 右侧曲线名称区
	QRect rectIndex;			// 指数区
	CPlotYMap cAxisLeft;
	CPlotYMap cAxisRight;

	~ChannelParam()
	{

	}
};
                                                  