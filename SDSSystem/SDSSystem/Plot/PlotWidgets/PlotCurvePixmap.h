/********************************************************************
* Copyright (C)  深圳市德力凯电子有限公司
* All rights reserved
*
* 文件名称：PlotCurvePixmap.h
* 摘    要：完成曲线的绘制操作，将曲线绘制到曲线图层中
*		
* 历史记录：
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
	//绘制线性曲线
	void DrawLinePix(const LineCurveInfo &stCurveInfo, const  CPlotData &cData,const CPlotXMap &cAxisX, const CPlotYMap &cAxisY);
	//绘制线性曲线（不包含无效值）
	void DrawLineWithoutInvalid(QPainter &painter, const LineCurveInfo &stCurveInfo, const CPlotData &cData, const CPlotXMap &cAxisX,const CPlotYMap &cAxisY);
	//*****************************************************
	// Method：  	DrawValidLine	
	// Purpose：	绘制曲线轨迹（跳过无效数据）
	// Access：    	privete
	// Returns：   	void
	// Parameter：	
	//		painter			-[input] 绘图类对象
	//		stCurveInfo		-[input] 曲线属性
	//		pathUp			-[input] 曲线上包络轨迹
	//		pathDown		-[input] 曲线下包络轨迹
	//*****************************************************
	void DrawValidLine(QPainter &painter, const LineCurveInfo &stLineCurveInfo, const QPainterPath &pathUp, const QPainterPath &pathDownm );
private:
	QPixmap m_pixChannel;	//将曲线绘制到该pixmap;
	QPixmap	m_pixVacant;	//空白pixmap
};

