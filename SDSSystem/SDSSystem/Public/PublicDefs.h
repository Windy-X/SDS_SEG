/********************************************************************
* Copyright (C) 深圳市德力凯医疗设备股份有限公司
* All rights reserved.
*
* 文件名称：PublicDefs.h
* 摘    要：
*		全局通用定义
* 历史记录：
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

// 常量定义 ===============================================
const char C_DEFAULT_DIR[] = "/Default";
const char C_DEF_CONFIG_XML[] = "/DefaultConfig.xml";
const char C_CONFIG_DIR[] = "/Setup";
const char C_CONFIG_XML[] = "/SDSConfig.xml";

// TODO:QNAN作为无效数据，SNAN作为空数据
const int C_VACANT_VALUE = -2048;			// 空数据
const int C_INVALID_VALUE = -4096;			// 无效数据

const QString C_DATETIME_FORMAT = "yyyy_MM_dd_hh:mm:ss.zzz";
const double C_DOUBLE_ZERO = 1e-6;			// 浮点0值
const double C_PI = 3.14159265358979323846;	// PI值

//inch和cm换算
const double C_INCH_CM = 2.54f;


 //数据存储文件名@xiongz [2020/6/16]
const QString C_FILE_NAME_ECG = "\\DataEcg.dat";
const QString C_FILE_NAME_TCD = "\\DataTCD.dat";
const QString C_FILE_NAME_NIBP = "\\DataNibp.dat";
const QString C_FILE_NAME_CO2 = "\\DataCO2.dat";
const QString C_FILE_NAME_SPO2 = "\\DataSpO2.dat";
const QString C_FILE_NAME_HRV = "\\DataHrv.dat";
const QString C_FILE_NAME_BRS = "\\DataBrs.dat";
//各模块数据的数据项
const int C_ECG_COUNT = 2;
const int C_TCD_COUNT = 4;
const int C_NIBP_COUNT = 6;
const int C_CO2_COUNT = 2;
const int C_SPO2_COUNT = 1;
const int C_HRV_COUNT = 4;
const int C_BRS_COUNT = 2;


// 数据编号 ===============================================
enum E_DATA_TYPE
{
	// 来自ECG的数据
	E_DATA_HR = 0,			// 心率
	// 来自TCD的数据
	E_DATA_VMR,				// 右侧血流流速
	E_DATA_PIR,				// 右侧脉动指数
	E_DATA_VML,				// 左侧血流流速
	E_DATA_PIL,				// 左侧脉动指数
	// 来自NIBP的数据
	E_DATA_SYS,				// 收缩压
	E_DATA_DIA,				// 舒张压
	E_DATA_MAP,				// 平均动脉压
	E_DATA_SV,				// 每博量
	E_DATA_CO,				// 心输出量
	E_DATA_SVR,				// 总外周阻力
	// 来自CO2的数据
	E_DATA_RR,				// 呼吸间期
	// 来自SPO2的数据
	E_DATA_SPO2,			// 血氧浓度
	// 计算得到的数据
	E_DATA_VLF,
	E_DATA_LF,
	E_DATA_HF,
	E_DATA_TP,
	E_DATA_LFHF,
	E_DATA_NOS,
	E_DATA_MS,
	//12导心电
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
//数据模块
enum E_DATA_MODULE
{
	//原始数据
	E_MODULE_NIBP = 0x0001,
	E_MODULE_TCD = 0x0002,
	E_MODULE_CO2 = 0x0004,
	E_MODULE_SPO2 = 0x0008,
	E_MODULE_ECG = 0x0010,
	//计算数据
	E_MODULE_HRV = 0x0020,
	E_MODULE_BRS = 0x0040,
	
};
// 计算HRV所用到的数据 ====================================
struct AlgHrvArg
{
	qint64 iFrameIndex;
	double fCardRR;			// HRV需用到RR间期
};

// 计算BRS所用到的数据 ====================================
struct AlgBrsArg
{
	qint64 iFrameIndex;
	double fCardRR;
	double fSys;
};

// HRV计算结果 ============================================
struct AlgHrvResult
{
	qint64 iIndex;			// 结果对应的帧号
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

// HRV计算得到的功率谱密度 ================================
struct AlgHrvPsd
{
	qint64 iIndex;			// 结果对应的帧号
	QVector<QPair<double, double>> vctPsd;

	AlgHrvPsd()
	{
		iIndex = 0;
		vctPsd.clear();
	}
};

// BRS计算得到的NOS和mS ===================================
struct AlgBrsResult
{
	qint64 iIndex;				// 结果对应的帧号
	int iNoS;					// 反射数
	double fmS;					// mS的值				
	QVector<double> vctTau;		// 6个Tau值，对应0个点到6个点分别占的比重

	AlgBrsResult()
		: iIndex(0)
		, iNoS(C_INVALID_VALUE)
		, fmS(C_INVALID_VALUE) {}
};
// 错误码 =================================================
enum E_ERR_CODE
{
	ERR_OK = 0,

	// 文件相关错误码
	ERR_FILE_NOT_EXISTS = 0x0010,		// 文件不存在
	ERR_FILE_OPEN_FAILED,				// 文件打开失败
	ERR_FILE_ALREADY_EXISTS,			// 文件已经存在
	ERR_FILE_PROCESSING_FAILED,			// 文件处理失败
	ERR_FILE_DAMAGED,					// 文件被损坏
	//add @xiongz [2020/6/16]
	ERR_FILE_NO_HEAD,

	// 绘图相关错误码
	ERR_PLOT_OVERSCALE = 0x0100,		// 超出当前显示区域（表示需要进行压缩）

	//add @xiongz [2020/6/16]
	ERR_XML_TAG_NOT_EXISTS,
	ERR_XML_TAG_ALREADY_EXISTS,
	ERR_INVALID_PARAM,
	ERR_DATA_NOT_EXISTS,
	ERR_CURVE_ALREADY_EXISTS,
	ERR_MEM_ALLOC_FAILED,
	ERR_END_OF_RECORDS,		// 遇到记录文件的末尾
	ERR_EVENT_ENCOUNTERED,	// 遇到事件
	ERR_DB_NO_RECORD_AQUIRED,	// 未检索到
	ERR_DB_QUERY_FAIL,
	ERR_DB_INSERT_FAIL,
	ERR_DB_UPDATE_FAIL,
	ERR_DB_DELETE_FAIL,
	ERR_DB_RECORD_EXISTS,
	ERR_USER_CANCEL,
	ERR_SOCKET_TIMEOUT,//网络连接超时

	//函数返回异常值
	ERR_EVENT_ALREADY_EXISTS,	//事件已经存在
	ERR_EVENT_NOT_EXISTS,		//事件不存在

	ERR_FUNC_RETURN_ERROR = -1024,
};

// 刻度轴体信息 -------------------------------------------
struct AxisInfo
{
	double fLowBound;			// 轴体可表示的最小值
	double fUpBound;			// 轴体可表示的最大值
	int iPrecision;				// 刻度值显示精度
	double fMin;				// 刻度可显示的最小是
	double fMax;				// 刻度可调整的最大值
	double fMinRange;			// 最大可放大的尺寸
	double fStep;				// 每次缩放调整的尺寸

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

// 采用那边轴体，由于左右两侧都可能有轴体 -----------------
enum E_AXIS_TYPE
{
	E_AXIS_LEFT = 0,
	E_AXIS_RIGHT
};

// 指数区显示类型 -----------------------------------------
enum E_INDEX_DISPLAY_TYPE
{
	E_INDEX_DISPLAY_TCD = 0,	// TCD显示类型，见需求文档LMCA、RMCA
	E_INDEX_DISPLAY_NIBP,		// NIBP指数显示类型
	E_INDEX_DISPLAY_HRV,		// HRV功率谱指数显示类型
	E_INDEX_DISPLAY_BRF,		// BRF指数显示slope类型
	E_INDEX_DISPLAY_BASIC,
};

// 曲线信息 -----------------------------------------------
struct BandCurveInfo		// 带状曲线
{
	bool bVisible;			// 是否可见
	uchar iLineWidth;		// 上下沿曲线宽度
	QPair<E_DATA_TYPE, E_DATA_TYPE> CurveData;
	bool bIgnoreVacant;		// 是否忽略空值，若忽略空值，直接将前后的有效值连接
	QString strLegend;		// 曲线名称
	E_AXIS_TYPE eAxis;		// 采用左侧/右侧的轴体，曲线名称绘制在左侧/右侧

	BandCurveInfo()
		: bVisible(true)
		, iLineWidth(1)
		, bIgnoreVacant(false)
		, strLegend("")
		, eAxis(E_AXIS_LEFT) {}
};

struct LineCurveInfo		// 线状曲线
{
	bool bVisible;			// 是否可见
	uchar iLineWidth;		// 曲线宽度
	bool bTrendOnly;		// 只显示趋势，不绘制每一列最大值最小值，每列数据都是按照上包络和下包络的平均值绘制
	E_DATA_TYPE CurveData;	// 曲线数据
	bool bIgnoreVacant;		// 是否忽略空值，若忽略空值，直接将前后的有效值连接
	QString strLegend;		// 曲线名称
	E_AXIS_TYPE eAxis;		// 采用左侧/右侧的轴体，曲线名称绘制在左侧/右侧

	LineCurveInfo()
		: bVisible(true)
		, iLineWidth(1)
		, bTrendOnly(false)
		, bIgnoreVacant(false)
		, strLegend("")
		, eAxis(E_AXIS_LEFT) {}
};

struct AccCurveInfo			// 下方绘制曲线
{
	bool bVisible;			// 是否可见
	uchar iLineWidth;		// 上沿曲线宽度
	E_DATA_TYPE CurveData;	// 曲线数据
	double fBase;			// 下方基线
	bool bIgnoreVacant;		// 是否忽略空值，若忽略空值，直接将前后的有效值连接
	QString strLegend;		// 曲线名称
	E_AXIS_TYPE eAxis;		// 采用左侧/右侧的轴体，曲线名称绘制在左侧/右侧

	AccCurveInfo()
		: bVisible(true)
		, iLineWidth(1)
		, bIgnoreVacant(false)
		, strLegend("")
		, eAxis(E_AXIS_LEFT) {}
};

// 一个参数显示信息 =======================================
struct IndexInfo
{
	bool bShowInfo;
	bool bShowRange;		// 是否显示值域
	bool bShowPercent;		// 是否显示百分数
	bool bShowUnit;
	E_DATA_TYPE eData;

	IndexInfo()
		: bShowInfo(true)
		, bShowRange(true)
		, bShowPercent(true)
		, bShowUnit(true)
		, eData(E_DATA_HR) {}
};

// 数据信息 ===============================================
struct DataInfo
{
	QString strName;
	double fMin;
	double fMax;
	QString strUnit;
	int iPrecision;
	QColor clLine;		// 曲线相关的颜色

	DataInfo()
		: strName("")
		, fMin(C_INVALID_VALUE)
		, fMax(C_INVALID_VALUE)
		, strUnit("")
		, iPrecision(0) 
		, clLine(Qt::black) {}
};

// 一导指数设置 ===========================================
struct IndexChannelConfig
{
	bool bShowChName;
	QString strChName;
	QVector<IndexInfo> vctIndexInfo;		// 显示的指数信息
	E_INDEX_DISPLAY_TYPE eIndexType;		// 指数显示类型

	IndexChannelConfig()
		: bShowChName(false)
		, strChName("")
		, eIndexType(E_INDEX_DISPLAY_BASIC) {}
};

// 一导图像设置 ===========================================
struct PlotChannelConfig
{
	AxisInfo stLeftAxis;
	AxisInfo stRightAxis;
	int iScaleDiv;							// 该导图像Y轴被均分成多少份
};

//导联索引号,修改配置 add xiongz-[2020/8/28]
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
//曲线配置
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
// 一导总体配置 ===========================================
struct ChannelConfig
{
	E_CHANNEL_INDEX eIndex;					//导联索引
	PlotChannelConfig stPlotConfig;			// 绘图配置(坐标轴配置)
	IndexChannelConfig stIndexConfig;		// 指数配置
	CurveConfig stCurveConfig;				//曲线配置（可能包含多条曲线）
	double fStretchFactor;					// 该导图像所占比例
	bool bVisible;							// 该导是否可见

	//曲线配置

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


//文件导出的文件头
const char C_COL_SBP[] = "SBP";
const char C_COL_DBP[] = "DBP";
const char C_COL_MBP[] = "MBP";
const char C_COL_RR[] = "R-R";
//文件导出的文件名
const char C_FILE_NAME_TXT[] = "/RawData.txt";
const char C_FILE_NAME_CSV[] = "/RawData.csv";
//存储事件的文件名
const char C_FILE_NAME_EVENT[]= "/Event.xml";

//xml文件节点
const char C_XML_EVENT_ROOT[] = "EventRoot";//ResultEvent.xml和Event.xml文件根节点
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

//HRV 和 BRS文件的文件名
const char C_FILE_NAME_HRV_RESULT[] = "/HrvResult.dat";
const char C_FILE_NAME_HRV_PSD[] = "/HrvPsd.dat";
const char C_FILE_NAME_BRS_RESULT[] = "/BrsResult.dat";

const int  C_HRV_PSD_COUNT = 128;
const int  C_BRS_TAU_COUNT = 6;
const int  C_HRV_RESULT_LINE_LEN = 6 * 8;

//选段保存文件xml节点属性名称	 @xiongz [2020/11/25]
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

//选段文件路径
const char C_SEGMENT_XML_FILE_NAME[] = "segment.xml";
//选段参数
const QColor C_COLOR_SEG = QColor(0, 128, 192, 125);
//统计信息配置add <2020/11/12>xiongz
const int C_SEG_MEASURE_WIDTH = 200;
//定义事件类型
enum E_EVENT_TYPE
{
	//方案事件	 @xiongz [2020/11/30]
	E_EVENT_NONE = 0,
	E_EVENT_MED,				//用药
	E_EVENT_BED_UP,				//床体上升
	E_EVENT_BED_DOWN,			//床体下降
	E_EVENT_FIRDT_AID,			//急救
	//数据分析事件	 @xiongz [2020/11/30]
	E_EVENT_MISS_FLOW_SIGN,		//血流信号丢失
	E_EVENT_FIND_FLOW_SIGN,		//血流信号找回
	E_EVENT_DIZZY,				//头晕
	E_EVENT_CHEST_DISSTRESS,	//胸闷
	E_EVENT_FATIGUE,			//乏力
	E_EVENT_VOMIT,				//呕吐
	E_EVENT_SICK,				//恶心
	E_EVENT_DIAPHORESIS,		//发汗

	E_EVENT_USER_DEF,			//用户自定义
	//系统事件
	E_EVENT_START,
	E_EVENT_SCAN_START,
	E_EVENT_SCAN_PAUSE,
	E_EVENT_END
};
//事件文件名	 @xiongz [2020/11/30]
const char C_EVNET_XML_FILE[] = "Event.xml";
//事件标记控件样式表参数//add <2020/12/8> xiongz
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
//定义事件标记结构体
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
 //界面数据模块
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
//指数类别
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
	E_LAYOUT_PARAMS = 0,//多参数布局
	E_LAYOUT_ECG,//ECG布局
	E_LAYOUT_MAX
};
//选段标记标志
enum E_SEGMENT_FLAG
{
	E_FLAG_NULL = -1, //不保存，无效标记
	E_FLAG_P0 ,
	E_FLAG_P1,
	E_FLAG_P2,
	E_FLAG_P3,
	E_FLAG_P4,
	E_FLAG_P5
};
//布局配置	 @xiongz [2020/11/30]
//常量定义
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

//每个channel的结构信息，包含一个或多个曲线配置。
struct SegmentChannelParam
{
	int iIndex;	//编号顺序
	E_CHANNEL_TYPE eChannelType;//Channel编号
	QVector<CurveParam> vctCurveItem;
	bool bSelected;//是否选中布局
	bool bLayout;//在设置和布局中/仅在布局中
	static bool CompareChannel(SegmentChannelParam &ch1, SegmentChannelParam &ch2)
	{
		return ch1.iIndex < ch2.iIndex;
	}
};
//每导曲线要显示的统计信息	 @xiongz [2020/11/18]
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
//弃用<2020/12/1>
struct SegmentCurveParam
{
	E_DATA_TYPE eType;
	int iIndex;
	MeasureInfo stMeasureInfo;		//统计信息
	QColor color;					//每条曲线颜色
};
//选段显示每导配置
struct SegmentChannelMeasure
{
	E_CHANNEL_TYPE eType;
	QVector <SegmentCurveParam>vctCurveParam;
	int iIndex;
	int iCount;
	double fStrechFactor;			//屏占比
	bool bIsVisible;				//是否可见
	SegmentChannelMeasure()
	{
		iIndex = -1;
		iCount = 2;
		fStrechFactor = 1;
		bIsVisible = false;
				
	}
};
//选段视图配置
struct SegmentView
{
	int iWidth;				//统计信息绘制区域的宽度（pix）
	QFont fontTitle;		//标题字体
	QFont fontCurveIndex;
	QColor colorTitle;		//标题颜色
	QColor colorBackground;	//选段背景色
	//每条曲线对应的颜色	 @xiongz [2020/11/25]
	int iAlpha;						//选段透明度
	SegmentView()
	{
		iWidth = C_SEG_MEASURE_WIDTH;
		colorBackground = C_COLOR_SEG;
		iAlpha = 125;
		
	}


};
//选段配置,
struct SegmentConfig
{
	SegmentView stView;
	QMap<E_LAYOUT_TYPE,QVector<SegmentChannelMeasure>> mapChannel;
	SegmentConfig()
	{
		mapChannel.clear();
	}
};
//选段统计信息配置
struct SegMeasureInfo
{
	int iPulses;		//总搏出量
	int iTime;			//选段时长
	QMap<E_LAYOUT_TYPE, QVector<SegmentChannelMeasure>> mapChannelMeasure;
	
	SegMeasureInfo()
	{
		mapChannelMeasure.clear();
		iPulses = 0;
		iTime = 0;
	}
};
//定义选段结构体
struct  SegmentMarks
{
	//选段标记	 @xiongz [2020/11/26]
	E_SEGMENT_FLAG eFlag;
	qint64 iBegIndex;
	qint64 iEndIndex;
	//选段统计信息
	SegMeasureInfo stMeasureInfo;	//选段统计信息
	//选段外观参数配置
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
