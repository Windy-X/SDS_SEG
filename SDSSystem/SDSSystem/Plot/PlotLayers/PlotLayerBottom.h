/********************************************************************
* Copyright (C)  深圳市德力凯电子有限公司
* All rights reserved
*
* 文件名称：PlotLayerBottom.h
* 摘    要：
*		网格线图层
* 历史记录：
*			Copy <2020/8/14>	xiongz,
* ********************************************************************/
#pragma once
#include "PlotLayerBase.h"
#include "PublicDefs.h"
class CPlotLayerBottom :
	public CPlotLayerBase
{
public:
	CPlotLayerBottom(void);
	virtual ~CPlotLayerBottom(void);
	void UpdatePix(const ChannelParam &sttParams);
private:
	//绘制底色
	void DrawChannelBackground(const ChannelParam& stParam);
	//绘制刻网格线
	void DrawChannelGrid(const ChannelParam& stParam);

	//绘制刻度值
	void DrawChannelScale(const ChannelParam& stParam);

	//绘制曲线名称
	void DrawChannelLegends(const ChannelParam &stParam);

	//绘制预留窗口
	void DrawResevedWidget(const ChannelParam &stParm);

};

