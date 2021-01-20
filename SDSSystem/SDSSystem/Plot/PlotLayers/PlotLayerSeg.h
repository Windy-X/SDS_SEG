/********************************************************************
* Copyright (C)  深圳市德力凯电子有限公司
* All rights reserved
*
* 文件名称：PlotLayerSeg.h
* 摘    要：
*		选段图层
* 历史记录：
*			Created <2020/9/2>	xiongz,
* ********************************************************************/
#pragma once
#include <QFont>
#include "PlotLayers/PlotLayerBase.h"
#include "Scale/PlotXMap.h"
#include "PublicDefs.h"
class CPlotLayerSeg : public CPlotLayerBase
{
public:
	 CPlotLayerSeg();
	virtual ~CPlotLayerSeg();
	//设置/获取起点
	void SetStartPos(const int &iStartPos);
	void SetStartPos(const QPoint &point);
	int GetStartIndex() const { return m_iStartIndex; }
	//设置/获取终点
	void SetEndPos(const int &iEndPos);
	void SetEndPos(const QPoint &point);
	int GetEndIndex() const { return m_iEndIndex; }

	//获取选段位置
	void GetSegIndex(int *pStart, int *pLen) const { (*pStart) = m_iStartIndex; (*pLen) = m_iEndIndex - m_iStartIndex; }

	//是否开始选段
	void SetTracking(bool bIsTracking);
	//是否标记事件
	void SetMarkEvent(const bool &bIsEventMode);
	//用户选段
	void DrawUserTrack(const QPoint& point);
	//更新图层绘制图层
	void UpdatePix(const QVector<SegmentMarks>& vctSegment,const SegmentView &stView,const CPlotXMap &cAxisX);
	//绘制底色
	void DrawBottom();
	//绘制事件标记线
	void DrawMarkEventLine(const QVector<StampedEvent> &vctMark,const CPlotXMap &cAxisX);
	//记录最后pixmap状态
	void StoreLastPixmap();
	//将最后的pixmap状态前进一档
	void RestoreLastPixmap();
	
	//===================绘制选段统计信息 <2020/11/18> xiongz==================
	//*****************************************************
	// Method：  	DrawSegmentMeasureInfo	
	// Purpose：	绘制选段测量信息
	// Access：    	public
	// Returns：   	void 		
	// Parameter：	
	//		stChannel		-[input] 每一导要显示的内容配置，字体，颜色，内容
	//		font			-[input] 字体
	//		rectChannel		-[input] 每一导的rect位置
	//*****************************************************
	void DrawSegmentMeasureInfo(QPainter * pp,const SegmentChannelMeasure &stChannel,const QFont &font,const QRect &rectChannel);
	//绘制标题
	void DrawSegmentMeasureInfoTitle(QPainter * pp,
		const QFont &font,
		const QColor &color,
		const int &iPulses,
		int iTime,
		const QRect &rectSeg);
	void UpdateMeasureInfoPix(const QVector<SegmentMarks>& vctSegment, const SegmentConfig &stSegConfig,const CPlotXMap &cAxisX);




private:
	QPixmap m_lastPix;			//记录最后选段状态的pixmap

	int m_iStartIndex;
	int m_iEndIndex;
	bool m_bTracking;
	bool m_bIsEventMode;


};

