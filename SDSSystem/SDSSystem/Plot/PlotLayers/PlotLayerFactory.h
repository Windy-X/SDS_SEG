/********************************************************************
* Copyright (C)  �����е������������޹�˾
* All rights reserved
*
* �ļ����ƣ�PlotLayerFactory.h
* ժ    Ҫ��
*		�򵥹���������ͼ��
* ��ʷ��¼��
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
	//����ͼ�����
	CPlotLayerBase *CreatLayer(const E_LAYER_INDEX,const QSize &size);

};

