/********************************************************************
* Copyright (C) 深圳市德力凯医疗设备股份有限公司
* All rights reserved.
*
* 文件名称：Config.cpp
* 摘    要：
*		实现配置文件读写单例
* 历史记录：
*		<2020/04/21> dingmh, Created
*********************************************************************/
#include "Config.h"
#include <QMutexLocker>
#include <QTextCodec>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include "FileFunction.h"
#include "PublicDefs.h"

// 常量定义 ===============================================
const int C_PAGE_TIME_1 = 20 * 60;		// 每页显示20分钟
const int C_PAGE_TIME_2 = 30 * 60;		// 每页显示30分钟
const int C_PAGE_TIME_3 = 40 * 60;		// 每页显示40分钟
const int C_PAGE_TIME_4 = 50 * 60;		// 每页显示50分钟
const int C_PAGE_TIME_5 = 60 * 60;		// 每页显示60分钟
const int C_PAGE_TIME_6 = 80 * 60;		// 每页显示80分钟
const int C_PAGE_TIME_7 = 100 * 60;		// 每页显示100分钟
const int C_PAGE_TIME_8 = 120 * 60;		// 每页显示120分钟
const int C_PAGE_TIME_9 = 150 * 60;		// 每页显示150分钟
const int C_PAGE_TIME_10 = 180 * 60;		// 每页显示180分钟
const int C_DEF_DATA_FREQ = 20;		// 默认数据125Hz
const int C_DEF_MIN_SEG_TIME = 30;		// 默认30最小选段

const char C_XML_ROOT_TAG[] = "SysConfig";
const char C_XML_PLOT_TAG[] = "PlotSetup";
const char C_XML_PLOT_PAGETIME_TAG[] = "PageTimeScale";
const char C_XML_DATAFREQ_TAG[] = "DataFreqSetup";
const char C_XML_PLOT_CHS_TAG[] = "RawPlot";
const char C_XML_PLOT_MINSEGLEN_TAG[] = "MinSegLen";
const char C_XML_PLOT_CHSSETUP_TAG[] = "ChannelsSetup";
const char C_XML_DATA_TAG[] = "DataType";
//选段配置	 @xiongz [2020/11/17]
const char C_XML_SEGMENT_TAG[] = "Segment";
const char C_XML_SEG_TITLE_TAG[] = "Title";
const char C_XML_SEG_CHANNELS_TAG[] = "Channels";
const char C_XML_SEG_CHANNEL_TAG[] = "Channel";

const char C_XML_RESERVE_WIDTH_TAG[] = "ReservedWidth";	//add <2020/9/27> xiongz

// 颜色常量 -----------------------------------------------
const QColor C_COLOR_GRID(205, 205, 205);		// 网格线颜色
const QColor C_COLOR_INVALID(150, 150, 150);
const QColor C_COLOR_BACKGROUND = Qt::white;	// 背景色
const QColor C_COLOR_FRONTGROUND = Qt::black;

// 线宽 ---------------------------------------------------
const int C_INVALID_WIDTH = 3;				// 无效数据线宽3像素
const int C_VERTICAL_GRID = 5;				// 垂直线，将X轴等分5等分（6条线）
const int C_SCALE_MARGIN_H = 3;				// 刻度值水平边距
const int C_SCALE_MARGIN_V = 4;				// 刻度值垂直边距
const int C_LABEL_SPACE = 6;				// 曲线名称之间间隔
const int C_CHANNEL_SPACE = 1;				// 各导图像之间间隔1像素
const int C_LEGEND_WIDTH = 12;				// 曲线名称宽度
const int C_INDEX_WIDTH = 149;				// 指数区域宽度
const int C_INDEX_HMARGIN = 5;
const int C_INDEX_VMARGIN = 3;
const int C_TIME_LABEL_HEIGHT = 14;			// 时间区域尺寸
											// 字体、字号 ---------------------------------------------
const QString C_SCALE_FONT_FAMILY = "Arial Narrow";		// 字体为微软雅黑；
const int C_SCALE_FONT_SIZE = 8;						// 刻度值字体大小
const int C_LEGEND_FONT_SIZE = 9;

const QString C_DEF_FONT_FAMILY = "Microsoft YaHei";	// 默认字体为微软雅黑
const int C_DEF_FONT_SIZE = 10;

const double C_DEF_TITLE_FACTOR = 0.25;
const double C_DEF_TP_HEIGHT_FACTOR = 0.5;
const int C_DEF_INDEX_BACK_ALPHA = 150;
const int C_DEF_LINE_ALPHA = 120;		// 默认透明度

const int C_DEF_Y_DIV = 4;				// 默认将Y轴分为4等分
const int C_DEF_Y_MAX = 100;			// 默认Y轴最大值为100
const int C_DEF_LINE_WIDTH = 1;			// 默认曲线宽度

// 单例定义 ===============================================
CConfig* CConfig::m_pInstance = NULL;
QMutex CConfig::m_Mutex;

// 单例获取 ===============================================
CConfig* CConfig::Instance()
{
	if (NULL == m_pInstance)
	{
		QMutexLocker lock(&m_Mutex);
		if (NULL == m_pInstance)
		{
			m_pInstance = new CConfig();
		}
	}
	return m_pInstance;
}

// 释放单例 ===============================================
void CConfig::ReleaseInstacne()
{
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

// 构造时，读取XML文件 ====================================
CConfig::CConfig()
{
	m_vctPageDataLen.clear();
	m_vctPageDataLen.append(C_PAGE_TIME_1 * C_DEF_DATA_FREQ);
	m_vctPageDataLen.append(C_PAGE_TIME_2 * C_DEF_DATA_FREQ);
	m_vctPageDataLen.append(C_PAGE_TIME_3 * C_DEF_DATA_FREQ);
	m_vctPageDataLen.append(C_PAGE_TIME_4 * C_DEF_DATA_FREQ);
	m_vctPageDataLen.append(C_PAGE_TIME_5 * C_DEF_DATA_FREQ);
	m_vctPageDataLen.append(C_PAGE_TIME_6 * C_DEF_DATA_FREQ);
	m_vctPageDataLen.append(C_PAGE_TIME_7 * C_DEF_DATA_FREQ);
	m_vctPageDataLen.append(C_PAGE_TIME_8 * C_DEF_DATA_FREQ);
	m_vctPageDataLen.append(C_PAGE_TIME_9 * C_DEF_DATA_FREQ);
	m_vctPageDataLen.append(C_PAGE_TIME_10 * C_DEF_DATA_FREQ);
	
	m_ConfigParam.iDataFreq = C_DEF_DATA_FREQ;
	m_ConfigParam.iMinSegLen = C_DEF_MIN_SEG_TIME;

	// 若Setup文件夹不存在，则创建文件夹
	QString str_dir = file_func::GetExePath() + C_CONFIG_DIR;
	if (!file_func::IsDirExist(str_dir))
	{
		file_func::MakeDir(str_dir);
	}
	// 若xml文件不存在，则从默认位置拷贝
	QString str_xml = file_func::GetExePath() + C_CONFIG_DIR + C_CONFIG_XML;
	QString str_def = file_func::GetExePath() + C_DEF_CONFIG_XML;
	if (!file_func::IsFileExist(str_xml))
	{
		Q_ASSERT(file_func::IsFileExist(str_def));
		file_func::CopyFileToFile(str_def, str_xml, true);
	}
	ReadTimePerPageSetup();
	ReadDataInfo();
	ReadViewParam();
	ReadReserveWidth();

	//选段配置
	ReadSegmentConfig();
	ReadEventMarksStyle();
}

CConfig::~CConfig()
{
	m_vctPageDataLen.clear();
	m_ConfigParam.mapDataInfo.clear();
}

// 读取每页时长选项 =======================================
void CConfig::ReadTimePerPageSetup()
{
	QString str_xml = file_func::GetExePath() + C_CONFIG_DIR + C_CONFIG_XML;
	QByteArray sz_xml = str_xml.toLocal8Bit();
	
	rapidxml::file<> f_doc(sz_xml.data());
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_default>(f_doc.data());
	rapidxml::xml_node<>* p_root = doc.first_node(C_XML_ROOT_TAG);
	if (NULL == p_root)
	{
		return;
	}
	// 读取数据刷新率
	rapidxml::xml_node<>* p_node = p_root->first_node(C_XML_DATAFREQ_TAG);
	if (NULL != p_node)
	{
		m_ConfigParam.iDataFreq = std::strtoul(p_node->value(), NULL, 0);
	}

	// 读取档位
	p_node = p_root->first_node(C_XML_PLOT_TAG);
	if (NULL != p_node)
	{
		rapidxml::xml_node<>* p_subnode = p_node->first_node(C_XML_PLOT_PAGETIME_TAG);
		if (NULL != p_subnode)
		{
			m_vctPageDataLen.clear();
			rapidxml::xml_node<>* p_item;
			for (p_item = p_subnode->first_node("item")
				; p_item
				; p_item = p_item->next_sibling("item"))
			{
				int i_value = std::strtoul(p_item->value(), NULL, 0);
				i_value = i_value * 60 * m_ConfigParam.iDataFreq;
				m_vctPageDataLen.append(i_value);
			}
		}
	}
}

// 读取选段最小时长 =======================================
void CConfig::ReadMinSegLenSetup()
{
	QString str_xml = file_func::GetExePath() + C_CONFIG_DIR + C_CONFIG_XML;
	QByteArray sz_xml = str_xml.toLocal8Bit();

	rapidxml::file<> f_doc(sz_xml.data());
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_default>(f_doc.data());
	rapidxml::xml_node<>* p_root = doc.first_node(C_XML_ROOT_TAG);
	if (NULL == p_root)
	{
		return;
	}
	rapidxml::xml_node<>* p_node = p_root->first_node(C_XML_PLOT_TAG);
	if (NULL != p_node)
	{
		rapidxml::xml_node<>* p_subnode = p_node->first_node(C_XML_PLOT_MINSEGLEN_TAG);
		if (NULL != p_subnode)
		{
			QString str_value = p_subnode->value();
			m_ConfigParam.iMinSegLen = str_value.toInt();
		}
	}
}

// 获取下一档页面长度 =====================================
int CConfig::GetNextPageDataLen(const int iCurrPageLen) const
{
	int i_len_max = m_vctPageDataLen.last();
	int i_ret = iCurrPageLen;
	if (iCurrPageLen > i_len_max)
	{
		while (iCurrPageLen > i_len_max)
		{
			i_len_max += C_PAGE_STEP * GetDataFreq();
		}
		if (iCurrPageLen == i_len_max)
		{
			i_ret = i_len_max + C_PAGE_STEP * GetDataFreq();
		}
		else
		{
			i_ret = i_len_max;
		}
	}
	else
	{
		int i = 0;
		for (; i < m_vctPageDataLen.size(); ++i)
		{
			if (iCurrPageLen <= m_vctPageDataLen.value(i))
			{
				break;
			}
		}
		if (iCurrPageLen == m_vctPageDataLen.value(i))
		{
			i_ret = i >= m_vctPageDataLen.size() - 1 ? (i_len_max + C_PAGE_STEP * GetDataFreq()) : m_vctPageDataLen.value(i + 1);
		}
		else
		{
			i_ret = m_vctPageDataLen.value(i);
		}
	}
	return i_ret;
}

// 获取上一档页面长度 =====================================
int CConfig::GetPrevPageDataLen(const int iCurrPageLen) const
{
	int i_len_max = m_vctPageDataLen.last();
	int i_ret = iCurrPageLen;
	if (iCurrPageLen < m_vctPageDataLen.first())
	{
		i_ret = -1;				// 档位已是最低档位，无法再利用“+”按钮放大
	}
	else if (iCurrPageLen > i_len_max)
	{
		while (iCurrPageLen > i_len_max)
		{
			i_len_max += C_PAGE_STEP * GetDataFreq();
		}
		if (iCurrPageLen == i_len_max)
		{
			i_ret = i_len_max - C_PAGE_STEP * GetDataFreq();
		}
		else
		{
			i_ret = i_len_max - C_PAGE_STEP * GetDataFreq();
		}
	}
	else
	{
		int i = 0;
		for (; i < m_vctPageDataLen.size(); ++i)
		{
			if (iCurrPageLen <= m_vctPageDataLen.value(i))
			{
				break;
			}
		}
		i_ret = i > 0 ? m_vctPageDataLen.value(i - 1) : -1;
	}
	return i_ret;
}

// 获取当前数据是根据哪一档数据生成 =======================
int CConfig::GetDataSrcIndex(const int iCurrPageLen) const
{
	int i_ret = -1;
	int i_len_max = m_vctPageDataLen.last();
	if (iCurrPageLen < m_vctPageDataLen.first())
	{
		i_ret = -1;					// 低于最低档位，采用原始数据作为数据源
	}
	else if (iCurrPageLen > i_len_max)
	{
		i_ret = m_vctPageDataLen.size() - 1;
	}
	else
	{
		int i = 0;
		for (; i < m_vctPageDataLen.size(); ++i)
		{
			if (iCurrPageLen <= m_vctPageDataLen.value(i))
			{
				break;
			}
		}
		if (iCurrPageLen == m_vctPageDataLen.value(i))
		{
			i_ret = i;
		}
		else
		{
			i_ret = i - 1;
		}
	}
	return i_ret;
}

// 读取显示相关配置 =======================================
void CConfig::ReadViewParam()
{
	QString str_xml = file_func::GetExePath() + C_CONFIG_DIR + C_CONFIG_XML;
	QByteArray sz_xml = str_xml.toLocal8Bit();

	m_ConfigParam.stViewConfig.clBackground = C_COLOR_BACKGROUND;
	m_ConfigParam.stViewConfig.clForeground = C_COLOR_FRONTGROUND;
	m_ConfigParam.stViewConfig.clGrid = C_COLOR_GRID;
	m_ConfigParam.stViewConfig.clInvalid = C_COLOR_INVALID;
	m_ConfigParam.stViewConfig.clIndexBackground = C_COLOR_BACKGROUND;

	m_ConfigParam.stViewConfig.iInvalidLineWidth = C_INVALID_WIDTH;
	m_ConfigParam.stViewConfig.iScaleHMargin = C_SCALE_MARGIN_H;
	m_ConfigParam.stViewConfig.iScaleVMargin = C_SCALE_MARGIN_V;
	m_ConfigParam.stViewConfig.iChannelSpace = C_CHANNEL_SPACE;
	m_ConfigParam.stViewConfig.iLegendWidth = C_LEGEND_WIDTH;
	m_ConfigParam.stViewConfig.iIndexWidth = C_INDEX_WIDTH;
	m_ConfigParam.stViewConfig.iIndexHMargin = C_INDEX_HMARGIN;
	m_ConfigParam.stViewConfig.iIndexVMargin = C_INDEX_VMARGIN;
	
	m_ConfigParam.stViewConfig.iTimeLabelHeight = C_TIME_LABEL_HEIGHT;
	m_ConfigParam.stViewConfig.iVerticalGrid = C_VERTICAL_GRID;
	m_ConfigParam.stViewConfig.fTitleFactor = C_DEF_TITLE_FACTOR;
	m_ConfigParam.stViewConfig.fTpHeightFactor = C_DEF_TP_HEIGHT_FACTOR;
	m_ConfigParam.stViewConfig.iIndexBackAlpha = C_DEF_INDEX_BACK_ALPHA;
	m_ConfigParam.stViewConfig.iCurveAlpha = C_DEF_LINE_ALPHA;

	rapidxml::file<> f_doc(sz_xml.data());
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_default>(f_doc.data());
	rapidxml::xml_node<>* p_root = doc.first_node(C_XML_ROOT_TAG);
	if (NULL == p_root)
	{
		return;
	}
	rapidxml::xml_node<>* p_node = p_root->first_node("Views");
	if (NULL == p_node)
	{
		return;
	}
	rapidxml::xml_node<>* p_subnode = p_node->first_node("Colors");
	rapidxml::xml_node<>* p_item;
	rapidxml::xml_attribute<>* p_attr;
	QString str_value;
	if (NULL != p_subnode)
	{
		p_item = p_subnode->first_node("Grid");
		if (NULL != p_item)
		{
			str_value = p_item->value();
			m_ConfigParam.stViewConfig.clGrid.setRgba(QRgb(str_value.toLongLong(NULL, 16)));
		}
		p_item = p_subnode->first_node("Invalid");
		if (NULL != p_item)
		{
			str_value = p_item->value();
			m_ConfigParam.stViewConfig.clInvalid.setRgba(QRgb(str_value.toLongLong(NULL, 16)));
		}
		p_item = p_subnode->first_node("Background");
		if (NULL != p_item)
		{
			str_value = p_item->value();
			m_ConfigParam.stViewConfig.clBackground.setRgba(QRgb(str_value.toLongLong(NULL, 16)));
		}
		p_item = p_subnode->first_node("Foreground");
		if (NULL != p_item)
		{
			str_value = p_item->value();
			m_ConfigParam.stViewConfig.clForeground.setRgba(QRgb(str_value.toLongLong(NULL, 16)));
		}
		p_item = p_subnode->first_node("IndexBackground");
		if (NULL != p_item)
		{
			str_value = p_item->value();
			m_ConfigParam.stViewConfig.clIndexBackground.setRgba(QRgb(str_value.toLongLong(NULL, 16)));
		}
	}
	p_subnode = p_node->first_node("Size");
	if (NULL != p_subnode)
	{
		p_item = p_subnode->first_node("InvalidLineWidth");
		if (NULL != p_item)
		{
			str_value = p_item->value();
			m_ConfigParam.stViewConfig.iInvalidLineWidth = str_value.toInt();
		}
		p_item = p_subnode->first_node("ScaleHMargin");
		if (NULL != p_item)
		{
			str_value = p_item->value();
			m_ConfigParam.stViewConfig.iScaleHMargin = str_value.toInt();
		}
		p_item = p_subnode->first_node("ScaleVMargin");
		if (NULL != p_item)
		{
			str_value = p_item->value();
			m_ConfigParam.stViewConfig.iScaleVMargin = str_value.toInt();
		}
		p_item = p_subnode->first_node("ChannelSpace");
		if (NULL != p_item)
		{
			str_value = p_item->value();
			m_ConfigParam.stViewConfig.iChannelSpace = str_value.toInt();
		}
		p_item = p_subnode->first_node("LegendWidth");
		if (NULL != p_item)
		{
			str_value = p_item->value();
			m_ConfigParam.stViewConfig.iLegendWidth = str_value.toInt();
		}
		p_item = p_subnode->first_node("IndexWidth");
		if (NULL != p_item)
		{
			str_value = p_item->value();
			m_ConfigParam.stViewConfig.iIndexWidth = str_value.toInt();
		}
		p_item = p_subnode->first_node("IndexHMargin");
		if (NULL != p_item)
		{
			str_value = p_item->value();
			m_ConfigParam.stViewConfig.iIndexHMargin = str_value.toInt();
		}
		p_item = p_subnode->first_node("IndexVMargin");
		if (NULL != p_item)
		{
			str_value = p_item->value();
			m_ConfigParam.stViewConfig.iIndexVMargin = str_value.toInt();
		}
		p_item = p_subnode->first_node("TimeLabelHeight");
		if (NULL != p_item)
		{
			str_value = p_item->value();
			m_ConfigParam.stViewConfig.iTimeLabelHeight = str_value.toInt();
		}
	}

	p_subnode = p_node->first_node("Fonts");
	if (NULL != p_subnode)
	{
		m_ConfigParam.stViewConfig.ftScale = ReadFont(p_subnode, "Scale");
		m_ConfigParam.stViewConfig.ftLegend = ReadFont(p_subnode, "Legend");
		m_ConfigParam.stViewConfig.ftTitle = ReadFont(p_subnode, "Title");
		
		m_ConfigParam.stViewConfig.ftFullSizeValue = ReadFont(p_subnode, "IndexFullSizeValue");
		m_ConfigParam.stViewConfig.ftMidSizeValue = ReadFont(p_subnode, "IndexMiddleSizeValue");
		m_ConfigParam.stViewConfig.ftSmallSizeValue = ReadFont(p_subnode, "IndexSmallSizeValue");
		m_ConfigParam.stViewConfig.ftFullSizePercent = ReadFont(p_subnode, "IndexFullSizePercent");
		m_ConfigParam.stViewConfig.ftSmallSizePercent = ReadFont(p_subnode, "IndexSmallSizePercent");
		m_ConfigParam.stViewConfig.ftFullSizeIndexName = ReadFont(p_subnode, "IndexFullSizeName");
		m_ConfigParam.stViewConfig.ftSmallSizeIndexName = ReadFont(p_subnode, "IndexSmallSizeName");
		m_ConfigParam.stViewConfig.ftFullSizeUnit = ReadFont(p_subnode, "IndexFullSizeUnit");
		m_ConfigParam.stViewConfig.ftSmallSizeUnit = ReadFont(p_subnode, "IndexSmallSzieUnit");
		m_ConfigParam.stViewConfig.ftFullSizeRange = ReadFont(p_subnode, "IndexFullSizeRange");
		m_ConfigParam.stViewConfig.ftSmallSizeRange = ReadFont(p_subnode, "IndexSmallSizeRange");
	}

	p_subnode = p_node->first_node("VerticalGrid");
	if (NULL != p_subnode)
	{
		str_value = p_subnode->value();
		m_ConfigParam.stViewConfig.iVerticalGrid = str_value.toInt();
	}

	p_subnode = p_node->first_node("IndexTitleFactor");
	if (NULL != p_subnode)
	{
		str_value = p_subnode->value();
		m_ConfigParam.stViewConfig.fTitleFactor = str_value.toDouble();
	}

	p_subnode = p_node->first_node("IndexBackAlpha");
	if (NULL != p_subnode)
	{
		str_value = p_subnode->value();
		m_ConfigParam.stViewConfig.iIndexBackAlpha = str_value.toInt();
	}

	p_subnode = p_node->first_node("CurveAlpha");
	if (NULL != p_subnode)
	{
		str_value = p_subnode->value();
		m_ConfigParam.stViewConfig.iCurveAlpha = str_value.toInt();
	}
}

// 读取数据设置 ===========================================
void CConfig::ReadDataInfo()
{
	m_ConfigParam.mapDataInfo.clear();
	for (E_DATA_TYPE e_type = E_DATA_HR
		; e_type < E_DATA_MAX
		; e_type = E_DATA_TYPE(e_type + 1))
	{
		m_ConfigParam.mapDataInfo[e_type] = DataInfo();
	}
	QString str_xml = file_func::GetExePath() + C_CONFIG_DIR + C_CONFIG_XML;
	QByteArray sz_xml = str_xml.toLocal8Bit();

	rapidxml::file<> f_doc(sz_xml.data());
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_default>(f_doc.data());
	rapidxml::xml_node<>* p_root = doc.first_node(C_XML_ROOT_TAG);
	if (NULL == p_root)
	{
		return;
	}
	rapidxml::xml_node<>* p_node = p_root->first_node(C_XML_DATA_TAG);
	if (NULL == p_node)
	{
		return;
	}
	rapidxml::xml_attribute<>* p_attr = NULL;
	QString str_value;
	for (rapidxml::xml_node<>* p_item = p_node->first_node("item")
		; p_item
		; p_item = p_item->next_sibling("item"))
	{
		DataInfo st_info;
		p_attr = p_item->first_attribute("ID");
		if (NULL == p_attr)
		{
			continue;
		}
		str_value = p_attr->value();
		E_DATA_TYPE e_type;
		if (str_value.toInt() < E_DATA_HR || str_value.toInt() >= E_DATA_MAX)
		{
			continue;
		}
		e_type = E_DATA_TYPE(str_value.toInt());

		p_attr = p_item->first_attribute("Name");
		if (NULL != p_attr)
		{
			st_info.strName = p_attr->value();
		}

		bool b_ok = false;
		p_attr = p_item->first_attribute("Min");
		if (NULL != p_attr)
		{
			str_value = p_attr->value();
			double f_value = str_value.toDouble(&b_ok);
			if (b_ok)
			{
				st_info.fMin = f_value;
			}
		}

		p_attr = p_item->first_attribute("Max");
		if (NULL != p_attr)
		{
			str_value = p_attr->value();
			double f_value = str_value.toDouble(&b_ok);
			if (b_ok)
			{
				st_info.fMax = f_value;
			}
		}

		p_attr = p_item->first_attribute("Unit");
		if (NULL != p_attr)
		{
			st_info.strUnit = p_attr->value();
		}

		p_attr = p_item->first_attribute("Precision");
		if (NULL != p_attr)
		{
			str_value = p_attr->value();
			int i_value = str_value.toDouble(&b_ok);
			if (b_ok)
			{
				st_info.iPrecision = i_value;
			}
		}

		p_attr = p_item->first_attribute("Color");
		if (NULL != p_attr)
		{
			str_value = p_attr->value();
			qDebug() << str_value;
			st_info.clLine.setRgba(QRgb(str_value.toLongLong(NULL, 16)));
		}

		if (st_info.fMin != C_INVALID_VALUE && st_info.fMax != C_INVALID_VALUE && st_info.fMin > st_info.fMax)
		{
			double f_value = st_info.fMin;
			st_info.fMin = st_info.fMax;
			st_info.fMax = st_info.fMin;
		}

		m_ConfigParam.mapDataInfo[e_type] = st_info;
	}
}

// 读取一导中Y轴设置 ======================================
void CConfig::ReadChannelAxisInfo(const rapidxml::xml_node<>* pChannelNode, PlotChannelConfig* pConfig) const
{
	assert(pChannelNode);
	assert(pConfig);
	rapidxml::xml_attribute<>* p_attr;
	p_attr = pChannelNode->first_attribute("Div");
	QString str_value;
	if (NULL == p_attr)
	{
		pConfig->iScaleDiv = C_DEF_Y_DIV;
	}
	else
	{
		str_value = p_attr->value();
		pConfig->iScaleDiv = (str_value.toInt() > 0) ? str_value.toInt() : C_DEF_Y_DIV;
	}
	rapidxml::xml_node<>* p_axis_node;
	rapidxml::xml_attribute<>* p_attr_down;
	rapidxml::xml_attribute<>* p_attr_up;
	// 左侧轴体
	p_axis_node = pChannelNode->first_node("LeftAxis");
	if (NULL == p_axis_node)
	{
		pConfig->stLeftAxis.fLowBound = 0;
		pConfig->stLeftAxis.fUpBound = C_DEF_Y_MAX;
		pConfig->stLeftAxis.iPrecision = 0;
	}
	else
	{
		p_attr_down = p_axis_node->first_attribute("DownBound");
		p_attr_up = p_axis_node->first_attribute("UpBound");
		if (NULL == p_attr_down || NULL == p_attr_up)
		{
			pConfig->stLeftAxis.fLowBound = 0;
			pConfig->stLeftAxis.fUpBound = C_DEF_Y_MAX;
		}
		else
		{
			str_value = p_attr_down->value();
			double f_value_down = str_value.toDouble();
			str_value = p_attr_up->value();
			double f_value_up = str_value.toDouble();
			if (f_value_down == f_value_up)
			{
				pConfig->stLeftAxis.fLowBound = 0;
				pConfig->stLeftAxis.fUpBound = C_DEF_Y_MAX;
			}
			else
			{
				pConfig->stLeftAxis.fLowBound = (f_value_down < f_value_up) ? f_value_down : f_value_up;
				pConfig->stLeftAxis.fUpBound = (f_value_down > f_value_up) ? f_value_down : f_value_up;
			}			
		}
		p_attr = p_axis_node->first_attribute("Precision");
		if (NULL == p_attr)
		{
			pConfig->stLeftAxis.iPrecision = 0;
		}
		else
		{
			str_value = p_attr->value();
			pConfig->stLeftAxis.iPrecision = str_value.toInt() >= 0 ? str_value.toInt() : 0;
		}

		p_attr = p_axis_node->first_attribute("Min");
		if (NULL == p_attr)
		{
			pConfig->stLeftAxis.fMin = pConfig->stLeftAxis.fLowBound;
		}
		else
		{
			str_value = p_attr->value();
			pConfig->stLeftAxis.fMin = str_value.toDouble() > pConfig->stLeftAxis.fLowBound ? pConfig->stLeftAxis.fLowBound : str_value.toDouble();
		}

		p_attr = p_axis_node->first_attribute("Max");
		if (NULL == p_attr)
		{
			if (pConfig->stLeftAxis.fUpBound > 0)
			{
				pConfig->stLeftAxis.fMax = pConfig->stLeftAxis.fUpBound * 2;
			}
			else
			{
				pConfig->stLeftAxis.fMax = pConfig->stLeftAxis.fUpBound / 2;
			}
		}
		else
		{
			str_value = p_attr->value();
			pConfig->stLeftAxis.fMax = str_value.toDouble() > pConfig->stLeftAxis.fUpBound ? str_value.toDouble() : pConfig->stLeftAxis.fUpBound;
		}

		p_attr = p_axis_node->first_attribute("MinRange");
		double f_range = pConfig->stLeftAxis.fUpBound - pConfig->stLeftAxis.fLowBound;
		if (NULL == p_attr)
		{
			pConfig->stLeftAxis.fMinRange = f_range / 2;
		}
		else
		{
			str_value = p_attr->value();
			double f_value = str_value.toDouble();
			pConfig->stLeftAxis.fMinRange = f_value > f_range ? f_range : f_value;
		}

		p_attr = p_axis_node->first_attribute("Step");
		if (NULL == p_attr)
		{
			pConfig->stLeftAxis.fStep = pConfig->stLeftAxis.fMinRange / 5;
		}
		else
		{
			str_value = p_attr->value();
			pConfig->stLeftAxis.fStep = str_value.toDouble();
		}
	}
	
	// 右侧轴体
	p_axis_node = pChannelNode->first_node("RightAxis");
	if (NULL == p_axis_node)
	{
		pConfig->stRightAxis = AxisInfo();
	}
	else
	{
		p_attr_down = p_axis_node->first_attribute("DownBound");
		p_attr_up = p_axis_node->first_attribute("UpBound");
		if (NULL == p_attr_down || NULL == p_attr_up)
		{
			pConfig->stRightAxis = AxisInfo();
		}
		else
		{
			str_value = p_attr_down->value();
			double f_value_down = str_value.toDouble();
			str_value = p_attr_up->value();
			double f_value_up = str_value.toDouble();
			if (f_value_down == f_value_up)
			{
				pConfig->stRightAxis = AxisInfo();
			}
			else
			{
				pConfig->stRightAxis.fLowBound = (f_value_down < f_value_up) ? f_value_down : f_value_up;
				pConfig->stRightAxis.fUpBound = (f_value_down > f_value_up) ? f_value_down : f_value_up;
			}
		}
		p_attr = p_axis_node->first_attribute("Precision");
		if (NULL == p_attr)
		{
			pConfig->stRightAxis.iPrecision = 0;
		}
		else
		{
			str_value = p_attr->value();
			pConfig->stRightAxis.iPrecision = str_value.toInt() >= 0 ? str_value.toInt() : 0;
		}

		p_attr = p_axis_node->first_attribute("Min");
		if (NULL == p_attr)
		{
			pConfig->stRightAxis.fMin = pConfig->stRightAxis.fLowBound;
		}
		else
		{
			str_value = p_attr->value();
			pConfig->stRightAxis.fMin = str_value.toDouble() > pConfig->stRightAxis.fLowBound ? pConfig->stRightAxis.fLowBound : str_value.toDouble();
		}

		p_attr = p_axis_node->first_attribute("Max");
		if (NULL == p_attr)
		{
			if (pConfig->stRightAxis.fUpBound > 0)
			{
				pConfig->stRightAxis.fMax = pConfig->stRightAxis.fUpBound * 2;
			}
			else
			{
				pConfig->stRightAxis.fMax = pConfig->stRightAxis.fUpBound / 2;
			}
		}
		else
		{
			str_value = p_attr->value();
			pConfig->stRightAxis.fMax = str_value.toDouble() > pConfig->stRightAxis.fUpBound ? str_value.toDouble() : pConfig->stRightAxis.fUpBound;
		}

		p_attr = p_axis_node->first_attribute("MinRange");
		double f_range = pConfig->stRightAxis.fUpBound - pConfig->stRightAxis.fLowBound;
		if (NULL == p_attr)
		{
			pConfig->stRightAxis.fMinRange = f_range / 2;
		}
		else
		{
			str_value = p_attr->value();
			double f_value = str_value.toDouble();
			pConfig->stRightAxis.fMinRange = f_value > f_range ? f_range : f_value;
		}

		p_attr = p_axis_node->first_attribute("Step");
		if (NULL == p_attr)
		{
			pConfig->stRightAxis.fStep = pConfig->stRightAxis.fMinRange / 5;
		}
		else
		{
			str_value = p_attr->value();
			pConfig->stRightAxis.fStep = str_value.toDouble();
		}
	}
}
//读取一导线性曲线配置 -- xiongz [2202/8/28]
void CConfig::ReadChannelLineCurve(const rapidxml::xml_node<> * pChannelNode, QVector<LineCurveInfo> *pVctLineCurve) const
{
	//参数检查
	assert(pChannelNode);
	assert(pVctLineCurve);
	//读取LineCurves节点
	rapidxml::xml_node<> *p_line_curves = pChannelNode->first_node("LineCurves");
	if (NULL == p_line_curves)
		return;
	rapidxml::xml_node<> *p_line_item = p_line_curves->first_node("item");
	if (NULL == p_line_item)
		return;
	while(p_line_item)
	{
		//读取linecurve的属性值
		LineCurveInfo st_line_info;
		QString str_value;
		//读取名称
		rapidxml::xml_attribute<> *p_attr = p_line_item->first_attribute("Name");
		if(NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.strLegend = str_value;
		//读取可见属性
		p_attr = p_line_item->first_attribute("Visible");
		if(NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.bVisible = (str_value.toInt() & 1);
		//读取数据类型
		p_attr = p_line_item->first_attribute("Data");
		if(NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.CurveData = (E_DATA_TYPE)(str_value.toInt());
		//读取线条宽度
		p_attr = p_line_item->first_attribute("Width");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.iLineWidth = str_value.toInt();
		//读取是否只见趋势
		p_attr = p_line_item->first_attribute("Trend");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.bTrendOnly = (str_value.toInt() & 1);
		//读取是否忽略无效值
		p_attr = p_line_item->first_attribute("IgnoreVacant");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.bIgnoreVacant = (str_value.toInt() & 1);
		//读取轴体类型
		p_attr = p_line_item->first_attribute("RightAligned");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.eAxis = (E_AXIS_TYPE)(str_value.toInt());
		pVctLineCurve->append(st_line_info);
		p_line_item = p_line_item->next_sibling("item");

	}
}
//读取一导中带状曲线的配置 xiongz-[2020/8/28]
void CConfig::ReadChannelBandCurve(const rapidxml::xml_node<> *pChannelNode, QVector<BandCurveInfo> *pVctBandCurve) const
{
	//参数检查
	assert(pChannelNode);
	assert(pVctBandCurve);
	//读取LineCurves节点
	rapidxml::xml_node<> *p_line_curves = pChannelNode->first_node("BandCurves");
	if (NULL == p_line_curves)
		return;
	rapidxml::xml_node<> *p_line_item = p_line_curves->first_node("item");
	if (NULL == p_line_item)
		return;
	while (p_line_item)
	{
		//读取linecurve的属性值
		BandCurveInfo st_line_info;
		QString str_value;
		//读取名称
		rapidxml::xml_attribute<> *p_attr = p_line_item->first_attribute("Name");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.strLegend = str_value;
		//读取可见属性
		p_attr = p_line_item->first_attribute("Visible");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.bVisible = (str_value.toInt() & 1);
		//读取上数据类型
		p_attr = p_line_item->first_attribute("DataUp");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.CurveData.first = (E_DATA_TYPE)(str_value.toInt());
		//读取下沿数据类型
		p_attr = p_line_item->first_attribute("DataDown");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.CurveData.second = (E_DATA_TYPE)(str_value.toInt());
		//读取线条宽度
		p_attr = p_line_item->first_attribute("Width");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.iLineWidth = str_value.toInt();
		//读取是否忽略无效值
		p_attr = p_line_item->first_attribute("IgnoreVacant");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.bIgnoreVacant = (str_value.toInt() & 1);
		//读取轴体类型
		p_attr = p_line_item->first_attribute("RightAligned");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.eAxis = (E_AXIS_TYPE)(str_value.toInt());
		pVctBandCurve->append(st_line_info);
		p_line_item = p_line_item->next_sibling("item");
	}
}
//读取一导中山峰曲线配置信息 xiongz-[2020/8/28]
void CConfig::ReadChannelAccCurve(const rapidxml::xml_node<> * pChannelNode, QVector<AccCurveInfo> *pVctAccCurve) const
{
	//参数检查
	assert(pChannelNode);
	assert(pVctAccCurve);
	//读取LineCurves节点
	rapidxml::xml_node<> *p_line_curves = pChannelNode->first_node("AccCurves");
	if (NULL == p_line_curves)
		return;
	rapidxml::xml_node<> *p_line_item = p_line_curves->first_node("item");
	if (NULL == p_line_item)
		return;
	while (p_line_item)
	{
		//读取linecurve的属性值
		AccCurveInfo st_line_info;
		QString str_value;
		//读取名称
		rapidxml::xml_attribute<> *p_attr = p_line_item->first_attribute("Name");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.strLegend = str_value;
		//读取可见属性
		p_attr = p_line_item->first_attribute("Visible");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.bVisible = (str_value.toInt() & 1);
		//读取数据类型
		p_attr = p_line_item->first_attribute("Data");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.CurveData = (E_DATA_TYPE)(str_value.toInt());
		//读取线条宽度
		p_attr = p_line_item->first_attribute("Width");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.iLineWidth = str_value.toInt();
		//读取是否忽略无效值
		p_attr = p_line_item->first_attribute("IgnoreVacant");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.bIgnoreVacant = (str_value.toInt() & 1);
		//读取轴体类型
		p_attr = p_line_item->first_attribute("RightAligned");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.eAxis = (E_AXIS_TYPE)(str_value.toInt());
		pVctAccCurve->append(st_line_info);
		p_line_item = p_line_item->next_sibling("item");

	}
}

//读取一导中所有曲线的配置信息 xiongz-[2020/8/28]
void CConfig::ReadChannelCurve(const rapidxml::xml_node<> *pChannelNode, CurveConfig *pCurveConfig) const
{
	assert(pChannelNode);
	assert(pCurveConfig);
	ReadChannelAccCurve(pChannelNode, &(pCurveConfig->vctAccCurves));
	ReadChannelBandCurve(pChannelNode, &(pCurveConfig->vctBandCurves));
	ReadChannelLineCurve(pChannelNode, &(pCurveConfig->vctLineCurves));

}
//读取导联索引xiongz-[2020/8/28]
void CConfig::ReadChannelIndex(const rapidxml::xml_node<> *pChannelNode, E_CHANNEL_INDEX *pChannelIndex) const
{
	assert(pChannelIndex);
	assert(pChannelIndex);
	QString str_value;
	rapidxml::xml_attribute<> *p_attr = pChannelNode->first_attribute("Index");
	if(NULL == p_attr)
	{
		return;
	}
	str_value = p_attr->value();
	*pChannelIndex = (E_CHANNEL_INDEX)(str_value.toInt());
}
// 读取一导指数区设置 =====================================
void CConfig::ReadChannelIndexSetup(const rapidxml::xml_node<>* pChannelNode, IndexChannelConfig* pConfig) const
{
	assert(pChannelNode);
	assert(pConfig);
	pConfig->vctIndexInfo.clear();

	rapidxml::xml_node<>* p_index_root = pChannelNode->first_node("Index");
	if (NULL == p_index_root)
	{
		return;
	}
	rapidxml::xml_attribute<>* p_attr = p_index_root->first_attribute("ShowChannelName");
	QString str_value;
	if (NULL == p_attr)
	{
		pConfig->bShowChName = false;
	}
	else
	{
		str_value = p_attr->value();
		pConfig->bShowChName = (str_value.toInt() != 0);
	}
	p_attr = p_index_root->first_attribute("ChannelName");
	if (NULL == p_attr)
	{
		pConfig->strChName = "";
	}
	else
	{
		pConfig->strChName = p_attr->value();
	}
	p_attr = p_index_root->first_attribute("Type");
	if (NULL == p_attr)
	{
		pConfig->eIndexType = E_INDEX_DISPLAY_BASIC;
	}
	else
	{
		str_value = p_attr->value();
		pConfig->eIndexType = E_INDEX_DISPLAY_TYPE(str_value.toInt());
	}
	rapidxml::xml_node<>* p_item;
	for (p_item = p_index_root->first_node("item")
		; p_item
		; p_item = p_item->next_sibling("item"))
	{
		IndexInfo st_info;

		p_attr = p_item->first_attribute("Data");
		if (NULL == p_attr)
		{
			st_info.eData = E_DATA_HR;
		}
		else
		{
			str_value = p_attr->value();
			st_info.eData = E_DATA_TYPE(str_value.toInt());
		}

		p_attr = p_item->first_attribute("ShowInfo");
		if (NULL == p_attr)
		{
			st_info.bShowInfo = false;
		}
		else
		{
			str_value = p_attr->value();
			st_info.bShowInfo = (str_value.toInt() != 0);
		}

		p_attr = p_item->first_attribute("ShowRange");
		if (NULL == p_attr)
		{
			st_info.bShowRange = false;
		}
		else
		{
			str_value = p_attr->value();
			st_info.bShowRange = (str_value.toInt() != 0);
		}

		p_attr = p_item->first_attribute("ShowUnit");
		if (NULL == p_attr)
		{
			st_info.bShowUnit = false;
		}
		else
		{
			str_value = p_attr->value();
			st_info.bShowUnit = (str_value.toInt() != 0);
		}

		p_attr = p_item->first_attribute("ShowPercent");
		if (NULL == p_attr)
		{
			st_info.bShowPercent = false;
		}
		else
		{
			str_value = p_attr->value();
			st_info.bShowPercent = (str_value.toInt() != 0);
		}

		pConfig->vctIndexInfo.append(st_info);
	}
}

// 读取一导图像缩放比率 ===================================
void CConfig::ReadChannelStretchFactor(const rapidxml::xml_node<>* pChannelNode, double* pStretchFactor) const
{
	assert(pChannelNode);
	assert(pStretchFactor);
	rapidxml::xml_attribute<>* p_attr = pChannelNode->first_attribute("StretchFactor");
	if (NULL == p_attr)
	{
		(*pStretchFactor) = 1.0;
	}
	else
	{
		QString str_value = p_attr->value();
		(*pStretchFactor) = str_value.toDouble();
	}
}

// 读取图像各导设置 =======================================
QVector<ChannelConfig> CConfig::ReadChannelsConfig()
{
	QVector<ChannelConfig> vct_ret;
	QString str_xml = file_func::GetExePath() + C_CONFIG_DIR + C_CONFIG_XML;
	QByteArray sz_xml = str_xml.toLocal8Bit();

	rapidxml::file<> f_doc(sz_xml.data());
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_default>(f_doc.data());
	rapidxml::xml_node<>* p_root = doc.first_node(C_XML_ROOT_TAG);
	if (NULL == p_root)
	{
		return vct_ret;
	}
	rapidxml::xml_node<>* p_node = p_root->first_node(C_XML_PLOT_TAG);
	if (NULL == p_node)
	{
		return vct_ret;
	}
	rapidxml::xml_node<>* p_subnode = p_node->first_node(C_XML_PLOT_CHS_TAG);
	if (NULL == p_subnode)
	{
		return vct_ret;
	}
	rapidxml::xml_node<>* p_item;
	for (p_item = p_subnode->first_node("Channel")
		; p_item
		; p_item = p_item->next_sibling("Channel"))
	{
		ChannelConfig config;
		ReadChannelStretchFactor(p_item, &(config.fStretchFactor));
		ReadChannelAxisInfo(p_item, &(config.stPlotConfig));
		ReadChannelIndexSetup(p_item, &(config.stIndexConfig));

		//add xiongz-[2020/8/28]
		ReadChannelIndex(p_item, &(config.eIndex));
		ReadChannelCurve(p_item, &(config.stCurveConfig));

		vct_ret.append(config);
	}

	m_vctNormalColors.clear();
	for (int i = 0; i < vct_ret.size(); ++i)
	{
		QMap<E_DATA_TYPE, QColor> map_colors;
		for (auto info : vct_ret.value(i).stIndexConfig.vctIndexInfo)
		{
			if (E_INDEX_DISPLAY_BRF == vct_ret.value(i).stIndexConfig.eIndexType ||
				E_INDEX_DISPLAY_HRV == vct_ret.value(i).stIndexConfig.eIndexType)
			{
				map_colors[info.eData] = m_ConfigParam.stViewConfig.clForeground;
			}
			else
			{
				map_colors[info.eData] = m_ConfigParam.mapDataInfo.value(info.eData).clLine;
			}			
		}
		m_vctNormalColors.append(map_colors);
	}
	return vct_ret;
}

// 读取一导山峰曲线绘制 ===================================
QVector<AccCurveInfo> CConfig::ReadAccCurveConfig(const int iCh) const
{
	QVector<AccCurveInfo> vct_ret;
	QString str_xml = file_func::GetExePath() + C_CONFIG_DIR + C_CONFIG_XML;
	QByteArray sz_xml = str_xml.toLocal8Bit();

	rapidxml::file<> f_doc(sz_xml.data());
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_default>(f_doc.data());
	rapidxml::xml_node<>* p_root = doc.first_node(C_XML_ROOT_TAG);
	if (NULL == p_root)
	{
		return vct_ret;
	}
	rapidxml::xml_node<>* p_node = p_root->first_node(C_XML_PLOT_TAG);
	if (NULL == p_node)
	{
		return vct_ret;
	}
	rapidxml::xml_node<>* p_subnode = p_node->first_node(C_XML_PLOT_CHS_TAG);
	if (NULL == p_subnode)
	{
		return vct_ret;
	}
	int i_curr_ch = 0;
	rapidxml::xml_node<>* p_ch = p_subnode->first_node("Channel");
	while (i_curr_ch < iCh)
	{
		p_ch = p_ch->next_sibling("Channel");
		if (NULL == p_ch)
		{
			break;
		}
		i_curr_ch++;
	}
	if (NULL == p_ch)
	{
		return vct_ret;
	}
	rapidxml::xml_node<>* p_acc_root = p_ch->first_node("AccCurves");
	if (NULL == p_acc_root)
	{
		return vct_ret;
	}
	rapidxml::xml_node<>* p_item;
	for (p_item = p_acc_root->first_node("item")
		; p_item
		; p_item = p_item->next_sibling("item"))
	{
		AccCurveInfo st_info;
		QString str_value;
		// 读取曲线名称
		rapidxml::xml_attribute<>* p_attr = p_item->first_attribute("Name");
		st_info.strLegend = (NULL == p_attr) ? "" : p_attr->value();
		// 读取曲线是否可见
		p_attr = p_item->first_attribute("Visible");
		if (NULL == p_attr)
		{
			st_info.bVisible = false;
		}
		else
		{
			str_value = p_attr->value();
			st_info.bVisible = (str_value.toInt() != 0);
		}
		// 读取曲线数据
		p_attr = p_item->first_attribute("Data");
		if (NULL == p_attr)
		{
			st_info.CurveData = E_DATA_TP;
		}
		else
		{
			str_value = p_attr->value();
			st_info.CurveData = E_DATA_TYPE(str_value.toInt());
		}
		// 读取线宽
		p_attr = p_item->first_attribute("Width");
		if (NULL == p_attr)
		{
			st_info.iLineWidth = 1;
		}
		else
		{
			str_value = p_attr->value();
			st_info.iLineWidth = str_value.toInt();
		}
		// 是否跳过无效数据
		p_attr = p_item->first_attribute("IgnoreVacant");
		if (NULL == p_attr)
		{
			st_info.bIgnoreVacant = false;
		}
		else
		{
			str_value = p_attr->value();
			st_info.bIgnoreVacant = (str_value.toInt() != 0);
		}
		// 采用左侧/右侧Y轴
		p_attr = p_item->first_attribute("RightAligned");
		if (NULL == p_attr)
		{
			st_info.eAxis = E_AXIS_LEFT;
		}
		else
		{
			str_value = p_attr->value();
			st_info.eAxis = (str_value.toInt() == 0) ? E_AXIS_LEFT : E_AXIS_RIGHT;
		}
		st_info.fBase = 0.0;
		vct_ret.append(st_info);
	}
	return vct_ret;
}

// 读取一导带状曲线设置 ===================================
QVector<BandCurveInfo> CConfig::ReadBandCurveConfig(const int iCh) const
{
	QVector<BandCurveInfo> vct_ret;
	QString str_xml = file_func::GetExePath() + C_CONFIG_DIR + C_CONFIG_XML;
	QByteArray sz_xml = str_xml.toLocal8Bit();

	rapidxml::file<> f_doc(sz_xml.data());
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_default>(f_doc.data());
	rapidxml::xml_node<>* p_root = doc.first_node(C_XML_ROOT_TAG);
	if (NULL == p_root)
	{
		return vct_ret;
	}
	rapidxml::xml_node<>* p_node = p_root->first_node(C_XML_PLOT_TAG);
	if (NULL == p_node)
	{
		return vct_ret;
	}
	rapidxml::xml_node<>* p_subnode = p_node->first_node(C_XML_PLOT_CHS_TAG);
	if (NULL == p_subnode)
	{
		return vct_ret;
	}
	int i_curr_ch = 0;
	rapidxml::xml_node<>* p_ch = p_subnode->first_node("Channel");
	while (i_curr_ch < iCh)
	{
		p_ch = p_ch->next_sibling("Channel");
		if (NULL == p_ch)
		{
			break;
		}
		i_curr_ch++;
	}
	if (NULL == p_ch)
	{
		return vct_ret;
	}
	rapidxml::xml_node<>* p_acc_root = p_ch->first_node("BandCurves");
	if (NULL == p_acc_root)
	{
		return vct_ret;
	}
	rapidxml::xml_node<>* p_item;
	for (p_item = p_acc_root->first_node("item")
		; p_item
		; p_item = p_item->next_sibling("item"))
	{
		BandCurveInfo st_info;
		QString str_value;
		rapidxml::xml_attribute<>* p_attr = p_item->first_attribute("Name");
		st_info.strLegend = (NULL == p_attr) ? "" : p_attr->value();
		p_attr = p_item->first_attribute("Visible");
		if (NULL == p_attr)
		{
			st_info.bVisible = false;
		}
		else
		{
			str_value = p_attr->value();
			st_info.bVisible = (str_value.toInt() != 0);
		}
		rapidxml::xml_attribute<>* p_attr_data_up = p_item->first_attribute("DataUp");
		rapidxml::xml_attribute<>* p_attr_data_Down = p_item->first_attribute("DataDown");
		if (NULL == p_attr_data_up || NULL == p_attr_data_Down)
		{
			st_info.CurveData.first = E_DATA_DIA;
			st_info.CurveData.second = E_DATA_SYS;
		}
		else
		{
			QString str_value_up = p_attr_data_up->value();
			QString str_value_down = p_attr_data_Down->value();
			if (str_value_up == str_value_down)
			{
				st_info.CurveData.first = E_DATA_DIA;
				st_info.CurveData.second = E_DATA_SYS;
			}
			else
			{
				st_info.CurveData.first = E_DATA_TYPE(str_value_down.toInt());
				st_info.CurveData.second = E_DATA_TYPE(str_value_up.toInt());
			}
		}
		p_attr = p_item->first_attribute("Width");
		if (NULL == p_attr)
		{
			st_info.iLineWidth = 1;
		}
		else
		{
			str_value = p_attr->value();
			st_info.iLineWidth = str_value.toInt();
		}
		p_attr = p_item->first_attribute("IgnoreVacant");
		if (NULL == p_attr)
		{
			st_info.bIgnoreVacant = false;
		}
		else
		{
			str_value = p_attr->value();
			st_info.bIgnoreVacant = (str_value.toInt() != 0);
		}
		p_attr = p_item->first_attribute("RightAligned");
		if (NULL == p_attr)
		{
			st_info.eAxis = E_AXIS_LEFT;
		}
		else
		{
			str_value = p_attr->value();
			st_info.eAxis = (str_value.toInt() == 0) ? E_AXIS_LEFT : E_AXIS_RIGHT;
		}
		vct_ret.append(st_info);
	}
	return vct_ret;
}

// 读取一导中线状曲线设置 =================================
QVector<LineCurveInfo> CConfig::ReadLineCurveConfig(const int iCh) const
{
	QVector<LineCurveInfo> vct_ret;
	QString str_xml = file_func::GetExePath() + C_CONFIG_DIR + C_CONFIG_XML;
	QByteArray sz_xml = str_xml.toLocal8Bit();

	rapidxml::file<> f_doc(sz_xml.data());
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_default>(f_doc.data());
	rapidxml::xml_node<>* p_root = doc.first_node(C_XML_ROOT_TAG);
	if (NULL == p_root)
	{
		return vct_ret;
	}
	rapidxml::xml_node<>* p_node = p_root->first_node(C_XML_PLOT_TAG);
	if (NULL == p_node)
	{
		return vct_ret;
	}
	rapidxml::xml_node<>* p_subnode = p_node->first_node(C_XML_PLOT_CHS_TAG);
	if (NULL == p_subnode)
	{
		return vct_ret;
	}
	int i_curr_ch = 0;
	rapidxml::xml_node<>* p_ch = p_subnode->first_node("Channel");
	while (i_curr_ch < iCh)
	{
		p_ch = p_ch->next_sibling("Channel");
		if (NULL == p_ch)
		{
			break;
		}
		i_curr_ch++;
	}
	if (NULL == p_ch)
	{
		return vct_ret;
	}
	rapidxml::xml_node<>* p_acc_root = p_ch->first_node("LineCurves");
	if (NULL == p_acc_root)
	{
		return vct_ret;
	}
	if (NULL == p_acc_root)
	{
		return vct_ret;
	}
	rapidxml::xml_node<>* p_item;
	for (p_item = p_acc_root->first_node("item")
		; p_item
		; p_item = p_item->next_sibling("item"))
	{
		LineCurveInfo st_info;
		QString str_value;
		rapidxml::xml_attribute<>* p_attr = p_item->first_attribute("Name");
		st_info.strLegend = (NULL == p_attr) ? "" : p_attr->value();
		p_attr = p_item->first_attribute("Visible");
		if (NULL == p_attr)
		{
			st_info.bVisible = false;
		}
		else
		{
			str_value = p_attr->value();
			st_info.bVisible = (str_value.toInt() != 0);
		}
		p_attr = p_item->first_attribute("Data");
		if (NULL == p_attr)
		{
			st_info.CurveData = E_DATA_VML;
		}
		else
		{
			str_value = p_attr->value();
			st_info.CurveData = E_DATA_TYPE(str_value.toInt());
		}
		p_attr = p_item->first_attribute("Width");
		if (NULL == p_attr)
		{
			st_info.iLineWidth = 1;
		}
		else
		{
			str_value = p_attr->value();
			st_info.iLineWidth = str_value.toInt();
		}
		p_attr = p_item->first_attribute("Trend");
		if (NULL == p_attr)
		{
			st_info.bTrendOnly = false;
		}
		else
		{
			str_value = p_attr->value();
			st_info.bTrendOnly = (str_value.toInt() != 0);
		}
		p_attr = p_item->first_attribute("IgnoreVacant");
		if (NULL == p_attr)
		{
			st_info.bIgnoreVacant = false;
		}
		else
		{
			str_value = p_attr->value();
			st_info.bIgnoreVacant = (str_value.toInt() != 0);
		}
		p_attr = p_item->first_attribute("RightAligned");
		if (NULL == p_attr)
		{
			st_info.eAxis = E_AXIS_LEFT;
		}
		else
		{
			str_value = p_attr->value();
			st_info.eAxis = (str_value.toInt() == 0) ? E_AXIS_LEFT : E_AXIS_RIGHT;
		}
		vct_ret.append(st_info);
	}
	return vct_ret;
}

// 设置某导数据上下限 =====================================
void CConfig::SetScaleRange(const int iCh, E_AXIS_TYPE eAxis, const double fLowValue, const double fUpValue)
{
	if (fLowValue == fUpValue)
	{
		return;
	}

	QString str_xml = file_func::GetExePath() + C_CONFIG_DIR + C_CONFIG_XML;
	Q_ASSERT(file_func::IsFileExist(str_xml));
	QByteArray sz_xml = str_xml.toLocal8Bit();
	rapidxml::file<> f_doc(sz_xml.data());
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_no_data_nodes | rapidxml::parse_full>(f_doc.data());
	rapidxml::xml_node<>* p_root = doc.first_node(C_XML_ROOT_TAG);
	Q_ASSERT(p_root);
	rapidxml::xml_node<>* p_node = p_root->first_node(C_XML_PLOT_TAG);
	Q_ASSERT(p_node);
	rapidxml::xml_node<>* p_subnode = p_node->first_node(C_XML_PLOT_CHS_TAG);
	Q_ASSERT(p_subnode);
	int i_curr_ch = 0;
	rapidxml::xml_node<>* p_ch = p_subnode->first_node("Channel");
	while (i_curr_ch < iCh)
	{
		p_ch = p_ch->next_sibling("Channel");
		if (NULL == p_ch)
		{
			break;
		}
		i_curr_ch++;
	}
	if (NULL == p_ch)
	{
		return;
	}
	PlotChannelConfig config;
	ReadChannelAxisInfo(p_ch, &config);
	if ((E_AXIS_LEFT == eAxis && !config.stLeftAxis.IsValid())
		|| (E_AXIS_RIGHT == eAxis && !config.stRightAxis.IsValid()))
	{
		return;
	}
	double f_valuedown = fLowValue > fUpValue ? fUpValue : fLowValue;
	double f_valueup = fLowValue < fUpValue ? fUpValue : fLowValue;
	QByteArray sz_node;
	if (E_AXIS_LEFT == eAxis)
	{
		sz_node = "LeftAxis";
		if (f_valuedown < config.stLeftAxis.fMin 
			|| f_valueup > config.stLeftAxis.fMax 
			|| (f_valueup - f_valuedown) < config.stLeftAxis.fMinRange)
		{
			return;
		}
	}
	else
	{
		sz_node = "RightAxis";
		if (f_valuedown < config.stRightAxis.fMin
			|| f_valueup > config.stRightAxis.fMax
			|| (f_valueup - f_valuedown) < config.stRightAxis.fMinRange)
		{
			return;
		}
	}
	rapidxml::xml_node<>* p_axis = p_ch->first_node(sz_node.data());
	if (NULL == p_axis)
	{
		return;
	}

	rapidxml::xml_attribute<>* p_attr = p_axis->first_attribute("UpBound");
	char szvalue[16] = { 0 };
	sprintf(szvalue, "%.2f", f_valueup);
	if (NULL == p_attr)
	{
		p_axis->append_attribute(doc.allocate_attribute("UpBound", doc.allocate_string(szvalue)));
	}
	else
	{
		p_attr->value(doc.allocate_string(szvalue));
	}

	p_attr = p_axis->first_attribute("DownBound");
	sprintf(szvalue, "%.2f", f_valuedown);
	if (NULL == p_attr)
	{
		p_axis->append_attribute(doc.allocate_attribute("DownBound", doc.allocate_string(szvalue)));
	}
	else
	{
		p_attr->value(doc.allocate_string(szvalue));
	}
	// 写入文件
	std::string str_text;
	rapidxml::print(std::back_inserter(str_text), doc, 0);
	std::ofstream out(sz_xml.data());
	out << doc;
}

// 读取字体 ===============================================
QFont CConfig::ReadFont(const rapidxml::xml_node<>* pFonts, QString strItem)
{
	Q_ASSERT(pFonts);
	Q_ASSERT(!strItem.isEmpty());
	QFont font(C_DEF_FONT_FAMILY, C_DEF_FONT_SIZE);
	QByteArray byte_item = strItem.toLocal8Bit();
	rapidxml::xml_node<>* p_item = pFonts->first_node(byte_item.data());
	QString str_value;
	if (NULL == p_item)
	{
		return font;
	}
	rapidxml::xml_attribute<>* p_attr = p_item->first_attribute("family");
	if (NULL != p_attr)
	{
		font.setFamily(p_attr->value());
	}
	p_attr = p_item->first_attribute("size");
	if (NULL != p_attr)
	{
		str_value = p_attr->value();
		int i_size = str_value.toInt();
		font.setPointSize(i_size);
	}
	p_attr = p_item->first_attribute("bold");
	if (NULL != p_attr)
	{
		str_value = p_attr->value();
		int i_value = str_value.toInt();
		font.setBold(i_value != 0);
	}
	return font;
}

// 设置指数报警上下限======================================
void CConfig::SetAlertRange(const E_DATA_TYPE eData, const double fLowBound, const double fUpBound)
{
	if (fLowBound >= fUpBound)
	{
		return;
	}
	if (!m_ConfigParam.mapDataInfo.contains(eData))
	{
		return;
	}

	if (m_ConfigParam.mapDataInfo.value(eData).fMin == fLowBound
		&& m_ConfigParam.mapDataInfo.value(eData).fMax == fUpBound)
	{
		return;
	}

	QString str_xml = file_func::GetExePath() + C_CONFIG_DIR + C_CONFIG_XML;
	Q_ASSERT(file_func::IsFileExist(str_xml));
	QByteArray sz_xml = str_xml.toLocal8Bit();
	rapidxml::file<> f_doc(sz_xml.data());
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_no_data_nodes | rapidxml::parse_full>(f_doc.data());
	rapidxml::xml_node<>* p_root = doc.first_node(C_XML_ROOT_TAG);
	Q_ASSERT(p_root);
	rapidxml::xml_node<>* p_node = p_root->first_node(C_XML_DATA_TAG);
	Q_ASSERT(p_node);
	rapidxml::xml_node<>* p_item;
	rapidxml::xml_attribute<>* p_attr;
	for (p_item = p_node->first_node("item")
		; p_item
		; p_item = p_item->next_sibling("item"))
	{
		p_attr = p_item->first_attribute("ID");
		if (NULL == p_attr)
		{
			continue;
		}
		QString str_value = p_attr->value();
		if (eData == str_value.toInt())
		{
			break;
		}
	}
	if (NULL == p_item)
	{
		return;
	}
	char szvalue[16] = { 0 };
	p_attr = p_item->first_attribute("Min");
	sprintf(szvalue, "%.2f", fLowBound);
	if (NULL == p_attr)
	{
		p_item->append_attribute(doc.allocate_attribute("Min", doc.allocate_string(szvalue)));
	}
	else
	{
		p_attr->value(doc.allocate_string(szvalue));
	}

	p_attr = p_item->first_attribute("Max");
	sprintf(szvalue, "%.2f", fUpBound);
	if (NULL == p_attr)
	{
		p_item->append_attribute(doc.allocate_attribute("Max", doc.allocate_string(szvalue)));
	}
	else
	{
		p_attr->value(doc.allocate_string(szvalue));
	}
	// 写入文件
	std::string str_text;
	rapidxml::print(std::back_inserter(str_text), doc, 0);
	std::ofstream out(sz_xml.data());
	out << doc;
}

// 设置数据对应颜色 =======================================
void CConfig::SetDataColor(const E_DATA_TYPE eData, const QColor& clNew)
{
	if (!m_ConfigParam.mapDataInfo.contains(eData))
	{
		return;
	}
	if (m_ConfigParam.mapDataInfo.value(eData).clLine == clNew)
	{
		return;
	}
	QString str_xml = file_func::GetExePath() + C_CONFIG_DIR + C_CONFIG_XML;
	Q_ASSERT(file_func::IsFileExist(str_xml));
	QByteArray sz_xml = str_xml.toLocal8Bit();
	rapidxml::file<> f_doc(sz_xml.data());
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_no_data_nodes | rapidxml::parse_full>(f_doc.data());
	rapidxml::xml_node<>* p_root = doc.first_node(C_XML_ROOT_TAG);
	Q_ASSERT(p_root);
	rapidxml::xml_node<>* p_node = p_root->first_node(C_XML_DATA_TAG);
	Q_ASSERT(p_node);
	rapidxml::xml_node<>* p_item;
	rapidxml::xml_attribute<>* p_attr;
	for (p_item = p_node->first_node("item")
		; p_item
		; p_item = p_item->next_sibling("item"))
	{
		p_attr = p_item->first_attribute("ID");
		if (NULL == p_attr)
		{
			continue;
		}
		QString str_value = p_attr->value();
		if (eData == str_value.toInt())
		{
			break;
		}
	}
	if (NULL == p_item)
	{
		return;
	}
	char szvalue[16] = { 0 };
	sprintf(szvalue, "%x", clNew.rgb());
	p_attr = p_item->first_attribute("Color");
	if (NULL == p_attr)
	{
		p_item->append_attribute(doc.allocate_attribute("Color", doc.allocate_string(szvalue)));
	}
	else
	{
		p_attr->value(doc.allocate_string(szvalue));
	}

	// 写入文件
	std::string str_text;
	rapidxml::print(std::back_inserter(str_text), doc, 0);
	std::ofstream out(sz_xml.data());
	out << doc;
}

void CConfig::ReadReserveWidth()
{
	QString str_xml = file_func::GetExePath() + C_CONFIG_DIR + C_CONFIG_XML;
	Q_ASSERT(file_func::IsFileExist(str_xml));
	QByteArray sz_xml = str_xml.toLocal8Bit();
	rapidxml::file<> xml_file(sz_xml);
	rapidxml::xml_document<> xml_doc;
	xml_doc.parse<0>(xml_file.data());
	rapidxml::xml_node<> *p_node_root = xml_doc.first_node(C_XML_ROOT_TAG);
	if(NULL == p_node_root)
	{
		return;
	}
	rapidxml::xml_node<> *p_node = p_node_root->first_node(C_XML_PLOT_TAG);
	if(NULL == p_node)
	{
		return;
	}
	rapidxml::xml_node<> *p_node_reserve = p_node->first_node(C_XML_RESERVE_WIDTH_TAG);
	if(NULL == p_node_reserve)
	{
		return;
	}
	QString str_reserve = QString(QLatin1String(p_node_reserve->value()));
	m_fReserverWidth = str_reserve.toDouble() / C_INCH_CM;
}
//获取预留窗口像素个数
int CConfig::GetReservedWidth() const
{
	int x_dpi = 0;
	QDesktopWidget *p_desk = QApplication::desktop();
	x_dpi = p_desk->physicalDpiX();
	return (x_dpi * m_fReserverWidth);
}

//===================选段配置读取==================
void CConfig::ReadSegmentConfig()
{
	QString str_xml = file_func::GetExePath() + C_CONFIG_DIR + C_CONFIG_XML;
	QByteArray sz_xml = str_xml.toLocal8Bit();
	QString str_value;	//	接受属性值
	SegmentView st_seg_view;
	//解析xml文件
	rapidxml::file<> f_doc(sz_xml.data());
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_default>(f_doc.data());
	//遍历节点
	//根节点sysconfig
	rapidxml::xml_node<> *p_root = doc.first_node(C_XML_ROOT_TAG);
	if (NULL == p_root)
	{
		return;
	}
	//titile节点
	rapidxml::xml_node<> *p_node_seg = p_root->first_node(C_XML_SEGMENT_TAG);
	rapidxml::xml_attribute<> *p_attr = NULL;
	if (NULL == p_node_seg)
	{
		return;
	}
	p_attr = p_node_seg->first_attribute("width");
	if (NULL == p_attr)
	{
		return;
	}
	str_value = p_attr->value();
	st_seg_view.iWidth = str_value.toInt();
	//读取字体信息
	rapidxml::xml_node<> *p_node_title = p_node_seg->first_node(C_XML_SEG_TITLE_TAG);
	if (NULL == p_node_title)
	{
		return;
	}
	
	p_attr = p_node_title->first_attribute("family");
	if (NULL == p_attr)
	{
		return;
	}
	str_value = p_attr->value();
	p_attr = NULL;
	st_seg_view.fontTitle.setFamily(str_value);
	p_attr = p_node_title->first_attribute("size");
	if (NULL == p_attr)
	{
		return;
	}
	str_value = p_attr->value();
	p_attr = NULL;
	st_seg_view.fontTitle.setPixelSize(str_value.toInt());
	p_attr = p_node_title->first_attribute("bold");
	if (NULL == p_attr)
	{
		return;
	}
	str_value = p_attr->value();
	p_attr = NULL;
	bool b_title_bold = (str_value.toInt() > 0);
	st_seg_view.fontTitle.setBold(b_title_bold);
	p_attr = p_node_title->first_attribute("color");
	if (NULL == p_attr)
	{
		return;
	}
	str_value = p_attr->value();
	p_attr = NULL;
	st_seg_view.colorTitle.setNamedColor(str_value);
	//Channels节点
	rapidxml::xml_node<> *p_node_channels = p_node_seg->first_node(C_XML_SEG_CHANNELS_TAG);
	if (NULL == p_node_channels)
	{
		return;
	}
	p_attr = p_node_channels->first_attribute("family");
	if (NULL == p_attr)
	{
		return;
	}
	str_value = p_attr->value();
	p_attr = NULL;
	st_seg_view.fontCurveIndex.setFamily(str_value);
	p_attr = p_node_channels->first_attribute("size");
	if (NULL == p_attr)
	{
		return;
	}
	str_value = p_attr->value();
	p_attr = NULL;
	st_seg_view.fontCurveIndex.setPixelSize(str_value.toInt());
	p_attr = p_node_channels->first_attribute("bold");
	if (NULL == p_attr)
	{
		return;
	}
	str_value = p_attr->value();
	p_attr = NULL;
	bool b_channels_bold = (str_value.toInt() > 0);
	st_seg_view.fontCurveIndex.setBold(b_channels_bold);
	//Layout节点
	for (rapidxml::xml_node<> *p_node_layout = p_node_channels->first_node("Layout");
		NULL != p_node_layout;
		p_node_layout = p_node_layout->next_sibling())
	{
		rapidxml::xml_attribute<> *p_attr_layout = p_node_layout->first_attribute("LayoutType");
		if (NULL == p_attr_layout)
		{
			return;
		}
		str_value = p_attr_layout->value();
		E_LAYOUT_TYPE e_layout = (E_LAYOUT_TYPE)(str_value.toInt());
		//channel节点
		for (rapidxml::xml_node<> *p_node_channel = p_node_layout->first_node();
			NULL != p_node_channel;
			p_node_channel = p_node_channel->next_sibling())
		{
			SegmentChannelMeasure st_measure;
			SegmentChannelParam st_channel_param;
			E_CHANNEL_TYPE e_type;
			//获取属性值
			p_attr = p_node_channel->first_attribute("index");
			if (NULL == p_attr)
			{
				return;
			}
			str_value = p_attr->value();
			e_type = (E_CHANNEL_TYPE)(str_value.toInt());
			st_measure.eType = e_type;
			p_attr = p_node_channel->first_attribute("count");
			if (NULL == p_attr)
			{
				return;
			}
			str_value = p_attr->value();
			st_measure.iCount = str_value.toInt();
			//弃用，每一导数据下的曲线顺序从布局中传入
			for (rapidxml::xml_node<> *p_node_item = p_node_channel->first_node();
				NULL != p_node_item;
				p_node_item = p_node_item->next_sibling())
			{
				SegmentCurveParam st_curve;
				p_attr = p_node_item->first_attribute("type");
				if (NULL == p_attr)
				{
					return;
				}
				str_value = p_attr->value();
				st_curve.eType = (E_DATA_TYPE)(str_value.toInt());
				p_attr = p_node_item->first_attribute("index");
				if (NULL == p_attr)
				{
					return;
				}
				str_value = p_attr->value();
				st_curve.iIndex = str_value.toInt();

				st_measure.vctCurveParam.append(st_curve);
			}
			m_SegmentConfig.mapChannel[e_layout].append(st_measure);
			//m_SegmentConfig.mapChannelMeasure.insert(e_type, st_measure);
		}
	}
	m_SegmentConfig.stView = st_seg_view;
}
void CConfig::ReadEventMarksStyle()
{
	QString str_xml = file_func::GetExePath() + C_CONFIG_DIR + C_CONFIG_XML;
	QByteArray sz_xml = str_xml.toLocal8Bit();
	QString str_value;	//	接受属性值
	QFont font;
	MarkStyleSheetParam st_mark;
	BorderParam st_border;
	rapidxml::file<> fdoc(sz_xml.data());
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_default>(fdoc.data());
	rapidxml::xml_attribute<> *p_attr_value = NULL;
	rapidxml::xml_node<> *p_node_root = doc.first_node("SysConfig");
	if (NULL == p_node_root)
	{
		return;
	}
	rapidxml::xml_node<> *p_node_event_marks = p_node_root->first_node("EventMarks");
	if (NULL == p_node_event_marks)
	{
		return;
	}
	rapidxml::xml_node<> *p_node_font = p_node_event_marks->first_node("Font");
	if (NULL == p_node_font)
	{
		return;
	}
	
	p_attr_value = p_node_font->first_attribute("family");
	if (NULL == p_attr_value)
	{
		return;
	}
	str_value = p_attr_value->value();
	font.setFamily(str_value);
	p_attr_value = p_node_font->first_attribute("size");
	if (NULL == p_attr_value)
	{
		return;
	}
	str_value = p_attr_value->value();
	font.setPixelSize(str_value.toInt());
	p_attr_value = p_node_font->first_attribute("bold");
	if (NULL == p_attr_value)
	{
		return;
	}
	str_value = p_attr_value->value();
	font.setBold(str_value.toInt() > 0 ? true:false);
	st_mark.fontMark = font;
	rapidxml::xml_node<> *p_node_mark = p_node_event_marks->first_node("Mark");
	while (p_node_mark)
	{
		E_EVENT_TYPE e_event_type = E_EVENT_NONE;
		p_attr_value = p_node_mark->first_attribute("type");
		if (NULL == p_attr_value)
		{
			return;
		}
		str_value = p_attr_value->value();
		e_event_type = (E_EVENT_TYPE)(str_value.toInt());
		p_attr_value = p_node_mark->first_attribute("background");
		if (NULL == p_attr_value)
		{
			return;
		}
		str_value = p_attr_value->value();
		st_mark.colorBackground.setNamedColor(str_value);
		p_attr_value = p_node_mark->first_attribute("description");
		if (NULL == p_attr_value)
		{
			return;
		}
		str_value = p_attr_value->value();
		st_mark.strDescripiton = str_value;
		rapidxml::xml_node<> *p_node_border = p_node_mark->first_node("Border");
		if (NULL == p_node_border)
		{
			return;
		}
		p_attr_value = p_node_border->first_attribute("style");
		if (NULL == p_attr_value)
		{
			return;
		}
		str_value = p_attr_value->value();
		st_border.strStyle = str_value;
		p_attr_value = p_node_border->first_attribute("width");
		if (NULL == p_attr_value)
		{
			return;
		}
		str_value = p_attr_value->value();
		st_border.iWidth = str_value.toInt();
		p_attr_value = p_node_border->first_attribute("color");
		if (NULL == p_attr_value)
		{
			return;
		}
		str_value = p_attr_value->value();
		st_border.colorBorder.setNamedColor(str_value);
		st_mark.stBorder = st_border;
		m_mapMarksStyleParam.insert(e_event_type, st_mark);
		p_node_mark = p_node_mark->next_sibling();
	}



}