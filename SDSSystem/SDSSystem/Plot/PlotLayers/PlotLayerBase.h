/********************************************************************
* Copyright (C)  �����е������������޹�˾
* All rights reserved
*
* �ļ����ƣ�PlotLayerBase.h
* ժ    Ҫ��
*		ͼ�����
* ��ʷ��¼��
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

	//��������ͼ���С
	void ResizeLayer(const QSize &size);
	//��ʼ��ͼ��
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

