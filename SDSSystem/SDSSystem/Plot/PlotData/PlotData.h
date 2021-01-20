/********************************************************************
* Copyright (C)  深圳市德力凯电子有限公司
* All rights reserved
*
* 文件名称：CPlotData.h
* 摘    要：
*		数据处理层
* 历史记录：
*			Copy<2020/8/13>	xiongz,
* ********************************************************************/
#pragma once
#include <QVector>
#include <QMap>
#include <QPair>
#include "PublicDefs.h"
#include "Scale\PlotXMap.h"
class CPlotData
{
public:
	CPlotData();
	~CPlotData();
	//*****************************************************
	// Method：  	Init	
	// Purpose：	初始化
	// Access：    	public
	// Returns：   	void
	// Parameter：	
	//		iPixWidth		-[input] 图像宽度
	//*****************************************************
	void Init(const int iPixWidth);
	//*****************************************************
	// Method：  	AppendData	
	// Purpose：	添加显示数据，调用前先设置当前比率和下一档比率
	// Access：    	public
	// Returns：   	void
	// Parameter：	
	//		fData		-[input] 增加的数据
	//*****************************************************
	void AppendData(const double fData);
	//*****************************************************
	// Method：  	ApppendData	
	// Purpose：	添加显示的数据,调用前先设置当前比率和下一档比率，
	//				是否换档取决于连个条件，vct容量是否达到上限，并且原始数据长度达到时间轴上限
	// Access：    	public
	// Returns：   	void
	// Parameter：	
	//		vctData		-[input] 增加的数据序列
	//		cAxisX		-[input] 添加时间轴
	//		bChange		-[input] 是否换档的标志，
	//*****************************************************
	void AppendData(const QVector<double> vctData, const CPlotXMap &cAxisX, const bool &bChange);

	//清除数据
	void Clear();
	//设置/获取比率
	void SetCurRatio(const double &fRatio) { m_fCurRatio = fRatio; }
	double GetCurRatio() const { return m_fCurRatio; }
	//设置当前索引号；
	void SetIndex(const qint64 iIndex) { m_iCurIndex = iIndex; }
	void AddIndex(const qint64 iOffset) { m_iCurIndex += iOffset; }
	qint64 GetCurrentIndex() const { return m_iCurIndex; }

	//设置/获取当前时间宽度，只在实时接收数据时有效
	void SetCurTimeWidth(const int &iTimeWidth) { m_iTimeWidth = iTimeWidth; }
	qint64 GetCurTimeWidth() const { return m_iTimeWidth; }
	//*****************************************************
	// Method：  	GetDisplayUpEvn	
	// Purpose：	获取数据的上包络显示
	// Access：    	public
	// Returns：   	上包络数据
	// Parameter：	
	//		xMap		-[input] 当前页面的时间轴
	//*****************************************************
	QVector<double>GetDisplayUpEvn(const CPlotXMap &xMap) const;

	//*****************************************************
	// Method：  	GetDisplayDownEvn	
	// Purpose：	获取数据的下包络显示
	// Access：    	public
	// Returns：   	下包络数据
	// Parameter：	
	//		xMap		-[input] 当前页面的时间轴
	//*****************************************************
	QVector<double>GetDisplayDowmEvn(const CPlotXMap &xMap) const;
	//获取原始数据
	const int &GetRawDataLen() const {}

	int GetPixWidth() const { return m_iPixWidth; }

	//*****************************************************
	// Method：  	ModifyShowData	
	// Purpose：	时间轴变化时，修改当前显示值序列m_vctCurEvn
	// Access：    	public
	// Returns：   	void		
	// Parameter：	
	//		pairRatio		--[input] first，修改前的比率，second修改后的比率
	//*****************************************************
	void ModifyShowData(const QPair<double,double> &pairRatio);

	//读取计算比率的因子，当前比率*因子
	double GetPreRatioScale() const { return m_fPreScale; }
	double GetNextRatioScale() const { return m_fNextScale; }
	//读取/设置前进步进
	void SetStep(const qint64 &iStep) { m_iStep = iStep; }
	qint64 GetStep() const { return m_iStep; }
	//平移曲线 add <2020/10/9> xiongz
	void MoveCurve();


	
private:
	QVector<double> m_vctRawData;					//从本地文件读取原始数据
	QVector<QPair<double, double>> m_vctCurData;	//显示数据
	qint64 m_iCurIndex;								//当前索引，同样也是数据长度
	int m_iPixWidth;								//空间的像素宽度
	int m_iReseverdWidth;							//预留窗口的像素宽度
	int m_iTimeWidth;								//当前时间轴的数据宽度
	double m_fPreScale;								//计算上一档比率的因子
	double m_fNextScale;							//计算下一档比率的因子
	double m_fCurRatio;								//当前比率

	qint64	m_iStep;								//数据满一屏，前进步进（等于预留像素宽度）


};

