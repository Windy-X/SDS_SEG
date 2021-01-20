/********************************************************************
* Copyright (C)  �����е������������޹�˾
* All rights reserved
*
* �ļ����ƣ�PlotLayerBottom.h
* ժ    Ҫ��
*		������ͼ��
* ��ʷ��¼��
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
	//���Ƶ�ɫ
	void DrawChannelBackground(const ChannelParam& stParam);
	//���ƿ�������
	void DrawChannelGrid(const ChannelParam& stParam);

	//���ƿ̶�ֵ
	void DrawChannelScale(const ChannelParam& stParam);

	//������������
	void DrawChannelLegends(const ChannelParam &stParam);

	//����Ԥ������
	void DrawResevedWidget(const ChannelParam &stParm);

};

