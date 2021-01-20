/********************************************************************
* Copyright (C) �����е�����ҽ���豸�ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�Config.cpp
* ժ    Ҫ��
*		ʵ�������ļ���д����
* ��ʷ��¼��
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

// �������� ===============================================
const int C_PAGE_TIME_1 = 20 * 60;		// ÿҳ��ʾ20����
const int C_PAGE_TIME_2 = 30 * 60;		// ÿҳ��ʾ30����
const int C_PAGE_TIME_3 = 40 * 60;		// ÿҳ��ʾ40����
const int C_PAGE_TIME_4 = 50 * 60;		// ÿҳ��ʾ50����
const int C_PAGE_TIME_5 = 60 * 60;		// ÿҳ��ʾ60����
const int C_PAGE_TIME_6 = 80 * 60;		// ÿҳ��ʾ80����
const int C_PAGE_TIME_7 = 100 * 60;		// ÿҳ��ʾ100����
const int C_PAGE_TIME_8 = 120 * 60;		// ÿҳ��ʾ120����
const int C_PAGE_TIME_9 = 150 * 60;		// ÿҳ��ʾ150����
const int C_PAGE_TIME_10 = 180 * 60;		// ÿҳ��ʾ180����
const int C_DEF_DATA_FREQ = 20;		// Ĭ������125Hz
const int C_DEF_MIN_SEG_TIME = 30;		// Ĭ��30��Сѡ��

const char C_XML_ROOT_TAG[] = "SysConfig";
const char C_XML_PLOT_TAG[] = "PlotSetup";
const char C_XML_PLOT_PAGETIME_TAG[] = "PageTimeScale";
const char C_XML_DATAFREQ_TAG[] = "DataFreqSetup";
const char C_XML_PLOT_CHS_TAG[] = "RawPlot";
const char C_XML_PLOT_MINSEGLEN_TAG[] = "MinSegLen";
const char C_XML_PLOT_CHSSETUP_TAG[] = "ChannelsSetup";
const char C_XML_DATA_TAG[] = "DataType";
//ѡ������	 @xiongz [2020/11/17]
const char C_XML_SEGMENT_TAG[] = "Segment";
const char C_XML_SEG_TITLE_TAG[] = "Title";
const char C_XML_SEG_CHANNELS_TAG[] = "Channels";
const char C_XML_SEG_CHANNEL_TAG[] = "Channel";

const char C_XML_RESERVE_WIDTH_TAG[] = "ReservedWidth";	//add <2020/9/27> xiongz

// ��ɫ���� -----------------------------------------------
const QColor C_COLOR_GRID(205, 205, 205);		// ��������ɫ
const QColor C_COLOR_INVALID(150, 150, 150);
const QColor C_COLOR_BACKGROUND = Qt::white;	// ����ɫ
const QColor C_COLOR_FRONTGROUND = Qt::black;

// �߿� ---------------------------------------------------
const int C_INVALID_WIDTH = 3;				// ��Ч�����߿�3����
const int C_VERTICAL_GRID = 5;				// ��ֱ�ߣ���X��ȷ�5�ȷ֣�6���ߣ�
const int C_SCALE_MARGIN_H = 3;				// �̶�ֵˮƽ�߾�
const int C_SCALE_MARGIN_V = 4;				// �̶�ֵ��ֱ�߾�
const int C_LABEL_SPACE = 6;				// ��������֮����
const int C_CHANNEL_SPACE = 1;				// ����ͼ��֮����1����
const int C_LEGEND_WIDTH = 12;				// �������ƿ��
const int C_INDEX_WIDTH = 149;				// ָ��������
const int C_INDEX_HMARGIN = 5;
const int C_INDEX_VMARGIN = 3;
const int C_TIME_LABEL_HEIGHT = 14;			// ʱ������ߴ�
											// ���塢�ֺ� ---------------------------------------------
const QString C_SCALE_FONT_FAMILY = "Arial Narrow";		// ����Ϊ΢���źڣ�
const int C_SCALE_FONT_SIZE = 8;						// �̶�ֵ�����С
const int C_LEGEND_FONT_SIZE = 9;

const QString C_DEF_FONT_FAMILY = "Microsoft YaHei";	// Ĭ������Ϊ΢���ź�
const int C_DEF_FONT_SIZE = 10;

const double C_DEF_TITLE_FACTOR = 0.25;
const double C_DEF_TP_HEIGHT_FACTOR = 0.5;
const int C_DEF_INDEX_BACK_ALPHA = 150;
const int C_DEF_LINE_ALPHA = 120;		// Ĭ��͸����

const int C_DEF_Y_DIV = 4;				// Ĭ�Ͻ�Y���Ϊ4�ȷ�
const int C_DEF_Y_MAX = 100;			// Ĭ��Y�����ֵΪ100
const int C_DEF_LINE_WIDTH = 1;			// Ĭ�����߿��

// �������� ===============================================
CConfig* CConfig::m_pInstance = NULL;
QMutex CConfig::m_Mutex;

// ������ȡ ===============================================
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

// �ͷŵ��� ===============================================
void CConfig::ReleaseInstacne()
{
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

// ����ʱ����ȡXML�ļ� ====================================
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

	// ��Setup�ļ��в����ڣ��򴴽��ļ���
	QString str_dir = file_func::GetExePath() + C_CONFIG_DIR;
	if (!file_func::IsDirExist(str_dir))
	{
		file_func::MakeDir(str_dir);
	}
	// ��xml�ļ������ڣ����Ĭ��λ�ÿ���
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

	//ѡ������
	ReadSegmentConfig();
	ReadEventMarksStyle();
}

CConfig::~CConfig()
{
	m_vctPageDataLen.clear();
	m_ConfigParam.mapDataInfo.clear();
}

// ��ȡÿҳʱ��ѡ�� =======================================
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
	// ��ȡ����ˢ����
	rapidxml::xml_node<>* p_node = p_root->first_node(C_XML_DATAFREQ_TAG);
	if (NULL != p_node)
	{
		m_ConfigParam.iDataFreq = std::strtoul(p_node->value(), NULL, 0);
	}

	// ��ȡ��λ
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

// ��ȡѡ����Сʱ�� =======================================
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

// ��ȡ��һ��ҳ�泤�� =====================================
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

// ��ȡ��һ��ҳ�泤�� =====================================
int CConfig::GetPrevPageDataLen(const int iCurrPageLen) const
{
	int i_len_max = m_vctPageDataLen.last();
	int i_ret = iCurrPageLen;
	if (iCurrPageLen < m_vctPageDataLen.first())
	{
		i_ret = -1;				// ��λ������͵�λ���޷������á�+����ť�Ŵ�
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

// ��ȡ��ǰ�����Ǹ�����һ���������� =======================
int CConfig::GetDataSrcIndex(const int iCurrPageLen) const
{
	int i_ret = -1;
	int i_len_max = m_vctPageDataLen.last();
	if (iCurrPageLen < m_vctPageDataLen.first())
	{
		i_ret = -1;					// ������͵�λ������ԭʼ������Ϊ����Դ
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

// ��ȡ��ʾ������� =======================================
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

// ��ȡ�������� ===========================================
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

// ��ȡһ����Y������ ======================================
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
	// �������
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
	
	// �Ҳ�����
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
//��ȡһ�������������� -- xiongz [2202/8/28]
void CConfig::ReadChannelLineCurve(const rapidxml::xml_node<> * pChannelNode, QVector<LineCurveInfo> *pVctLineCurve) const
{
	//�������
	assert(pChannelNode);
	assert(pVctLineCurve);
	//��ȡLineCurves�ڵ�
	rapidxml::xml_node<> *p_line_curves = pChannelNode->first_node("LineCurves");
	if (NULL == p_line_curves)
		return;
	rapidxml::xml_node<> *p_line_item = p_line_curves->first_node("item");
	if (NULL == p_line_item)
		return;
	while(p_line_item)
	{
		//��ȡlinecurve������ֵ
		LineCurveInfo st_line_info;
		QString str_value;
		//��ȡ����
		rapidxml::xml_attribute<> *p_attr = p_line_item->first_attribute("Name");
		if(NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.strLegend = str_value;
		//��ȡ�ɼ�����
		p_attr = p_line_item->first_attribute("Visible");
		if(NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.bVisible = (str_value.toInt() & 1);
		//��ȡ��������
		p_attr = p_line_item->first_attribute("Data");
		if(NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.CurveData = (E_DATA_TYPE)(str_value.toInt());
		//��ȡ�������
		p_attr = p_line_item->first_attribute("Width");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.iLineWidth = str_value.toInt();
		//��ȡ�Ƿ�ֻ������
		p_attr = p_line_item->first_attribute("Trend");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.bTrendOnly = (str_value.toInt() & 1);
		//��ȡ�Ƿ������Чֵ
		p_attr = p_line_item->first_attribute("IgnoreVacant");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.bIgnoreVacant = (str_value.toInt() & 1);
		//��ȡ��������
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
//��ȡһ���д�״���ߵ����� xiongz-[2020/8/28]
void CConfig::ReadChannelBandCurve(const rapidxml::xml_node<> *pChannelNode, QVector<BandCurveInfo> *pVctBandCurve) const
{
	//�������
	assert(pChannelNode);
	assert(pVctBandCurve);
	//��ȡLineCurves�ڵ�
	rapidxml::xml_node<> *p_line_curves = pChannelNode->first_node("BandCurves");
	if (NULL == p_line_curves)
		return;
	rapidxml::xml_node<> *p_line_item = p_line_curves->first_node("item");
	if (NULL == p_line_item)
		return;
	while (p_line_item)
	{
		//��ȡlinecurve������ֵ
		BandCurveInfo st_line_info;
		QString str_value;
		//��ȡ����
		rapidxml::xml_attribute<> *p_attr = p_line_item->first_attribute("Name");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.strLegend = str_value;
		//��ȡ�ɼ�����
		p_attr = p_line_item->first_attribute("Visible");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.bVisible = (str_value.toInt() & 1);
		//��ȡ����������
		p_attr = p_line_item->first_attribute("DataUp");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.CurveData.first = (E_DATA_TYPE)(str_value.toInt());
		//��ȡ������������
		p_attr = p_line_item->first_attribute("DataDown");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.CurveData.second = (E_DATA_TYPE)(str_value.toInt());
		//��ȡ�������
		p_attr = p_line_item->first_attribute("Width");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.iLineWidth = str_value.toInt();
		//��ȡ�Ƿ������Чֵ
		p_attr = p_line_item->first_attribute("IgnoreVacant");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.bIgnoreVacant = (str_value.toInt() & 1);
		//��ȡ��������
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
//��ȡһ����ɽ������������Ϣ xiongz-[2020/8/28]
void CConfig::ReadChannelAccCurve(const rapidxml::xml_node<> * pChannelNode, QVector<AccCurveInfo> *pVctAccCurve) const
{
	//�������
	assert(pChannelNode);
	assert(pVctAccCurve);
	//��ȡLineCurves�ڵ�
	rapidxml::xml_node<> *p_line_curves = pChannelNode->first_node("AccCurves");
	if (NULL == p_line_curves)
		return;
	rapidxml::xml_node<> *p_line_item = p_line_curves->first_node("item");
	if (NULL == p_line_item)
		return;
	while (p_line_item)
	{
		//��ȡlinecurve������ֵ
		AccCurveInfo st_line_info;
		QString str_value;
		//��ȡ����
		rapidxml::xml_attribute<> *p_attr = p_line_item->first_attribute("Name");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.strLegend = str_value;
		//��ȡ�ɼ�����
		p_attr = p_line_item->first_attribute("Visible");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.bVisible = (str_value.toInt() & 1);
		//��ȡ��������
		p_attr = p_line_item->first_attribute("Data");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.CurveData = (E_DATA_TYPE)(str_value.toInt());
		//��ȡ�������
		p_attr = p_line_item->first_attribute("Width");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.iLineWidth = str_value.toInt();
		//��ȡ�Ƿ������Чֵ
		p_attr = p_line_item->first_attribute("IgnoreVacant");
		if (NULL == p_attr)
		{
			return;
		}
		str_value = p_attr->value();
		st_line_info.bIgnoreVacant = (str_value.toInt() & 1);
		//��ȡ��������
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

//��ȡһ�����������ߵ�������Ϣ xiongz-[2020/8/28]
void CConfig::ReadChannelCurve(const rapidxml::xml_node<> *pChannelNode, CurveConfig *pCurveConfig) const
{
	assert(pChannelNode);
	assert(pCurveConfig);
	ReadChannelAccCurve(pChannelNode, &(pCurveConfig->vctAccCurves));
	ReadChannelBandCurve(pChannelNode, &(pCurveConfig->vctBandCurves));
	ReadChannelLineCurve(pChannelNode, &(pCurveConfig->vctLineCurves));

}
//��ȡ��������xiongz-[2020/8/28]
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
// ��ȡһ��ָ�������� =====================================
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

// ��ȡһ��ͼ�����ű��� ===================================
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

// ��ȡͼ��������� =======================================
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

// ��ȡһ��ɽ�����߻��� ===================================
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
		// ��ȡ��������
		rapidxml::xml_attribute<>* p_attr = p_item->first_attribute("Name");
		st_info.strLegend = (NULL == p_attr) ? "" : p_attr->value();
		// ��ȡ�����Ƿ�ɼ�
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
		// ��ȡ��������
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
		// ��ȡ�߿�
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
		// �Ƿ�������Ч����
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
		// �������/�Ҳ�Y��
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

// ��ȡһ����״�������� ===================================
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

// ��ȡһ������״�������� =================================
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

// ����ĳ������������ =====================================
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
	// д���ļ�
	std::string str_text;
	rapidxml::print(std::back_inserter(str_text), doc, 0);
	std::ofstream out(sz_xml.data());
	out << doc;
}

// ��ȡ���� ===============================================
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

// ����ָ������������======================================
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
	// д���ļ�
	std::string str_text;
	rapidxml::print(std::back_inserter(str_text), doc, 0);
	std::ofstream out(sz_xml.data());
	out << doc;
}

// �������ݶ�Ӧ��ɫ =======================================
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

	// д���ļ�
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
//��ȡԤ���������ظ���
int CConfig::GetReservedWidth() const
{
	int x_dpi = 0;
	QDesktopWidget *p_desk = QApplication::desktop();
	x_dpi = p_desk->physicalDpiX();
	return (x_dpi * m_fReserverWidth);
}

//===================ѡ�����ö�ȡ==================
void CConfig::ReadSegmentConfig()
{
	QString str_xml = file_func::GetExePath() + C_CONFIG_DIR + C_CONFIG_XML;
	QByteArray sz_xml = str_xml.toLocal8Bit();
	QString str_value;	//	��������ֵ
	SegmentView st_seg_view;
	//����xml�ļ�
	rapidxml::file<> f_doc(sz_xml.data());
	rapidxml::xml_document<> doc;
	doc.parse<rapidxml::parse_default>(f_doc.data());
	//�����ڵ�
	//���ڵ�sysconfig
	rapidxml::xml_node<> *p_root = doc.first_node(C_XML_ROOT_TAG);
	if (NULL == p_root)
	{
		return;
	}
	//titile�ڵ�
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
	//��ȡ������Ϣ
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
	//Channels�ڵ�
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
	//Layout�ڵ�
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
		//channel�ڵ�
		for (rapidxml::xml_node<> *p_node_channel = p_node_layout->first_node();
			NULL != p_node_channel;
			p_node_channel = p_node_channel->next_sibling())
		{
			SegmentChannelMeasure st_measure;
			SegmentChannelParam st_channel_param;
			E_CHANNEL_TYPE e_type;
			//��ȡ����ֵ
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
			//���ã�ÿһ�������µ�����˳��Ӳ����д���
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
	QString str_value;	//	��������ֵ
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