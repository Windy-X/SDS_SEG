/********************************************************************
* Copyright (C)  深圳市德力凯电子有限公司
* All rights reserved
*
* 文件名称：PlotSegmentData.h
* 摘    要：选段，事件标记数据管理
*		
* 历史记录：
*			Modify <2020/10/16>	xiongz,
* ********************************************************************/
#pragma once
#include <QObject>
#include <QMutex>
#include <QVector>
#include "Scale/PlotXMap.h"
#include "PublicDefs.h"
#include "CSegmentData.h"
class CPlotSegmentData
{
	
public:
	static CPlotSegmentData* GetInstance();
	static void ReleaseInstance();
	//清除资源
	void Clear();
	//初始化，设置页面宽度
	void Init(const qint64& iWidth);
	void Init();
	////设置/读取页面宽度
	//void SetPixWidth(const qint64&);
	qint64 GetPixWidth() const { return m_cAxisX.GetPixWidth(); }
	////设置每一页的数据长度
	//void SetPageDataLen(const qint64&);
	qint64 GetPageDataLen() const { return m_cAxisX.GetDataWidth(); }
	////设置/读取当前页起始索引
	//void SetBegCol(const qint64 &iBegCol) { m_iBegCol = iBegCol; }
	qint64 GetBegCol() const { return m_cAxisX.GetBegCol(); }
	qint64 GetCurCol() const { return m_cAxisX.GetCurCol(); }
	qint64 GetCurIndex() const { return m_cAxisX.GetCurIndex(); }
	qint64 GetBegIndex() const { return m_cAxisX.GetCurIndex(); }
	//获取压缩比
	double GetPageRatio() const { double f_ratio = double(m_cAxisX.GetDataWidth()) / m_cAxisX.GetPixWidth(); return f_ratio; }
	//添加选段记录
	void AppendSeg(const SegmentMarks& stSeg);
	void AppendSeg(const QVector<SegmentMarks>& vctSeg);

	//删除选段记录
	void DeleteSeg(const qint64 &iIndex);

	//*****************************************************
	// Method：  	AddSegment	
	// Purpose：	在选段控件中进行选段，将控件中的坐标转化为数据帧号
	// Access：    	public
	// Returns：   	void
	// Parameter：	
	//		iBegCol			-[input] 起点位置，起点列号
	//		iEndCol			-[input] 终点位置，起点列号
	//		color			-[input] 选段背景色 add <2020/11/4> xiongz
	//*****************************************************
	void AddSegment(const int &iBegCol,const int &iEndCol,const QColor &color = C_COLOR_SEG);
	//*****************************************************
	// Method：  	AppendEvent	
	// Purpose：	添加事件标记
	// Access：    	public
	// Returns：   	void
	// Parameter：	
	//		stEvent			-[input] 事件标记结构体
	//*****************************************************
	void AppendEventMark(const StampedEvent &stEvent);
	void AppendEventMark(const QVector<StampedEvent> &vctEvent);
	//*****************************************************
	// Method：  	DeleteEvent	
	// Purpose：	根据标记帧号和时间类型在序列中删除事件
	// Access：    	public
	// Returns：   	void
	// Parameter：	
	//		iIndex			-[input] 事件在发生的帧号
	//		eType			-[input] 事件类型
	//*****************************************************
	void DeleteEvent(const qint64 &iIndex, const E_EVENT_TYPE &eType);
	//获取选段记录
	QVector<SegmentMarks> GetSegMarks() const { return m_vctSeg; }
	//获取事件标记记录
	QVector<StampedEvent> GetEventMarks() const { return m_vctEvent; }
	//数据帧号和页面列号换算
	//*****************************************************
	// Method：  	TransFrame2Col	
	// Purpose：	将数据帧号转换为当前页面的列
	// Access：    	public
	// Returns：   	当前页面的列
	// Parameter：	
	//		iFrame			-[input] 数据的帧号
	//*****************************************************
	qint64 TransFrame2Col(const qint64& iFrame);
	qint64 TransCol2Frame(const qint64 &iCol);

	//设置时间轴
	void SetTimeMap(const CPlotXMap &cAxisX);
	//获取时间轴
	CPlotXMap GetTimeMap() const { return m_cAxisX; }
	//设置当前选段的背景	 @xiongz [2020/11/5]
	void SetCurSegBackground(const QColor &color);


	//TODO:选段序列需要保存，可以保存为xml文件便于访问

	void SaveSegmentMarks();
	void DeleteSegment(const E_SEGMENT_FLAG &eFlag);
	//*****************************************************
	// Method：		UpdateSegmentMarklist	
	// Purpose：		清除选段序列中不需要保存的标记（即标记是E_FLAG_NULL的标记），
	//				在添加标记和保存标记时调用
	// Access：    	private
	// Returns：   			
	// Parameter：	
	//
	//*****************************************************
	void UpdateSegemntMarkList();

private:
	CPlotSegmentData();
	~CPlotSegmentData();
	CPlotSegmentData& operator= (const CPlotSegmentData&) = delete;
	CPlotSegmentData(const CPlotSegmentData&) = delete;
private:
	static CPlotSegmentData* m_pInstance;
	static QMutex m_Mutex;
	static QMutex m_MutexSeg;						//保护标记序列
	static QMutex m_MutexEvent;						//保护事件标记
	QVector<SegmentMarks> m_vctSeg;					//标记序列
	QVector<StampedEvent> m_vctEvent;				//事件标记
	CPlotXMap m_cAxisX;
	SegmentMarks m_stCurSeg;
};

