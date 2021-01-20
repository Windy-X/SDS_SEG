/********************************************************************
* Copyright (C)  深圳市德力凯电子有限公司
* All rights reserved
*
* 文件名称：PlotLayerBase.h
* 摘    要：
*		图层基类
* 历史记录：
*			copy <2020/8/14>	xiongz,
* ********************************************************************/
#pragma once
#include <QPixmap>
#include "PlotDefs.h"
class CPlotLayerBase
{
public:
	CPlotLayerBase(void);
	virtual ~CPlotLayerBase(void);

	//重新设置图层大小
	void ResizeLayer(const QSize &size);
	//初始化图层
	void InitLayer(const QSize &size);
	void InitLayer(const QSize &size, const QColor &clBackground);

	const QPixmap &GetPixmap() const { return m_Pix; }
	void SetVisible(const bool bVisible) { m_bVisible = bVisible; }
	bool IsVisible() const { return m_bVisible; }
	void Clear();
protected:
	QPixmap m_Pix;
	QPixmap m_Vacant;
	bool m_bVisible;

};

