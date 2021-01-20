#pragma once
#include "PlotLayerBase.h"
#include "PublicDefs.h"
#include "PlotData/PlotData.h"
#include "Scale/PlotXMap.h"
class CPlotLayerCurve :
	public CPlotLayerBase
{
public:
	CPlotLayerCurve();
	virtual ~CPlotLayerCurve();
	//*****************************************************
	// Method：  	UpdatePix	
	// Purpose：	更新曲线图层
	// Access：    	public
	// Returns：	void
	// Parameter：	
	//		stParam		-[input] 每一导参数配置，包括绘图区域的设定，曲线信息
	//		mapPlot		-[input] 该导显示数据
	//		cAxisX		-[input] 时间轴
	//*****************************************************
	void UpdatePix(const ChannelParam &stParam,const QMap<E_DATA_TYPE,CPlotData> &mapPlot,const CPlotXMap &cAxisX);

};

