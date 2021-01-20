/********************************************************************
* Copyright (C)  �����е������������޹�˾
* All rights reserved
*
* �ļ����ƣ�PlotCurvePixmap.h
* ժ    Ҫ��������ߵĻ��Ʋ����������߻��Ƶ�����ͼ����
*		
* ��ʷ��¼��
*			copy <2020/8/21>	xiongz,
* ********************************************************************/
#pragma once
#include <QPixmap>
#include "Scale/PlotXMap.h"
#include "Scale/PlotYMap.h"
#include "PlotData/PlotData.h"
class CPlotCurvePixmap
{
public:
	CPlotCurvePixmap();
	CPlotCurvePixmap(const QSize &size);
	~CPlotCurvePixmap();
	void InitPixmap(const QSize &size);
	void ResizePixmap(const QSize &size);
	void UpdatePix(const LineCurveInfo &stCurveInfo, const CPlotData &cData,const CPlotXMap &cAxisX, const CPlotYMap &cAxisLeft, const CPlotYMap &cAxisRight);
	QPixmap GetPixmap() const { return m_pixChannel; }
private:
	//������������
	void DrawLinePix(const LineCurveInfo &stCurveInfo, const  CPlotData &cData,const CPlotXMap &cAxisX, const CPlotYMap &cAxisY);
	//�����������ߣ���������Чֵ��
	void DrawLineWithoutInvalid(QPainter &painter, const LineCurveInfo &stCurveInfo, const CPlotData &cData, const CPlotXMap &cAxisX,const CPlotYMap &cAxisY);
	//*****************************************************
	// Method��  	DrawValidLine	
	// Purpose��	�������߹켣��������Ч���ݣ�
	// Access��    	privete
	// Returns��   	void
	// Parameter��	
	//		painter			-[input] ��ͼ�����
	//		stCurveInfo		-[input] ��������
	//		pathUp			-[input] �����ϰ���켣
	//		pathDown		-[input] �����°���켣
	//*****************************************************
	void DrawValidLine(QPainter &painter, const LineCurveInfo &stLineCurveInfo, const QPainterPath &pathUp, const QPainterPath &pathDownm );
private:
	QPixmap m_pixChannel;	//�����߻��Ƶ���pixmap;
	QPixmap	m_pixVacant;	//�հ�pixmap
};

