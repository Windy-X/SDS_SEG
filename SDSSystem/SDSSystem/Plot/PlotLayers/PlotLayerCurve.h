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
	// Method��  	UpdatePix	
	// Purpose��	��������ͼ��
	// Access��    	public
	// Returns��	void
	// Parameter��	
	//		stParam		-[input] ÿһ���������ã�������ͼ������趨��������Ϣ
	//		mapPlot		-[input] �õ���ʾ����
	//		cAxisX		-[input] ʱ����
	//*****************************************************
	void UpdatePix(const ChannelParam &stParam,const QMap<E_DATA_TYPE,CPlotData> &mapPlot,const CPlotXMap &cAxisX);

};

