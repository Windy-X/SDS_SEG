/********************************************************************
* Copyright (C) �����е�����ҽ���豸�ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�PublicDefs.h
* ժ    Ҫ��
*		ȫ��ͨ�ö���
* ��ʷ��¼��
*		<2020/04/21> dingmh, Created
*********************************************************************/
#pragma once
#include <QColor>
#include <QFont>
#include <QVector>
#include <QMap>
#include <QRect>
#include <QDate>
#include <string>

// �������� ===============================================
const char C_DEFAULT_DIR[] = "/Default";
const char C_DEF_CONFIG_XML[] = "/DefaultConfig.xml";
const char C_CONFIG_DIR[] = "/Setup";
const char C_CONFIG_XML[] = "/SDSConfig.xml";

// TODO:QNAN��Ϊ��Ч���ݣ�SNAN��Ϊ������
const int C_VACANT_VALUE = -2048;			// ������
const int C_INVALID_VALUE = -4096;			// ��Ч����

const QString C_DATETIME_FORMAT = "yyyy_MM_dd_hh:mm:ss.zzz";
const double C_DOUBLE_ZERO = 1e-6;			// ����0ֵ
const double C_PI = 3.14159265358979323846;	// PIֵ

//inch��cm����
const double C_INCH_CM = 2.54f;


 //���ݴ洢�ļ���@xiongz [2020/6/16]
const QString C_FILE_NAME_ECG = "\\DataEcg.dat";
const QString C_FILE_NAME_TCD = "\\DataTCD.dat";
const QString C_FILE_NAME_NIBP = "\\DataNibp.dat";
const QString C_FILE_NAME_CO2 = "\\DataCO2.dat";
const QString C_FILE_NAME_SPO2 = "\\DataSpO2.dat";
const QString C_FILE_NAME_HRV = "\\DataHrv.dat";
const QString C_FILE_NAME_BRS = "\\DataBrs.dat";
//��ģ�����ݵ�������
const int C_ECG_COUNT = 2;
const int C_TCD_COUNT = 4;
const int C_NIBP_COUNT = 6;
const int C_CO2_COUNT = 2;
const int C_SPO2_COUNT = 1;
const int C_HRV_COUNT = 4;
const int C_BRS_COUNT = 2;


// ���ݱ�� ===============================================
enum E_DATA_TYPE
{
	// ����ECG������
	E_DATA_HR = 0,			// ����
	// ����TCD������
	E_DATA_VMR,				// �Ҳ�Ѫ������
	E_DATA_PIR,				// �Ҳ�����ָ��
	E_DATA_VML,				// ���Ѫ������
	E_DATA_PIL,				// �������ָ��
	// ����NIBP������
	E_DATA_SYS,				// ����ѹ
	E_DATA_DIA,				// ����ѹ
	E_DATA_MAP,				// ƽ������ѹ
	E_DATA_SV,				// ÿ����
	E_DATA_CO,				// �������
	E_DATA_SVR,				// ����������
	// ����CO2������
	E_DATA_RR,				// ��������
	// ����SPO2������
	E_DATA_SPO2,			// Ѫ��Ũ��
	// ����õ�������
	E_DATA_VLF,
	E_DATA_LF,
	E_DATA_HF,
	E_DATA_TP,
	E_DATA_LFHF,
	E_DATA_NOS,
	E_DATA_MS,
	//12���ĵ�
	E_DATA_ECG_I,
	E_DATA_ECG_II,
	E_DATA_ECG_III,
	E_DATA_ECG_AVR,
	E_DATA_ECG_AVL,
	E_DATA_ECG_AVF,
	E_DATA_ECG_V1,
	E_DATA_ECG_V2,
	E_DATA_ECG_V3,
	E_DATA_ECG_V4,
	E_DATA_ECG_V5,
	E_DATA_ECG_V6,
	E_DATA_MAX
};
//����ģ��
enum E_DATA_MODULE
{
	//ԭʼ����
	E_MODULE_NIBP = 0x0001,
	E_MODULE_TCD = 0x0002,
	E_MODULE_CO2 = 0x0004,
	E_MODULE_SPO2 = 0x0008,
	E_MODULE_ECG = 0x0010,
	//��������
	E_MODULE_HRV = 0x0020,
	E_MODULE_BRS = 0x0040,
	
};
// ����HRV���õ������� ====================================
struct AlgHrvArg
{
	qint64 iFrameIndex;
	double fCardRR;			// HRV���õ�RR����
};

// ����BRS���õ������� ====================================
struct AlgBrsArg
{
	qint64 iFrameIndex;
	double fCardRR;
	double fSys;
};

// HRV������ ============================================
struct AlgHrvResult
{
	qint64 iIndex;			// �����Ӧ��֡��
	double fVlf;
	double fLf;
	double fHf;
	double fTf;
	double fTp;

	AlgHrvResult()
		: iIndex(0)
		, fVlf(C_INVALID_VALUE)
		, fLf(C_INVALID_VALUE)
		, fHf(C_INVALID_VALUE)
		, fTf(C_INVALID_VALUE)
		, fTp(C_INVALID_VALUE) {}
};

// HRV����õ��Ĺ������ܶ� ================================
struct AlgHrvPsd
{
	qint64 iIndex;			// �����Ӧ��֡��
	QVector<QPair<double, double>> vctPsd;

	AlgHrvPsd()
	{
		iIndex = 0;
		vctPsd.clear();
	}
};

// BRS����õ���NOS��mS ===================================
struct AlgBrsResult
{
	qint64 iIndex;				// �����Ӧ��֡��
	int iNoS;					// ������
	double fmS;					// mS��ֵ				
	QVector<double> vctTau;		// 6��Tauֵ����Ӧ0���㵽6����ֱ�ռ�ı���

	AlgBrsResult()
		: iIndex(0)
		, iNoS(C_INVALID_VALUE)
		, fmS(C_INVALID_VALUE) {}
};
// ������ =================================================
enum E_ERR_CODE
{
	ERR_OK = 0,

	// �ļ���ش�����
	ERR_FILE_NOT_EXISTS = 0x0010,		// �ļ�������
	ERR_FILE_OPEN_FAILED,				// �ļ���ʧ��
	ERR_FILE_ALREADY_EXISTS,			// �ļ��Ѿ�����
	ERR_FILE_PROCESSING_FAILED,			// �ļ�����ʧ��
	ERR_FILE_DAMAGED,					// �ļ�����
	//add @xiongz [2020/6/16]
	ERR_FILE_NO_HEAD,

	// ��ͼ��ش�����
	ERR_PLOT_OVERSCALE = 0x0100,		// ������ǰ��ʾ���򣨱�ʾ��Ҫ����ѹ����

	//add @xiongz [2020/6/16]
	ERR_XML_TAG_NOT_EXISTS,
	ERR_XML_TAG_ALREADY_EXISTS,
	ERR_INVALID_PARAM,
	ERR_DATA_NOT_EXISTS,
	ERR_CURVE_ALREADY_EXISTS,
	ERR_MEM_ALLOC_FAILED,
	ERR_END_OF_RECORDS,		// ������¼�ļ���ĩβ
	ERR_EVENT_ENCOUNTERED,	// �����¼�
	ERR_DB_NO_RECORD_AQUIRED,	// δ������
	ERR_DB_QUERY_FAIL,
	ERR_DB_INSERT_FAIL,
	ERR_DB_UPDATE_FAIL,
	ERR_DB_DELETE_FAIL,
	ERR_DB_RECORD_EXISTS,
	ERR_USER_CANCEL,
	ERR_SOCKET_TIMEOUT,//�������ӳ�ʱ

	//���������쳣ֵ
	ERR_EVENT_ALREADY_EXISTS,	//�¼��Ѿ�����
	ERR_EVENT_NOT_EXISTS,		//�¼�������

	ERR_FUNC_RETURN_ERROR = -1024,
};

// �̶�������Ϣ -------------------------------------------
struct AxisInfo
{
	double fLowBound;			// ����ɱ�ʾ����Сֵ
	double fUpBound;			// ����ɱ�ʾ�����ֵ
	int iPrecision;				// �̶�ֵ��ʾ����
	double fMin;				// �̶ȿ���ʾ����С��
	double fMax;				// �̶ȿɵ��������ֵ
	double fMinRange;			// ���ɷŴ�ĳߴ�
	double fStep;				// ÿ�����ŵ����ĳߴ�

	AxisInfo()
		: fLowBound(0.0)
		, fUpBound(0)
		, iPrecision(0) 
		, fMin(0)
		, fMax(0)
		, fMinRange(0)
		, fStep(0) {}

	bool IsValid() const { return (fLowBound != fUpBound); }
};

// �����Ǳ����壬�����������඼���������� -----------------
enum E_AXIS_TYPE
{
	E_AXIS_LEFT = 0,
	E_AXIS_RIGHT
};

// ָ������ʾ���� -----------------------------------------
enum E_INDEX_DISPLAY_TYPE
{
	E_INDEX_DISPLAY_TCD = 0,	// TCD��ʾ���ͣ��������ĵ�LMCA��RMCA
	E_INDEX_DISPLAY_NIBP,		// NIBPָ����ʾ����
	E_INDEX_DISPLAY_HRV,		// HRV������ָ����ʾ����
	E_INDEX_DISPLAY_BRF,		// BRFָ����ʾslope����
	E_INDEX_DISPLAY_BASIC,
};

// ������Ϣ -----------------------------------------------
struct BandCurveInfo		// ��״����
{
	bool bVisible;			// �Ƿ�ɼ�
	uchar iLineWidth;		// ���������߿��
	QPair<E_DATA_TYPE, E_DATA_TYPE> CurveData;
	bool bIgnoreVacant;		// �Ƿ���Կ�ֵ�������Կ�ֵ��ֱ�ӽ�ǰ�����Чֵ����
	QString strLegend;		// ��������
	E_AXIS_TYPE eAxis;		// �������/�Ҳ�����壬�������ƻ��������/�Ҳ�

	BandCurveInfo()
		: bVisible(true)
		, iLineWidth(1)
		, bIgnoreVacant(false)
		, strLegend("")
		, eAxis(E_AXIS_LEFT) {}
};

struct LineCurveInfo		// ��״����
{
	bool bVisible;			// �Ƿ�ɼ�
	uchar iLineWidth;		// ���߿��
	bool bTrendOnly;		// ֻ��ʾ���ƣ�������ÿһ�����ֵ��Сֵ��ÿ�����ݶ��ǰ����ϰ�����°����ƽ��ֵ����
	E_DATA_TYPE CurveData;	// ��������
	bool bIgnoreVacant;		// �Ƿ���Կ�ֵ�������Կ�ֵ��ֱ�ӽ�ǰ�����Чֵ����
	QString strLegend;		// ��������
	E_AXIS_TYPE eAxis;		// �������/�Ҳ�����壬�������ƻ��������/�Ҳ�

	LineCurveInfo()
		: bVisible(true)
		, iLineWidth(1)
		, bTrendOnly(false)
		, bIgnoreVacant(false)
		, strLegend("")
		, eAxis(E_AXIS_LEFT) {}
};

struct AccCurveInfo			// �·���������
{
	bool bVisible;			// �Ƿ�ɼ�
	uchar iLineWidth;		// �������߿��
	E_DATA_TYPE CurveData;	// ��������
	double fBase;			// �·�����
	bool bIgnoreVacant;		// �Ƿ���Կ�ֵ�������Կ�ֵ��ֱ�ӽ�ǰ�����Чֵ����
	QString strLegend;		// ��������
	E_AXIS_TYPE eAxis;		// �������/�Ҳ�����壬�������ƻ��������/�Ҳ�

	AccCurveInfo()
		: bVisible(true)
		, iLineWidth(1)
		, bIgnoreVacant(false)
		, strLegend("")
		, eAxis(E_AXIS_LEFT) {}
};

// һ��������ʾ��Ϣ =======================================
struct IndexInfo
{
	bool bShowInfo;
	bool bShowRange;		// �Ƿ���ʾֵ��
	bool bShowPercent;		// �Ƿ���ʾ�ٷ���
	bool bShowUnit;
	E_DATA_TYPE eData;

	IndexInfo()
		: bShowInfo(true)
		, bShowRange(true)
		, bShowPercent(true)
		, bShowUnit(true)
		, eData(E_DATA_HR) {}
};

// ������Ϣ ===============================================
struct DataInfo
{
	QString strName;
	double fMin;
	double fMax;
	QString strUnit;
	int iPrecision;
	QColor clLine;		// ������ص���ɫ

	DataInfo()
		: strName("")
		, fMin(C_INVALID_VALUE)
		, fMax(C_INVALID_VALUE)
		, strUnit("")
		, iPrecision(0) 
		, clLine(Qt::black) {}
};

// һ��ָ������ ===========================================
struct IndexChannelConfig
{
	bool bShowChName;
	QString strChName;
	QVector<IndexInfo> vctIndexInfo;		// ��ʾ��ָ����Ϣ
	E_INDEX_DISPLAY_TYPE eIndexType;		// ָ����ʾ����

	IndexChannelConfig()
		: bShowChName(false)
		, strChName("")
		, eIndexType(E_INDEX_DISPLAY_BASIC) {}
};

// һ��ͼ������ ===========================================
struct PlotChannelConfig
{
	AxisInfo stLeftAxis;
	AxisInfo stRightAxis;
	int iScaleDiv;							// �õ�ͼ��Y�ᱻ���ֳɶ��ٷ�
};

//����������,�޸����� add xiongz-[2020/8/28]
enum E_CHANNEL_INDEX
{
	CHANNEL_ERROR = -1,
	CHANNEL_HR = 1,
	CHANNEL_NIBP,
	CHANNEL_VMR_PIR,
	CHANNEL_VML_PIL,
	CHANNEL_SV,
	CHANNEL_CO,
	CHANNEL_TPR,
	CHANNEL_CO2,
	CHANNEL_SPO2,
	CHANNEL_HRV,
	CHANNEL_LF_HF,
	CHANNEL_MS
};
//��������
struct CurveConfig
{
	QVector<AccCurveInfo> vctAccCurves;
	QVector<BandCurveInfo> vctBandCurves;
	QVector<LineCurveInfo> vctLineCurves;
	~CurveConfig()
	{
		vctLineCurves.clear();
		vctBandCurves.clear();
		vctAccCurves.clear();
	}
};
// һ���������� ===========================================
struct ChannelConfig
{
	E_CHANNEL_INDEX eIndex;					//��������
	PlotChannelConfig stPlotConfig;			// ��ͼ����(����������)
	IndexChannelConfig stIndexConfig;		// ָ������
	CurveConfig stCurveConfig;				//�������ã����ܰ����������ߣ�
	double fStretchFactor;					// �õ�ͼ����ռ����
	bool bVisible;							// �õ��Ƿ�ɼ�

	//��������

	ChannelConfig()
		: fStretchFactor(1.0)
		, bVisible(true) {}
};



enum  E_LINE_PN
{
	E_LINE_P0 = 0,
	E_LINE_P1,
	E_LINE_P2,
	E_LINE_P3,
	E_LINE_P4,
	E_LINE_P5,
	E_LINE_MAX
};


//�ļ��������ļ�ͷ
const char C_COL_SBP[] = "SBP";
const char C_COL_DBP[] = "DBP";
const char C_COL_MBP[] = "MBP";
const char C_COL_RR[] = "R-R";
//�ļ��������ļ���
const char C_FILE_NAME_TXT[] = "/RawData.txt";
const char C_FILE_NAME_CSV[] = "/RawData.csv";
//�洢�¼����ļ���
const char C_FILE_NAME_EVENT[]= "/Event.xml";

//xml�ļ��ڵ�
const char C_XML_EVENT_ROOT[] = "EventRoot";//ResultEvent.xml��Event.xml�ļ����ڵ�
const char C_XML_NONE_TAG[] = "NONE";
const char C_XML_CONNECTION_ERR_TAG[] = "CONNECTION_ERR";
const char C_XML_ECG_ERR_TAG[] = "ECG_ERR";
const char C_XML_TCD_ERR_TAG[] = "TCD_ERR";
const char C_XML_BACKFLOW_TAG[] = "BACKFLOW";
const char C_XML_NIBP_ERR_TAG[] = "NIBP_ERR";
const char C_XML_CO2_ERR_TAG[] = "CO2_ERR";
const char C_XML_SPO2_ERR_TAG[] = "SPO2_ERR";
const char C_XML_HRV_PARAM_TAG[] = "HRV_PARAM";
const char C_XML_BRS_PARAM_TAG[] = "BRS_PARAM";
const char C_XML_CONNECTION_RECOVER_TAG[] = "CONNCETION_RECOVER";
const char C_XML_ECG_RECOVER_TAG[] = "ECG_RECOVER";
const char C_XML_TCD_RECOVER_TAG[] = "TCD_RECOVER";
const char C_XML_BACKFLOW_OVER_TAG[] = "BACKFLOW_OVER";
const char C_XML_NIBP_RECOVER_TAG[] = "NIBP_RECOVER";
const char C_XML_CO2_RECOVER_TAG[] = "CO2_RECOVER";
const char C_XML_SPO2_RECOVER_TAG[] = "SPO2_RECOVER";
const char C_XML_ITEM_TAG[] = "item";
const char C_XML_INDEX_ATTR[] = "index";
const char C_XML_TYPE_ATTR[] = "type";
const char C_XML_DES_ATTR[] = "description";
const char C_XML_HEAD[] = "xml version = \"1.0\" encoding = \"utf-8\"";

//HRV �� BRS�ļ����ļ���
const char C_FILE_NAME_HRV_RESULT[] = "/HrvResult.dat";
const char C_FILE_NAME_HRV_PSD[] = "/HrvPsd.dat";
const char C_FILE_NAME_BRS_RESULT[] = "/BrsResult.dat";

const int  C_HRV_PSD_COUNT = 128;
const int  C_BRS_TAU_COUNT = 6;
const int  C_HRV_RESULT_LINE_LEN = 6 * 8;

//ѡ�α����ļ�xml�ڵ���������	 @xiongz [2020/11/25]
const char C_SEG_XML_HEAD[] = "xml ver = \"1.0\" encoding = \"utf-8\"";
const char C_SEG_XML_SEG_INFO_NODE[] = "SegmentInfo";
const char C_SEG_XML_SEG_FLAG_NODE[] = "SegmentFlag";
//const char C_SEG_XML_FLAG_P0_NODE[] = "FlagP0";
//const char C_SEG_XML_FLAG_P1_NODE[] = "FlagP1";
//const char C_SEG_XML_FLAG_P2_NODE[] = "FlagP2";
//const char C_SEG_XML_FLAG_P3_NODE[] = "FlagP3";
//const char C_SEG_XML_FLAG_P4_NODE[] = "FlagP4";
//const char C_SEG_XML_FLAG_P5_NODE[] = "FlagP5";
const char C_SEG_XML_FLAG_ATTR[] = "Flag";
const char C_SEG_XML_CHANNEL_NODE[] = "Channel";
const char C_SEG_XML_ITEM_NODE[] = "item";
const char C_SEG_XML_STRAT_INDEX_ATTR[] = "StartIndex";
const char C_SEG_XML_END_INDEX_ATTR[] = "EndIndex";
const char C_SEG_XML_PLUSES_ATTR[] = "Pluses";
const char C_SEG_XML_TIME_ATTR[] = "Time";
const char C_SEG_XML_INDEX_ATTR[] = "index";
const char C_SEG_XML_VISIBLE_ATTR[] = "visible";
const char C_SEG_XML_TYPE_ATTR[] = "type";
const char C_SEG_XML_MEAN_ATTR[] = "mean";
const char C_SEG_XML_MAX_ATTR[] = "max";
const char C_SEG_XML_MIN_ATTRR[] = "min";
const char C_SEG_XML_MAX_DIV_MIN_ATTR[] = "max_div_min";

//ѡ���ļ�·��
const char C_SEGMENT_XML_FILE_NAME[] = "segment.xml";
//ѡ�β���
const QColor C_COLOR_SEG = QColor(0, 128, 192, 125);
//ͳ����Ϣ����add <2020/11/12>xiongz
const int C_SEG_MEASURE_WIDTH = 200;
//�����¼�����
enum E_EVENT_TYPE
{
	//�����¼�	 @xiongz [2020/11/30]
	E_EVENT_NONE = 0,
	E_EVENT_MED,				//��ҩ
	E_EVENT_BED_UP,				//��������
	E_EVENT_BED_DOWN,			//�����½�
	E_EVENT_FIRDT_AID,			//����
	//���ݷ����¼�	 @xiongz [2020/11/30]
	E_EVENT_MISS_FLOW_SIGN,		//Ѫ���źŶ�ʧ
	E_EVENT_FIND_FLOW_SIGN,		//Ѫ���ź��һ�
	E_EVENT_DIZZY,				//ͷ��
	E_EVENT_CHEST_DISSTRESS,	//����
	E_EVENT_FATIGUE,			//����
	E_EVENT_VOMIT,				//Ż��
	E_EVENT_SICK,				//����
	E_EVENT_DIAPHORESIS,		//����

	E_EVENT_USER_DEF,			//�û��Զ���
	//ϵͳ�¼�
	E_EVENT_START,
	E_EVENT_SCAN_START,
	E_EVENT_SCAN_PAUSE,
	E_EVENT_END
};
//�¼��ļ���	 @xiongz [2020/11/30]
const char C_EVNET_XML_FILE[] = "Event.xml";
//�¼���ǿؼ���ʽ�����//add <2020/12/8> xiongz
struct BorderParam
{
	QString strStyle;
	int		iWidth;
	QColor colorBorder;
	BorderParam()
	{
		strStyle = "radius:3px";
		iWidth = 0;
	}
};
struct MarkStyleSheetParam
{
	E_EVENT_TYPE eEventType;
	QFont fontMark;
	BorderParam stBorder;
	QColor colorBackground;
	QString strDescripiton;
	MarkStyleSheetParam()
	{
		eEventType = E_EVENT_NONE;
		fontMark.setBold(true);
		fontMark.setPixelSize(10);
		fontMark.setFamily("arial");
		colorBackground = Qt::yellow;
		strDescripiton = "medicine";
	}
	static QString Color2QString(const QColor &color) 
	{
		QString str_rgb = QString("rgb(%1,%2,%3,%4)").arg(color.red())
			.arg(color.green()).arg(color.blue()).arg(color.alpha());
		return str_rgb;
	}
};
//�����¼���ǽṹ��
struct StampedEvent
{
	qint64 iIndex;
	E_EVENT_TYPE eType;
	QString strDescription;
	QColor lineColor;
	inline StampedEvent()
	{
		iIndex = -1;
		eType = E_EVENT_NONE;
		strDescription = "N/A";
	}
	static bool CompareEvent(const StampedEvent &event1, const StampedEvent &event2)
	{
		return (event1.iIndex < event2.iIndex);
	}
};
 //��������ģ��
enum E_CHANNEL_TYPE
{
	E_CHANNEL_HR = 0,
	E_CHANNEL_NIBP,
	E_CHANNEL_VM_PI_R,
	E_CHANNEL_VM_PI_L,
	E_CHANNEL_SV,
	E_CHANNEL_CO,
	E_CHANNEL_SVR,
	E_CHANNEL_RR,
	E_CHANNEL_SPO2,
	E_CHANNEL_HRV,
	E_CHANNEL_LFHF,
	E_CHANNEL_BRS,
	E_CHANNEL_ECG_I,
	E_CHANNEL_ECG_II,
	E_CHANNEL_ECG_III,
	E_CHANNEL_ECG_AVR,
	E_CHANNEL_ECG_AVL,
	E_CHANNEL_ECG_AVF,
	E_CHANNEL_ECG_V1,
	E_CHANNEL_ECG_V2,
	E_CHANNEL_ECG_V3,
	E_CHANNEL_ECG_V4,
	E_CHANNEL_ECG_V5,
	E_CHANNEL_ECG_V6
};
//ָ�����
enum E_CHANNEL_CURVE
{
	E_CURVE_HR = 0,
	E_CURVE_SYS,
	E_CURVE_MAP,
	E_CURVE_DIA,
	E_CURVE_VMR,
	E_CURVE_PIR,
	E_CURVE_VML,
	E_CURVE_PIL,
	E_CURVE_SV,
	E_CURVE_CO,
	E_CURVE_SVR,
	E_CURVE_RR,
	E_CURVE_SPO2,
	E_CURVE_TP,
	E_CURVE_VLF,
	E_CURVE_LF,
	E_CURVE_HF,
	E_CURVE_LF_HF,
	E_CURVE_SLOP,
	E_CURVE_BRF,
	E_CURVE_I,
	E_CURVE_II,
	E_CURVE_III,
	E_CURVE_AVR,
	E_CURVE_AVL,
	E_CURVE_AVF,
	E_CURVE_V1,
	E_CURVE_V2,
	E_CURVE_V3,
	E_CURVE_V4,
	E_CURVE_V5,
	E_CURVE_V6
};
enum E_LAYOUT_TYPE
{
	E_LAYOUT_PARAMS = 0,//���������
	E_LAYOUT_ECG,//ECG����
	E_LAYOUT_MAX
};
//ѡ�α�Ǳ�־
enum E_SEGMENT_FLAG
{
	E_FLAG_NULL = -1, //�����棬��Ч���
	E_FLAG_P0 ,
	E_FLAG_P1,
	E_FLAG_P2,
	E_FLAG_P3,
	E_FLAG_P4,
	E_FLAG_P5
};
//��������	 @xiongz [2020/11/30]
//��������
const qint64 C_DEFAULT_VALUE = -4096;
const QColor C_DEFAULT_COLOR = Qt::transparent;
const QString C_EMPTY_STRING = QString("");

struct CurveParam
{
	QString strName;
	QString strUnit;
	QColor crCurve;
	double dLower;
	double dUpper;
	bool bShowRange;
	bool bShowColor;
	QVector<double> vctLowerValue;
	QVector<double> vctUpperValue;
	CurveParam()
	{
		strName = "";
		strUnit = "";
		crCurve = C_DEFAULT_COLOR;
		dLower = C_DEFAULT_VALUE;
		dUpper = C_DEFAULT_VALUE;
		bShowColor = false;
		bShowRange = false;
		vctLowerValue.clear();
		vctUpperValue.clear();
	}
	bool ShowRange() { return bShowRange; }
	bool ShowColor() { return bShowColor; }
	QString color2Str()
	{
		return (QString::number(crCurve.red()) + ","
			+ QString::number(crCurve.green()) + "," 
			+ QString::number(crCurve.blue()) );
	}
};

//ÿ��channel�Ľṹ��Ϣ������һ�������������á�
struct SegmentChannelParam
{
	int iIndex;	//���˳��
	E_CHANNEL_TYPE eChannelType;//Channel���
	QVector<CurveParam> vctCurveItem;
	bool bSelected;//�Ƿ�ѡ�в���
	bool bLayout;//�����úͲ�����/���ڲ�����
	static bool CompareChannel(SegmentChannelParam &ch1, SegmentChannelParam &ch2)
	{
		return ch1.iIndex < ch2.iIndex;
	}
};
//ÿ������Ҫ��ʾ��ͳ����Ϣ	 @xiongz [2020/11/18]
struct MeasureInfo
{
	QString strMean;
	QString strMax;
	QString strMin;
	QString strMax_Min;
	MeasureInfo()
	{
		strMean = "";
		strMax = "";
		strMin = "";
		strMax_Min = "";
	}
	void SetMeasureInfo(const QString &strMeanValue,
		const QString &strMaxValue,
		const QString &strMinValue,
		const QString &strMaxMinValue
		)
	{
		strMean = strMeanValue;
		strMax = strMaxValue;
		strMin = strMinValue;
		strMax_Min = strMaxMinValue;
	}
};
//����<2020/12/1>
struct SegmentCurveParam
{
	E_DATA_TYPE eType;
	int iIndex;
	MeasureInfo stMeasureInfo;		//ͳ����Ϣ
	QColor color;					//ÿ��������ɫ
};
//ѡ����ʾÿ������
struct SegmentChannelMeasure
{
	E_CHANNEL_TYPE eType;
	QVector <SegmentCurveParam>vctCurveParam;
	int iIndex;
	int iCount;
	double fStrechFactor;			//��ռ��
	bool bIsVisible;				//�Ƿ�ɼ�
	SegmentChannelMeasure()
	{
		iIndex = -1;
		iCount = 2;
		fStrechFactor = 1;
		bIsVisible = false;
				
	}
};
//ѡ����ͼ����
struct SegmentView
{
	int iWidth;				//ͳ����Ϣ��������Ŀ�ȣ�pix��
	QFont fontTitle;		//��������
	QFont fontCurveIndex;
	QColor colorTitle;		//������ɫ
	QColor colorBackground;	//ѡ�α���ɫ
	//ÿ�����߶�Ӧ����ɫ	 @xiongz [2020/11/25]
	int iAlpha;						//ѡ��͸����
	SegmentView()
	{
		iWidth = C_SEG_MEASURE_WIDTH;
		colorBackground = C_COLOR_SEG;
		iAlpha = 125;
		
	}


};
//ѡ������,
struct SegmentConfig
{
	SegmentView stView;
	QMap<E_LAYOUT_TYPE,QVector<SegmentChannelMeasure>> mapChannel;
	SegmentConfig()
	{
		mapChannel.clear();
	}
};
//ѡ��ͳ����Ϣ����
struct SegMeasureInfo
{
	int iPulses;		//�ܲ�����
	int iTime;			//ѡ��ʱ��
	QMap<E_LAYOUT_TYPE, QVector<SegmentChannelMeasure>> mapChannelMeasure;
	
	SegMeasureInfo()
	{
		mapChannelMeasure.clear();
		iPulses = 0;
		iTime = 0;
	}
};
//����ѡ�νṹ��
struct  SegmentMarks
{
	//ѡ�α��	 @xiongz [2020/11/26]
	E_SEGMENT_FLAG eFlag;
	qint64 iBegIndex;
	qint64 iEndIndex;
	//ѡ��ͳ����Ϣ
	SegMeasureInfo stMeasureInfo;	//ѡ��ͳ����Ϣ
	//ѡ����۲�������
	SegmentView stView;
	SegmentMarks()
	{
		eFlag = E_FLAG_NULL;
		iBegIndex = -1;
		iEndIndex = -1;
	}
	static bool CompareSeg(const SegmentMarks &mark1, const SegmentMarks &mark2)
	{
		if (mark1.iBegIndex == mark2.iBegIndex)
		{
			return mark1.iEndIndex < mark2.iEndIndex;

		}
		else
		{
			return mark1.iBegIndex < mark2.iBegIndex;
		}
	}
};
