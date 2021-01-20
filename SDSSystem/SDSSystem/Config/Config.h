/********************************************************************
* Copyright (C) 深圳市德力凯医疗设备股份有限公司
* All rights reserved.
*
* 文件名称：Config.h
* 摘    要：
*		配置单例类，用来读写xml配置文件
* 历史记录：
*		<2020/04/21> dingmh, Created
*********************************************************************/
#pragma once
#include <QMutex>
#include <QVector>
#include <QMap>
#include <QString>
#include <QFont>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include "PublicDefs.h"

const int C_PAGE_STEP = 60 * 60;	// 超出3小时后，每次步进1小时

// 与显示有关的配置（常量）
struct ViewParam
{
	QColor clGrid;			// 网格颜色
	QColor clInvalid;		// 无效数据颜色
	QColor clBackground;	// 背景色
	QColor clForeground;	// 前景色
	QColor clIndexBackground;

	int iInvalidLineWidth;	// 无效数据线宽度
	int iScaleHMargin;		// 刻度值水平方向边距
	int iScaleVMargin;		// 刻度值垂直方向边距
	int iChannelSpace;		// 不同导之间的间距
	int iLegendWidth;		// 导名称宽度
	int iIndexWidth;		// 指数区宽度
	int iIndexHMargin;
	int iIndexVMargin;
		
	QFont ftScale;			// 刻度字体
	QFont ftLegend;			// 曲线名称字体
	QFont ftTitle;			// 导名称字体
	
	QFont ftFullSizeValue;
	QFont ftMidSizeValue;
	QFont ftSmallSizeValue;

	QFont ftFullSizePercent;
	QFont ftSmallSizePercent;
	QFont ftFullSizeIndexName;
	QFont ftSmallSizeIndexName;
	QFont ftFullSizeUnit;
	QFont ftSmallSizeUnit;
	QFont ftFullSizeRange;
	QFont ftSmallSizeRange;
	
	int iVerticalGrid;		// 水平方向分为几分，垂直网格线数量为（iVerticalGrid + 1）
	double fTitleFactor;	// 当前导标题
	int iIndexBackAlpha;	// 指数区背景透明度
	int iCurveAlpha;		// 山峰曲线、带状曲线透明度
	double fTpHeightFactor;	// HRV中TP指数所占高度比例
	int iTimeLabelHeight;	// 时间指示条高度
};

// 用户配置结构体
struct ConfigParam
{
	int iDataFreq;			// 数据频率
	int iMinSegLen;			// 最小选段长度
	ViewParam stViewConfig;	
	QMap<E_DATA_TYPE, DataInfo> mapDataInfo;
};

class CConfig
{
public:
	static CConfig* Instance();
	static void ReleaseInstacne();
	// 禁用拷贝构造函数、赋值操作符
	CConfig(const CConfig&) = delete;
	CConfig& operator= (const CConfig&) = delete;

	//*****************************************************
	// Method:    	GetNextPageDataLen
	// Purpose:		获取下一档页面长度
	// Access:    	Public
	// Returns:   	下一档位页面显示多长时间数据
	//				超出配置文件中最大页面长度后，按照每一档1小时的时长向上累加
	// Parameter: 	void
	//*****************************************************
	int GetNextPageDataLen(const int iCurrPageLen) const;

	//*****************************************************
	// Method:    	GetPrevPageDataLen
	// Purpose:		获取前一档页面长度
	// Access:    	public
	// Returns:   	前一档页面显示多长时间数据
	//				如果当前已是配置文件中设置的最小页面长度，该函数返回-1
	// Parameter: 	void
	//*****************************************************
	int GetPrevPageDataLen(const int iCurrPageLen) const;

	//*****************************************************
	// Method:    	GetDataSrcIndex
	// Purpose:		获取当前数据是根据哪一档的数据生成
	//				不同页面时间档位各生成一组显示数据
	//				在进行任意压缩比显示时，总是根据其前一档压缩比的显示数据进行处理
	//				这样做是避免总是从原始数据进行直接压缩处理，
	//				数据量过大，导致运算速度很慢（4小时有180万条数据）
	// Access:    	public
	// Returns:   	当前数据应该是根据哪一档的显示数据生成
	//				如果超过最大档位（默认4小时），则按4小时的数据进行生成
	//				如果没达到最小档位（默认20分钟），则按照原始数据进行
	// Parameter: 	void
	//*****************************************************
	int GetDataSrcIndex(const int iCurrPageLen) const;

	// 获取所有页面长度选项
	const QVector<int>& GetPageDataLenOpt() const { return m_vctPageDataLen; }
	// 获取页面长度档位对应的页面长度
	int GetPageDataLen(const int iOpt) const { return m_vctPageDataLen.at(iOpt); }
	// 获取数据频率
	int GetDataFreq() const { return m_ConfigParam.iDataFreq; }
	// 获取最小选段长度
	int GetMinSegLen() const { return m_ConfigParam.iMinSegLen; }
	//获取预留窗口宽度,单位CM
	double GetReservedWidthCM() const { return m_fReserverWidth; }
	int GetReservedWidth() const;

	// 获取数据信息
	const QMap<E_DATA_TYPE, DataInfo>& GetDataInfo() const { return m_ConfigParam.mapDataInfo; }
	DataInfo GetDataInfo(const E_DATA_TYPE eType) const { return m_ConfigParam.mapDataInfo.value(eType); }
	
	// 获取网格颜色
	const QColor& GetGridColor() const { return m_ConfigParam.stViewConfig.clGrid; }
	// 获取无效数据颜色
	const QColor& GetInvalidColor() const { return m_ConfigParam.stViewConfig.clInvalid; }
	// 获取背景色
	const QColor& GetBackgroundColor() const { return m_ConfigParam.stViewConfig.clBackground; }
	// 获取前景色
	const QColor& GetForegroundColor() const { return m_ConfigParam.stViewConfig.clForeground; }
	// 获取默认指数区背景
	const QColor& GetIndexBackColor() const { return m_ConfigParam.stViewConfig.clIndexBackground; }

	// 获取无效数据线宽
	int GetInvalidLineWidth() const { return m_ConfigParam.stViewConfig.iInvalidLineWidth; }
	// 获取刻度值水平边距
	int GetScaleHMargin() const { return m_ConfigParam.stViewConfig.iScaleHMargin; }
	// 获取刻度值垂直边距
	int GetScaleVMargin() const { return m_ConfigParam.stViewConfig.iScaleVMargin; }
	// 获取各导图像之间间隔
	int GetChannelSpace() const { return m_ConfigParam.stViewConfig.iChannelSpace; }
	// 获取导联名称宽度
	int GetLegendWidth() const { return m_ConfigParam.stViewConfig.iLegendWidth; }
	// 获取指数区宽度
	int GetIndexWidth() const { return m_ConfigParam.stViewConfig.iIndexWidth; }
	int GetIndexHMargin() const { return m_ConfigParam.stViewConfig.iIndexHMargin; }
	int GetIndexVMargin() const { return m_ConfigParam.stViewConfig.iIndexVMargin; }

	// 获取刻度值字体
	const QFont& GetScaleFont() const { return m_ConfigParam.stViewConfig.ftScale; }
	// 获取曲线名称字体
	const QFont& GetLegendFont() const { return m_ConfigParam.stViewConfig.ftLegend; }
	const QFont& GetTitleFont() const { return m_ConfigParam.stViewConfig.ftTitle; }
	const QFont& GetFullSizeValueFont() const { return m_ConfigParam.stViewConfig.ftFullSizeValue; }
	const QFont& GetMidSizeValueFont() const { return m_ConfigParam.stViewConfig.ftMidSizeValue; }
	const QFont& GetSmallSizeValueFont() const { return m_ConfigParam.stViewConfig.ftSmallSizeValue; }
	const QFont& GetFullSizePercentFont() const { return m_ConfigParam.stViewConfig.ftFullSizePercent; }
	const QFont& GetSmallSizePercentFont() const { return m_ConfigParam.stViewConfig.ftSmallSizePercent; }
	const QFont& GetFullSizeNameFont() const { return m_ConfigParam.stViewConfig.ftFullSizeIndexName; }
	const QFont& GetSmallSizeNameFont() const { return m_ConfigParam.stViewConfig.ftSmallSizeIndexName; }
	const QFont& GetFullSizeUnitFont() const { return m_ConfigParam.stViewConfig.ftFullSizeUnit; }
	const QFont& GetSmallSizeUnitFont() const { return m_ConfigParam.stViewConfig.ftSmallSizeUnit; }
	const QFont& GetFullSizeRangeFont() const { return m_ConfigParam.stViewConfig.ftFullSizeRange; }
	const QFont& GetSmallSizeRangeFont() const { return m_ConfigParam.stViewConfig.ftSmallSizeRange; }

	int GetVerticalGrid() const { return m_ConfigParam.stViewConfig.iVerticalGrid; }
	int GetIndexBackAlpha() const { return m_ConfigParam.stViewConfig.iIndexBackAlpha; }
	int GetCurveAlpha() const { return m_ConfigParam.stViewConfig.iCurveAlpha; }
	double GetTitleFactor() const { return m_ConfigParam.stViewConfig.fTitleFactor; }

	const QVector<QMap<E_DATA_TYPE, QColor>>& GetNormalColors() const { return m_vctNormalColors; }
	// 读取/设置初始化设置-------------------------------------------
	// 读取一导中的曲线有哪些
	QVector<AccCurveInfo> ReadAccCurveConfig(const int iCh) const;
	QVector<BandCurveInfo> ReadBandCurveConfig(const int iCh) const;
	QVector<LineCurveInfo> ReadLineCurveConfig(const int iCh) const;
	// 读取导的设置，生成各导各指数颜色选项
	QVector<ChannelConfig> ReadChannelsConfig();
	// 设置 ---------------------------------------------------------
	// 设置某导标尺
	void SetScaleRange(const int iCh, E_AXIS_TYPE eAxis, const double fLowValue, const double fUpValue);
	// 设置指数报警上下限
	void SetAlertRange(const E_DATA_TYPE eData, const double fLowBound, const double fUpBound);
	// 设置数据类型的颜色
	void SetDataColor(const E_DATA_TYPE eData, const QColor& clNew);


	//===================选段配置add <2020/11/13> xiongz==================
	int GetSegmentMeasureWidth() const { return m_SegmentConfig.stView.iWidth; };
	QFont GetSegmentTitleFont() const { return m_SegmentConfig.stView.fontTitle; }
	QFont GetSegmentChannelsFont() const { return m_SegmentConfig.stView.fontCurveIndex; }
	QColor GetSegmentTitleColor() const { return m_SegmentConfig.stView.colorTitle; }
	SegmentConfig GetSegmentConfig() const { return m_SegmentConfig; }
	QMap<E_LAYOUT_TYPE,QVector<SegmentChannelMeasure>> GetSegmentChannelMeasure() const 
	{
		return m_SegmentConfig.mapChannel;
	}
	//===================事件标记控件样式表参数add <2020/12/8> xiongz==================
	QMap<E_EVENT_TYPE, MarkStyleSheetParam> GetEventMarksParam() const 
	{
		return m_mapMarksStyleParam;
	}
private:
	// 构造函数、析构函数私有化
	CConfig();
	~CConfig();
	
	//*****************************************************
	// Method:    	ReadTimePerPageSetup
	// Purpose:		读取每页时长选项
	// Access:    	private
	// Returns:   	void
	// Parameter: 	void
	//*****************************************************
	void ReadTimePerPageSetup();
	// 读取选段最小时长
	void ReadMinSegLenSetup();
	// 读取一导中Y轴设置
	void ReadChannelAxisInfo(const rapidxml::xml_node<>* pChannelNode, PlotChannelConfig* pConfig) const;
	// 读取一导中指数区域设置
	void ReadChannelIndexSetup(const rapidxml::xml_node<>* pChannelNode, IndexChannelConfig* pConfig) const;
	// 读取一导数据缩放比例
	void ReadChannelStretchFactor(const rapidxml::xml_node<>* pChannelNode, double* pStretchFactor) const;
	//读取一导中曲线信息xiongz-[2020/8/27[
	void ReadChannelLineCurve(const rapidxml::xml_node<> * pChannelNode, QVector<LineCurveInfo> *pVctLineCurve) const ;
	//读取一导中山峰曲线信息 xiongz-[2020/8/28]
	void ReadChannelAccCurve(const rapidxml::xml_node<> * pChannelNode, QVector<AccCurveInfo> *pVctAccCurve) const;
	//读取一导带状曲线配置信息 xiongz-[2020/8/28]
	void ReadChannelBandCurve(const rapidxml::xml_node<> *pChannelNode,QVector<BandCurveInfo> *pVctBandCurve) const;
	//读取导联索引 xiongz-[2020/8/28]
	void ReadChannelIndex(const rapidxml::xml_node<> *pChannelNode, E_CHANNEL_INDEX *pChannelIndex) const;
	void ReadChannelCurve(const rapidxml::xml_node<> *pChannelNode, CurveConfig *pCurveConfig) const;
	// 读取数据信息
	void ReadDataInfo();
	// 读取显示有关配置
	void ReadViewParam();

	//读取预留窗口宽度，单位inch
	void ReadReserveWidth();

	QFont ReadFont(const rapidxml::xml_node<>* pFonts, QString strItem);

	//===================选段配置add <2020/11/13> xiongz==================
	void ReadSegmentConfig();

	//===================事件标记控件样式表==================
	void ReadEventMarksStyle();

private:
	static CConfig* m_pInstance;
	static QMutex m_Mutex;

	ConfigParam m_ConfigParam;
	QVector<int> m_vctPageDataLen;		// 每页时长档位
	QVector<QMap<E_DATA_TYPE, QColor>> m_vctNormalColors;
	double m_fReserverWidth;			//预留窗口的长度，单位inch

	SegmentConfig m_SegmentConfig;		//绘制选段的配置信息
	QMap<E_EVENT_TYPE,MarkStyleSheetParam>	m_mapMarksStyleParam; //事件标记样式表参数	 @xiongz [2020/12/8]


};


