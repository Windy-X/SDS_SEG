/********************************************************************
* Copyright (C)  深圳市德力凯电子有限公司
* All rights reserved
*
* 文件名称：MarkWidget.h
* 摘    要：
*		标记层控件，覆盖整个图示图显示区，背景透明
* 历史记录：
*			<2020/8/20>	xiongz,
* ********************************************************************/
#pragma once

#include <QWidget>
#include <QResizeEvent>
#include <QThread>
#include "PlotLayers/PlotLayerSeg.h"
#include "PlotData/PlotSegmentData.h"
#include "Scale/PlotXMap.h"
#include "SegMenu.h"
const QColor C_COLOR_BASIC_STATUS = QColor(255,255,255);
const QColor C_COLOR_P0 = QColor(0,255,0);
const QColor C_COLOR_P1 = QColor("#15d6ba");
const QColor C_COLOR_P2 = QColor(135, 206, 250);
const QColor C_COLOR_P3 = QColor(173, 255, 47);
const QColor C_COLOR_P4 = QColor(255,215,0);
const QColor C_COLOR_P5 = QColor(255, 105, 180);



class CMarkWidget : public QWidget
{
	Q_OBJECT

public:
	CMarkWidget(QWidget *parent );
	virtual ~CMarkWidget();
signals:
	void SigAxisXChanged(const CPlotXMap& cAxisX);
	//发送选段帧号
	void SigSendSegment(const int& iBeginIndex, const int &iEndIndex);
	//
	void SigSendSegTracking(const bool &bIsTracking);
private slots:
	virtual void paintEvent(QPaintEvent *) override;
	virtual void resizeEvent(QResizeEvent *event) override;
	//鼠标点击事件
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;

	//debug修改当前选段颜色 <2020/11/27> xiongz
	void SetCurrentSegBackground(const QColor &color);
public slots:
	void SetAxisX(const CPlotXMap& cAxisX);
	//设置当前列
	void SetCurCol(const qint64 &iCurCol) { m_iCurCol = iCurCol; }
	//*****************************************************
	// Method：  	SlotMarkEvent	
	// Purpose：	实时接收数据时进行事件标记
	//				实时扫查时的事件标记类型固定，用药，床体上升和下降，急救和自定义
	//				主界面选择进入事件标记模式后发送信号，该函数暂停数据刷新，在最后一帧数据结尾绘制标记线
	//				并标记事件描述
	// Access：    	public
	// Returns：   	void
	// Parameter：	
	//		eEventType		-[input] 事件类型
	//		colorBackground	-[input] 背景色
	//		strDesctiption	-[input] 事件描述
	//*****************************************************
	void SlotMarkEvent(const E_EVENT_TYPE &eEventType, const QColor &colorBackground, const QString &strDescription);
	//*****************************************************
	// Method：  	SetCurrentLayout	
	// Purpose：	设置当前的布局配置和当前的布局（多参数和心电）,并且更新选段参数SegmentMarks
	// Access：    	
	// Returns：   			
	// Parameter：	
	//
	//*****************************************************
	void SetCurrentLayout(const E_LAYOUT_TYPE &eLayout, const QMap<E_LAYOUT_TYPE,QVector<SegmentChannelParam>> &);
	//*****************************************************
	// Method：  	SlotSetMeasureInfo	
	// Purpose：	设置每条曲线的统计信息的槽函数
	//				需要和控件外部统一结构体
	// Access：    	pbulic
	// Returns：   	void
	// Parameter：	
	//		mapMearsureInfo			-[input] 统计信息结构体映射
	//
	//*****************************************************
	void SlotSetMearsureInfo(const QMap<E_DATA_TYPE,MeasureInfo> &mapMearsureInfo);

public:
	//*****************************************************
	// Method：  	InitDlg	
	// Purpose：	初始化标记控件
	// Access：    	public
	// Returns：   	void	
	// Parameter：	
	//		rect		-[input] 控件位置
	//*****************************************************
	void InitDlg();
	void UpdatePlot();
	//*****************************************************
	// Method：  	AppendSegMarks	
	// Purpose：	添加选段数据,外部调用，在选段模式下，进行选段操作之后调用
	// Access：    	public
	// Returns：   	void	
	// Parameter：	
	//		stMarks		-[input] 选段数据
	//*****************************************************
	void AppendSegMarks(const SegmentMarks& stMarks);
	void AppendSegMarks(const QVector<SegmentMarks>& vctMarks);
	//清理内存
	void Clear();

	//*****************************************************
	// Method：  	SetTrack	
	// Purpose：	设置当前是否使用鼠标进行选段操作,鼠标点击后进行设置
	// Access：    	public
	// Returns：   	void		
	// Parameter：	
	//		bTracking		-[input] 是否进行选段操作，true - 正在选段；false - 没有选段
	//*****************************************************
	void SetTracking(const bool& bTracking);
	//获取当前的选段操作状态
	bool GetTracking()const { return m_bIsTracking; }
	//*****************************************************
	// Method：  	SetSegMode	
	// Purpose：	设置进入/退出选段模式
	// Access：    	public
	// Returns：   	void
	// Parameter：	
	//		bSegmode		-[input] 是否进入选段模式
	//*****************************************************
	void SetSegMode(const bool& bSegmode) { m_bIsSegMode = bSegmode; }
	bool GetSegMode() const { return m_bIsSegMode; }

	//设置/获取事件标记状态
	void SetEventMode(const bool &bEvent) { m_bIsSegMode = bEvent; }
	bool GetEventMode() const { return m_bIsEventMode; }
	//获取选段图层可见状态
	bool IsLayerVisible() const;
	//设置图层是否可见
	void SetLayerVisible(const bool& bVisible);

	//TODO:根据数据模块类型计算区域
	//*****************************************************
	// Method：  	CaculateChannelRect	
	// Purpose：	计算数据模块的显示区域
	// Access：    	
	// Returns：   			
	// Parameter：	
	//
	//*****************************************************
	void CaculateChannelRect(const E_CHANNEL_TYPE &eType,QRect *pRect);
	void SetCurrentSegment(const SegMeasureInfo &stMeasureInfo) { m_stSegMarks.stMeasureInfo = stMeasureInfo; };

	//TODO:添加槽函数，刷新当前显示的导
	//TODO:设置每条曲线的信息内容
	//TODO:设置曲线显示的颜色
	void SetCurveColor(const E_CHANNEL_CURVE &eCurve,const QColor);
	//debug <2020/12/1>xiongz
	void SetMeasureInfo(QMap<E_DATA_TYPE,MeasureInfo> &mapMeasureInfo);

	//保存选段
	void SaveSegmentMarks();


	//TODO:鼠标置于趋势图最右侧时，界面自己滚动
	//TODO:在选段处显示趋势图的参数的时长，完整脉搏，均值，最大值，最小值，最小值/最大值
private:
	//在选段标记状态下按下鼠标左键 add <2020/10/17> xiongz
	void OnSegMousePress(QMouseEvent *pEvent);
	//事件标记下按下鼠标左键
	void OnEventMousePress(QMouseEvent *pEvent);
	//选段标记状态下松开鼠标左键
	void OnSegMouseRelease(QMouseEvent *pEvent);
	//进行选段标记操作
	void MarkSeg(QMouseEvent *pEvent);
	//在选段模式下，设置鼠标样式
	void EnterEvent(QEvent* pEvent);
	//鼠标退出事件
	void LeaveEvent(QEvent *pEvent);
	//比较起点和终点位置，若起点小于终点位置，交换两个值

private:
	CPlotLayerSeg *m_pSegLayer;

	bool m_bIsTracking;					//是否在进行选段
	bool m_bIsSegMode;					//是否在选段模式

	bool m_bIsEventMode;				//是否启用时间模式 add <2020/10/16> xiongz

	CPlotSegmentData *m_pSegData;		//数据层对象指针	
	qint64 m_iBegCol;					//点击鼠标，开始的索引
	qint64 m_iEndCol;					//松开鼠标，结束的索引
	qint64 m_iCurCol;					//当前列

	SegMenu *m_pMenu;					//选段菜单控件
	//弃用<2020/11/30> xiongz
	SegmentMarks m_stSegMarks;			//当前选段
	E_LAYOUT_TYPE m_eCurrentLayout;		//当前布局；
	SegmentConfig m_SegConfig;

	
	
				
};
