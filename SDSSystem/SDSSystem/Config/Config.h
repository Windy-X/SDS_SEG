/********************************************************************
* Copyright (C) �����е�����ҽ���豸�ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�Config.h
* ժ    Ҫ��
*		���õ����࣬������дxml�����ļ�
* ��ʷ��¼��
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

const int C_PAGE_STEP = 60 * 60;	// ����3Сʱ��ÿ�β���1Сʱ

// ����ʾ�йص����ã�������
struct ViewParam
{
	QColor clGrid;			// ������ɫ
	QColor clInvalid;		// ��Ч������ɫ
	QColor clBackground;	// ����ɫ
	QColor clForeground;	// ǰ��ɫ
	QColor clIndexBackground;

	int iInvalidLineWidth;	// ��Ч�����߿��
	int iScaleHMargin;		// �̶�ֵˮƽ����߾�
	int iScaleVMargin;		// �̶�ֵ��ֱ����߾�
	int iChannelSpace;		// ��ͬ��֮��ļ��
	int iLegendWidth;		// �����ƿ��
	int iIndexWidth;		// ָ�������
	int iIndexHMargin;
	int iIndexVMargin;
		
	QFont ftScale;			// �̶�����
	QFont ftLegend;			// ������������
	QFont ftTitle;			// ����������
	
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
	
	int iVerticalGrid;		// ˮƽ�����Ϊ���֣���ֱ����������Ϊ��iVerticalGrid + 1��
	double fTitleFactor;	// ��ǰ������
	int iIndexBackAlpha;	// ָ��������͸����
	int iCurveAlpha;		// ɽ�����ߡ���״����͸����
	double fTpHeightFactor;	// HRV��TPָ����ռ�߶ȱ���
	int iTimeLabelHeight;	// ʱ��ָʾ���߶�
};

// �û����ýṹ��
struct ConfigParam
{
	int iDataFreq;			// ����Ƶ��
	int iMinSegLen;			// ��Сѡ�γ���
	ViewParam stViewConfig;	
	QMap<E_DATA_TYPE, DataInfo> mapDataInfo;
};

class CConfig
{
public:
	static CConfig* Instance();
	static void ReleaseInstacne();
	// ���ÿ������캯������ֵ������
	CConfig(const CConfig&) = delete;
	CConfig& operator= (const CConfig&) = delete;

	//*****************************************************
	// Method:    	GetNextPageDataLen
	// Purpose:		��ȡ��һ��ҳ�泤��
	// Access:    	Public
	// Returns:   	��һ��λҳ����ʾ�೤ʱ������
	//				���������ļ������ҳ�泤�Ⱥ󣬰���ÿһ��1Сʱ��ʱ�������ۼ�
	// Parameter: 	void
	//*****************************************************
	int GetNextPageDataLen(const int iCurrPageLen) const;

	//*****************************************************
	// Method:    	GetPrevPageDataLen
	// Purpose:		��ȡǰһ��ҳ�泤��
	// Access:    	public
	// Returns:   	ǰһ��ҳ����ʾ�೤ʱ������
	//				�����ǰ���������ļ������õ���Сҳ�泤�ȣ��ú�������-1
	// Parameter: 	void
	//*****************************************************
	int GetPrevPageDataLen(const int iCurrPageLen) const;

	//*****************************************************
	// Method:    	GetDataSrcIndex
	// Purpose:		��ȡ��ǰ�����Ǹ�����һ������������
	//				��ͬҳ��ʱ�䵵λ������һ����ʾ����
	//				�ڽ�������ѹ������ʾʱ�����Ǹ�����ǰһ��ѹ���ȵ���ʾ���ݽ��д���
	//				�������Ǳ������Ǵ�ԭʼ���ݽ���ֱ��ѹ������
	//				���������󣬵��������ٶȺ�����4Сʱ��180�������ݣ�
	// Access:    	public
	// Returns:   	��ǰ����Ӧ���Ǹ�����һ������ʾ��������
	//				����������λ��Ĭ��4Сʱ������4Сʱ�����ݽ�������
	//				���û�ﵽ��С��λ��Ĭ��20���ӣ�������ԭʼ���ݽ���
	// Parameter: 	void
	//*****************************************************
	int GetDataSrcIndex(const int iCurrPageLen) const;

	// ��ȡ����ҳ�泤��ѡ��
	const QVector<int>& GetPageDataLenOpt() const { return m_vctPageDataLen; }
	// ��ȡҳ�泤�ȵ�λ��Ӧ��ҳ�泤��
	int GetPageDataLen(const int iOpt) const { return m_vctPageDataLen.at(iOpt); }
	// ��ȡ����Ƶ��
	int GetDataFreq() const { return m_ConfigParam.iDataFreq; }
	// ��ȡ��Сѡ�γ���
	int GetMinSegLen() const { return m_ConfigParam.iMinSegLen; }
	//��ȡԤ�����ڿ��,��λCM
	double GetReservedWidthCM() const { return m_fReserverWidth; }
	int GetReservedWidth() const;

	// ��ȡ������Ϣ
	const QMap<E_DATA_TYPE, DataInfo>& GetDataInfo() const { return m_ConfigParam.mapDataInfo; }
	DataInfo GetDataInfo(const E_DATA_TYPE eType) const { return m_ConfigParam.mapDataInfo.value(eType); }
	
	// ��ȡ������ɫ
	const QColor& GetGridColor() const { return m_ConfigParam.stViewConfig.clGrid; }
	// ��ȡ��Ч������ɫ
	const QColor& GetInvalidColor() const { return m_ConfigParam.stViewConfig.clInvalid; }
	// ��ȡ����ɫ
	const QColor& GetBackgroundColor() const { return m_ConfigParam.stViewConfig.clBackground; }
	// ��ȡǰ��ɫ
	const QColor& GetForegroundColor() const { return m_ConfigParam.stViewConfig.clForeground; }
	// ��ȡĬ��ָ��������
	const QColor& GetIndexBackColor() const { return m_ConfigParam.stViewConfig.clIndexBackground; }

	// ��ȡ��Ч�����߿�
	int GetInvalidLineWidth() const { return m_ConfigParam.stViewConfig.iInvalidLineWidth; }
	// ��ȡ�̶�ֵˮƽ�߾�
	int GetScaleHMargin() const { return m_ConfigParam.stViewConfig.iScaleHMargin; }
	// ��ȡ�̶�ֵ��ֱ�߾�
	int GetScaleVMargin() const { return m_ConfigParam.stViewConfig.iScaleVMargin; }
	// ��ȡ����ͼ��֮����
	int GetChannelSpace() const { return m_ConfigParam.stViewConfig.iChannelSpace; }
	// ��ȡ�������ƿ��
	int GetLegendWidth() const { return m_ConfigParam.stViewConfig.iLegendWidth; }
	// ��ȡָ�������
	int GetIndexWidth() const { return m_ConfigParam.stViewConfig.iIndexWidth; }
	int GetIndexHMargin() const { return m_ConfigParam.stViewConfig.iIndexHMargin; }
	int GetIndexVMargin() const { return m_ConfigParam.stViewConfig.iIndexVMargin; }

	// ��ȡ�̶�ֵ����
	const QFont& GetScaleFont() const { return m_ConfigParam.stViewConfig.ftScale; }
	// ��ȡ������������
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
	// ��ȡ/���ó�ʼ������-------------------------------------------
	// ��ȡһ���е���������Щ
	QVector<AccCurveInfo> ReadAccCurveConfig(const int iCh) const;
	QVector<BandCurveInfo> ReadBandCurveConfig(const int iCh) const;
	QVector<LineCurveInfo> ReadLineCurveConfig(const int iCh) const;
	// ��ȡ�������ã����ɸ�����ָ����ɫѡ��
	QVector<ChannelConfig> ReadChannelsConfig();
	// ���� ---------------------------------------------------------
	// ����ĳ�����
	void SetScaleRange(const int iCh, E_AXIS_TYPE eAxis, const double fLowValue, const double fUpValue);
	// ����ָ������������
	void SetAlertRange(const E_DATA_TYPE eData, const double fLowBound, const double fUpBound);
	// �����������͵���ɫ
	void SetDataColor(const E_DATA_TYPE eData, const QColor& clNew);


	//===================ѡ������add <2020/11/13> xiongz==================
	int GetSegmentMeasureWidth() const { return m_SegmentConfig.stView.iWidth; };
	QFont GetSegmentTitleFont() const { return m_SegmentConfig.stView.fontTitle; }
	QFont GetSegmentChannelsFont() const { return m_SegmentConfig.stView.fontCurveIndex; }
	QColor GetSegmentTitleColor() const { return m_SegmentConfig.stView.colorTitle; }
	SegmentConfig GetSegmentConfig() const { return m_SegmentConfig; }
	QMap<E_LAYOUT_TYPE,QVector<SegmentChannelMeasure>> GetSegmentChannelMeasure() const 
	{
		return m_SegmentConfig.mapChannel;
	}
	//===================�¼���ǿؼ���ʽ�����add <2020/12/8> xiongz==================
	QMap<E_EVENT_TYPE, MarkStyleSheetParam> GetEventMarksParam() const 
	{
		return m_mapMarksStyleParam;
	}
private:
	// ���캯������������˽�л�
	CConfig();
	~CConfig();
	
	//*****************************************************
	// Method:    	ReadTimePerPageSetup
	// Purpose:		��ȡÿҳʱ��ѡ��
	// Access:    	private
	// Returns:   	void
	// Parameter: 	void
	//*****************************************************
	void ReadTimePerPageSetup();
	// ��ȡѡ����Сʱ��
	void ReadMinSegLenSetup();
	// ��ȡһ����Y������
	void ReadChannelAxisInfo(const rapidxml::xml_node<>* pChannelNode, PlotChannelConfig* pConfig) const;
	// ��ȡһ����ָ����������
	void ReadChannelIndexSetup(const rapidxml::xml_node<>* pChannelNode, IndexChannelConfig* pConfig) const;
	// ��ȡһ���������ű���
	void ReadChannelStretchFactor(const rapidxml::xml_node<>* pChannelNode, double* pStretchFactor) const;
	//��ȡһ����������Ϣxiongz-[2020/8/27[
	void ReadChannelLineCurve(const rapidxml::xml_node<> * pChannelNode, QVector<LineCurveInfo> *pVctLineCurve) const ;
	//��ȡһ����ɽ��������Ϣ xiongz-[2020/8/28]
	void ReadChannelAccCurve(const rapidxml::xml_node<> * pChannelNode, QVector<AccCurveInfo> *pVctAccCurve) const;
	//��ȡһ����״����������Ϣ xiongz-[2020/8/28]
	void ReadChannelBandCurve(const rapidxml::xml_node<> *pChannelNode,QVector<BandCurveInfo> *pVctBandCurve) const;
	//��ȡ�������� xiongz-[2020/8/28]
	void ReadChannelIndex(const rapidxml::xml_node<> *pChannelNode, E_CHANNEL_INDEX *pChannelIndex) const;
	void ReadChannelCurve(const rapidxml::xml_node<> *pChannelNode, CurveConfig *pCurveConfig) const;
	// ��ȡ������Ϣ
	void ReadDataInfo();
	// ��ȡ��ʾ�й�����
	void ReadViewParam();

	//��ȡԤ�����ڿ�ȣ���λinch
	void ReadReserveWidth();

	QFont ReadFont(const rapidxml::xml_node<>* pFonts, QString strItem);

	//===================ѡ������add <2020/11/13> xiongz==================
	void ReadSegmentConfig();

	//===================�¼���ǿؼ���ʽ��==================
	void ReadEventMarksStyle();

private:
	static CConfig* m_pInstance;
	static QMutex m_Mutex;

	ConfigParam m_ConfigParam;
	QVector<int> m_vctPageDataLen;		// ÿҳʱ����λ
	QVector<QMap<E_DATA_TYPE, QColor>> m_vctNormalColors;
	double m_fReserverWidth;			//Ԥ�����ڵĳ��ȣ���λinch

	SegmentConfig m_SegmentConfig;		//����ѡ�ε�������Ϣ
	QMap<E_EVENT_TYPE,MarkStyleSheetParam>	m_mapMarksStyleParam; //�¼������ʽ�����	 @xiongz [2020/12/8]


};


