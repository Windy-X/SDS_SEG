/********************************************************************
* Copyright (C)  深圳市德力凯电子有限公司
* All rights reserved
*
* 文件名称：PlotLayerFactory.h
* 摘    要：
*		简单工厂，创建图层
* 历史记录：
*		Copy	<2020/8/17>	xiongz,
* ********************************************************************/
#pragma once
#include "PlotLayerBase.h"
#include "PlotLayerBottom.h"
#include "PlotLayerCurve.h"
class CPlotLayerFactory
{
public:
	CPlotLayerFactory(void);
	~CPlotLayerFactory(void);
	//创建图层对象
	CPlotLayerBase *CreatLayer(const E_LAYER_INDEX,const QSize &size);

};

