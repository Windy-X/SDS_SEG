/********************************************************************
* Copyright (C) �����е�����ҽ���豸�ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�PlotDefs.h
* ժ    Ҫ��
*		��ͼ��������
* ��ʷ��¼��
*		<2020/04/26> dingmh, Created
*********************************************************************/
#pragma once
#include <QColor>
#include <QString>
#include "PublicDefs.h"
#include "Scale/PlotYMap.h"

const double C_TP_HEIGHT_FACTOR = 0.5;

// ͼ����Ϣ -----------------------------------------------
enum E_LAYER_INDEX
{
	E_LAYER_BOTTOM = 0,		// �ײ㣬�������������ߣ����ػ�
	E_LAYER_CURVE,			// ���߲�
	E_LAYER_SEG,			// ѡ�β�
	E_LAYER_MARK,			// ��ǲ�
	E_LAYER_VALUE,			// ָ��ͼ��
	E_LAYER_TOP
};



// ÿ����������һ���������õ��룬������ʵ�ʳߴ�����ݵó�--
struct ChannelParam
{
	ChannelConfig stConfig;
	QRect rectLeftLegend;		// �������������
	QRect rectPlot;				// ��ͼ��
	QRect rectReservedWidget;	//Ԥ����������
	QRect rectRightLegend;		// �Ҳ�����������
	QRect rectIndex;			// ָ����
	CPlotYMap cAxisLeft;
	CPlotYMap cAxisRight;

	~ChannelParam()
	{

	}
};
                                                  