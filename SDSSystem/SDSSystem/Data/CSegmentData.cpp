#include "CSegmentData.h"

QMutex CSegmentData::m_Mutex;
QMutex CSegmentData::m_MutexSegment;
CSegmentData *CSegmentData::m_pInstance = NULL;

CSegmentData::CSegmentData()
{
	m_mapSegmentMarks.clear();
}
CSegmentData* CSegmentData::Instance()
{
	if (NULL == m_pInstance)
	{
		QMutexLocker locker(&m_Mutex);
		if (NULL == m_pInstance)
		{
			m_pInstance = new CSegmentData();
		}
	}
	return m_pInstance;
}
void CSegmentData::ReleaseInstance()
{
	if (m_pInstance)
	{
		QMutexLocker locker(&m_Mutex);
		delete m_pInstance;
		m_pInstance = NULL;
	}
}
void CSegmentData::Clear()
{
	QMutexLocker locker(&m_MutexSegment);
	m_mapSegmentMarks.clear();
}
void CSegmentData::SetFileName(const QString &strFileName)
{
	m_strFileName = strFileName;
}
QString CSegmentData::GetFileName() const
{
	return m_strFileName;
}
void CSegmentData::SetDir(const QString &strDir)
{
	m_strDir = strDir;
}
int CSegmentData::CreateFile()
{

	return ERR_OK;

}
void CSegmentData::ReadSegmentMarks()
{
	QString str_file_name = m_strDir + m_strFileName;
	QByteArray sz_xml_name = str_file_name.toLocal8Bit();
	//解析xml文件
	rapidxml::file<> fdoc(sz_xml_name.data());
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<> *p_node_flag = NULL;
	rapidxml::xml_node<> *p_node_channel = NULL;
	rapidxml::xml_node<> *p_node_item = NULL;
	doc.parse<rapidxml::parse_default>(fdoc.data());
	rapidxml::xml_node<> *p_node_root = doc.first_node();
	if (NULL == p_node_root)
	{
		return;
	}
	for (p_node_flag = p_node_root->first_node();
		p_node_flag != NULL;
		p_node_flag = p_node_flag->next_sibling()
		)

	{
		SegmentMarks st_marks;
		E_SEGMENT_FLAG e_flag;
		QString str_flag_value;
		//attribute flag
		rapidxml::xml_attribute<> *p_attr_flag = p_node_flag->first_attribute(C_SEG_XML_FLAG_ATTR);
		if (NULL == p_attr_flag)
		{
			return;
		}
		str_flag_value = p_attr_flag->value();
		e_flag = (E_SEGMENT_FLAG)str_flag_value.toInt();
		//attribute startindex
		p_attr_flag = p_node_flag->first_attribute(C_SEG_XML_STRAT_INDEX_ATTR);
		if (NULL == p_attr_flag)
		{
			return;
		}
		str_flag_value = p_attr_flag->value();
		st_marks.iBegIndex = str_flag_value.toLongLong();
		//attribute endindex
		p_attr_flag = p_node_flag->first_attribute(C_SEG_XML_END_INDEX_ATTR);
		if (NULL == p_attr_flag)
		{
			return;
		}
		str_flag_value = p_node_flag->value();
		st_marks.iEndIndex = str_flag_value.toLongLong();
		//attribute pluses
		p_attr_flag = p_node_flag->first_attribute(C_SEG_XML_PLUSES_ATTR);
		if (NULL == p_attr_flag)
		{
			return;
		}
		str_flag_value = p_attr_flag->value();
		st_marks.stMeasureInfo.iPulses = str_flag_value.toInt();
		//attribute time
		p_attr_flag = p_node_flag->first_attribute(C_SEG_XML_TIME_ATTR);
		if (NULL == p_attr_flag)
		{
			return;
		}
		str_flag_value = p_node_flag->value();
		st_marks.stMeasureInfo.iTime = str_flag_value.toInt();
		//各导数据
		for (p_node_channel = p_node_flag->first_node();
			p_node_channel != NULL;
			p_node_channel = p_node_channel->next_sibling())
		{
			//attribute index
			SegmentChannelMeasure st_channel;
			E_CHANNEL_TYPE e_channel;
			QString str_channel_value;
			rapidxml::xml_attribute<> *p_attr_channel = p_node_channel->first_attribute(C_SEG_XML_INDEX_ATTR);
			if (NULL == p_attr_channel)
			{
				return;
			}
			str_channel_value = p_attr_channel->value();
			e_channel = (E_CHANNEL_TYPE)str_channel_value.toInt();
			st_channel.bIsVisible = true;
			//node item
			for (p_node_item = p_node_channel->first_node();
				p_node_item != NULL;
				p_node_item->next_sibling())
			{
				QString str_item_value;
				SegmentCurveParam st_item;
				//attribute type
				rapidxml::xml_attribute<> *p_attr_item = p_node_item->first_attribute(C_SEG_XML_TYPE_ATTR);
				if (NULL == p_attr_item)
				{
					return;
				}
				str_item_value = p_attr_item->value();
				st_item.eType = (E_DATA_TYPE)str_item_value.toInt();
				//attribute  index
				p_attr_item = p_node_item->first_attribute(C_SEG_XML_INDEX_ATTR);
				if (NULL == p_attr_item)
				{
					return;
				}
				str_item_value = p_attr_item->value();
				st_item.iIndex = str_item_value.toInt();
				//attribute mean
				p_attr_item = p_node_item->first_attribute(C_SEG_XML_MEAN_ATTR);
				if (NULL == p_attr_item)
				{
					return;
				}
				str_item_value = p_attr_item->value();
				st_item.stMeasureInfo.strMean = str_item_value;
				//attribure max
				p_attr_item = p_node_item->first_attribute(C_SEG_XML_MAX_ATTR);
				if (NULL == p_attr_item)
				{
					return;
				}
				str_item_value = p_attr_item->value();
				st_item.stMeasureInfo.strMax = str_item_value;
				//attribute min
				p_attr_item = p_node_item->first_attribute(C_SEG_XML_MIN_ATTRR);
				if (NULL == p_attr_item)
				{
					return;
				}
				str_item_value = p_attr_item->value();
				st_item.stMeasureInfo.strMin = str_item_value;
				//attribute max_div_min
				p_attr_item = p_node_item->first_attribute(C_SEG_XML_MAX_DIV_MIN_ATTR);
				if (NULL == p_attr_item)
				{
					return;
				}
				str_item_value = p_attr_item->value();
				st_item.stMeasureInfo.strMax_Min = QString("%1%").arg(100 * str_item_value.toFloat());
			}
			
		}
	}


}

int CSegmentData::SaveSegmentMarks(const QVector<SegmentMarks> &vctSegmentMarks)
{
	//参数判断
	Q_ASSERT(!vctSegmentMarks.empty());
	//数据存储
	QString str_file_name = m_strDir + m_strFileName;
	QByteArray sz_file_name = str_file_name.toLocal8Bit();
	rapidxml::xml_document<> *p_doc = new rapidxml::xml_document<>();
	WriteXmlHead(C_SEG_XML_HEAD,&p_doc);
	rapidxml::xml_node<> *p_node_seg_info = NULL;
	WriteXmlRootNode(C_SEG_XML_SEG_INFO_NODE, &p_doc, &p_node_seg_info);
	//添加结点
	 
	for (QVector<SegmentMarks>::const_iterator iter = vctSegmentMarks.begin();
		iter != vctSegmentMarks.end();
		iter++)
	{
		rapidxml::xml_node<> *p_node_flag = NULL;
		WriteMeasureInfo(*iter,&p_doc,&p_node_seg_info,&p_node_flag);
		//add flag
		//WriteSegmentFlag(iter.key(), &p_doc, &p_node_flag);
	}
	std::ofstream pout(sz_file_name.data());
	pout << *p_doc;
	delete p_doc;	
	return 0;

}
void CSegmentData::WriteXmlHead(const char *szHeadNode, rapidxml::xml_document<> **ppDoc)
{
	Q_ASSERT(*ppDoc);
	Q_ASSERT(szHeadNode);
	rapidxml::xml_document<> * &p_doc = *ppDoc;
	rapidxml::xml_node<> *head = (p_doc)->allocate_node(rapidxml::node_pi,(p_doc)->allocate_string(szHeadNode));
	(p_doc)->append_node(head);
}
void CSegmentData::WriteXmlRootNode(
	const char *szRootNode,
	rapidxml::xml_document<> **ppDoc,
	rapidxml::xml_node <> **ppNodeRoot)
{
	Q_ASSERT(szRootNode);
	Q_ASSERT(*ppDoc);
	rapidxml::xml_document<> * &p_doc = *ppDoc;
	rapidxml::xml_node<> * &p_node_root = *ppNodeRoot;
	p_node_root = (p_doc)->allocate_node(rapidxml::node_element, (p_doc)->allocate_string(szRootNode));
	(p_doc)->append_node(p_node_root);

}
void CSegmentData::WriteCurveInfo(
	const SegmentCurveParam &stCurveInfo,
	rapidxml::xml_document<> **ppDoc,
	rapidxml::xml_node<> **ppNodeParent,
	rapidxml::xml_node<> **ppNodeItem)
{
	Q_ASSERT(*ppDoc);
	Q_ASSERT(*ppNodeParent);
	rapidxml::xml_document<> * &p_doc = *ppDoc;
	rapidxml::xml_node<> * &p_node_parent = *ppNodeParent;
	rapidxml::xml_node<> * &p_node_item = *ppNodeItem;
	p_node_item = p_doc->allocate_node(rapidxml::node_element,p_doc->allocate_string(C_SEG_XML_ITEM_NODE));
	//add attribute
	rapidxml::xml_attribute<> *p_attr_item = NULL;
	QByteArray sz_buff;
	//type attribute
	sz_buff = QString::number((int)stCurveInfo.eType).toLocal8Bit();
	p_attr_item = p_doc->allocate_attribute(C_SEG_XML_TYPE_ATTR, p_doc->allocate_string(sz_buff.data()));
	p_node_item->append_attribute(p_attr_item);
	//index attribute
	sz_buff = QString::number(stCurveInfo.iIndex).toLocal8Bit();
	p_attr_item = p_doc->allocate_attribute(C_SEG_XML_INDEX_ATTR,p_doc->allocate_string(sz_buff.data()));
	p_node_item->append_attribute(p_attr_item);
	//mean attribute
	sz_buff = stCurveInfo.stMeasureInfo.strMean.toLocal8Bit();
	p_attr_item = p_doc->allocate_attribute(C_SEG_XML_MEAN_ATTR,p_doc->allocate_string(sz_buff.data()));
	p_node_item->append_attribute(p_attr_item);
	//max attribute
	sz_buff = stCurveInfo.stMeasureInfo.strMax.toLocal8Bit();
	p_attr_item = p_doc->allocate_attribute(C_SEG_XML_MAX_ATTR,p_doc->allocate_string(sz_buff.data()));
	p_node_item->append_attribute(p_attr_item);
	//min attribute
	sz_buff = stCurveInfo.stMeasureInfo.strMin.toLocal8Bit();
	p_attr_item = p_doc->allocate_attribute(C_SEG_XML_MIN_ATTRR,p_doc->allocate_string(sz_buff.data()));
	p_node_item->append_attribute(p_attr_item);
	//max_div_min attribute
	sz_buff = stCurveInfo.stMeasureInfo.strMax_Min.toLocal8Bit();
	p_attr_item = p_doc->allocate_attribute(C_SEG_XML_MAX_DIV_MIN_ATTR,p_doc->allocate_string(sz_buff.data()));
	p_node_item->append_attribute(p_attr_item);
	p_node_parent->append_node(p_node_item);
}
//DEPRECATED
//Reason：选段结构体修改了，数据类型放到结构体
//
//Author：xiongz [2020/12/1]
//void CSegmentData::WriteChannelType(
//	const E_CHANNEL_TYPE &eType,
//	rapidxml::xml_document<> **ppDoc,
//	rapidxml::xml_node<> **ppNodeChannel)
//{
//	Q_ASSERT(*ppDoc);
//	Q_ASSERT(*ppNodeChannel);
//	rapidxml::xml_document<> * &p_doc = *ppDoc;
//	rapidxml::xml_node<> * &p_node_channel = *ppNodeChannel;
//	QByteArray sz_buff = QString::number((int)eType).toLocal8Bit();
//	rapidxml::xml_attribute<> *p_attr_channel = p_doc->allocate_attribute(C_SEG_XML_INDEX_ATTR,
//		p_doc->allocate_string(sz_buff.data()));
//	p_node_channel->prepend_attribute(p_attr_channel);
//
//}
void CSegmentData::WriteChannelMeasureInfo(
	const SegmentChannelMeasure &stChannelMeasure,
	rapidxml::xml_document<> **ppDoc,
	rapidxml::xml_node<> **ppNodeParent,
	rapidxml::xml_node<> **ppNodeChannel)
{
	Q_ASSERT(*ppDoc);
	Q_ASSERT(*ppNodeParent);
	rapidxml::xml_document<> * &p_doc = *ppDoc;
	rapidxml::xml_node<> * &p_node_parent = *ppNodeParent;
	rapidxml::xml_node<> * &p_node_channel = *ppNodeChannel;
	QByteArray sz_buff;
	rapidxml::xml_node<> *p_node_item = NULL;
	p_node_channel = p_doc->allocate_node(rapidxml::node_element,p_doc->allocate_string(C_SEG_XML_CHANNEL_NODE));
	
	//index
	sz_buff = QString::number((int)stChannelMeasure.eType).toLocal8Bit();
	rapidxml::xml_attribute<> *p_attr_channel = p_doc->allocate_attribute(C_SEG_XML_INDEX_ATTR,
		p_doc->allocate_string(sz_buff.data()));
	p_node_channel->append_attribute(p_attr_channel);
	QString strVisible;
	//visible
	if (stChannelMeasure.bIsVisible)
	{
		strVisible = QString("1");
	}
	else
	{
		strVisible = QString("0");
	}
	sz_buff = strVisible.toLocal8Bit();
	p_attr_channel = p_doc->allocate_attribute(C_SEG_XML_VISIBLE_ATTR,
		p_doc->allocate_string(sz_buff.data()));
	p_node_channel->append_attribute(p_attr_channel);
	//添加item节点
	for (auto it : stChannelMeasure.vctCurveParam)
	{
		WriteCurveInfo(it, ppDoc, ppNodeChannel, &p_node_item);
	}
	p_node_parent->append_node(p_node_channel);
	
}
void CSegmentData::WriteMeasureInfo(
	const SegmentMarks &stMarks,
	rapidxml::xml_document<> **ppDoc,
	rapidxml::xml_node<> **ppNodeParent,
	rapidxml::xml_node<> **ppNodeFlag)
{
	//参数判断
	Q_ASSERT(*ppDoc);
	Q_ASSERT(*ppNodeParent);

	rapidxml::xml_document<> * &p_doc = *ppDoc;
	rapidxml::xml_node<> * &p_node_parent = *ppNodeParent;
	rapidxml::xml_node<> * &p_node_flag = *ppNodeFlag;

	QByteArray sz_buff;
	rapidxml::xml_node<> *p_node_channel = NULL;
	rapidxml::xml_attribute<> *p_attr_flag = NULL;
	p_node_flag = p_doc->allocate_node(rapidxml::node_element,p_doc->allocate_string(C_SEG_XML_SEG_FLAG_NODE));
	//Flag attribute
	sz_buff = QString::number((int)stMarks.eFlag).toLocal8Bit();
	p_attr_flag = p_doc->allocate_attribute(C_SEG_XML_FLAG_ATTR,p_doc->allocate_string(sz_buff.data()));
	p_node_flag->append_attribute(p_attr_flag);
	//StartIndex attribute
	sz_buff = QString::number(stMarks.iBegIndex).toLocal8Bit();
	p_attr_flag = p_doc->allocate_attribute(C_SEG_XML_STRAT_INDEX_ATTR,p_doc->allocate_string(sz_buff.data()));
	p_node_flag->append_attribute(p_attr_flag);
	//EndIndex attribute
	sz_buff = QString::number(stMarks.iEndIndex).toLocal8Bit();
	p_attr_flag = p_doc->allocate_attribute(C_SEG_XML_END_INDEX_ATTR,p_doc->allocate_string(sz_buff.data()));
	p_node_flag->append_attribute(p_attr_flag);
	//Pluses attribute
	sz_buff = QString::number(stMarks.stMeasureInfo.iPulses).toLocal8Bit();
	p_attr_flag = p_doc->allocate_attribute(C_SEG_XML_PLUSES_ATTR,p_doc->allocate_string(sz_buff.data()));
	p_node_flag->append_attribute(p_attr_flag);
	//time attribute
	sz_buff = QString::number(stMarks.stMeasureInfo.iTime).toLocal8Bit();
	p_attr_flag = p_doc->allocate_attribute(C_SEG_XML_TIME_ATTR,p_doc->allocate_string(sz_buff.data()));
	p_node_flag->append_attribute(p_attr_flag);
	//add nodes
	for (auto iter = stMarks.stMeasureInfo.mapChannelMeasure.begin();
		 iter != stMarks.stMeasureInfo.mapChannelMeasure.end();
		 iter ++ )
	{
		QVector<SegmentChannelMeasure> vct_channel = iter.value();
		for (auto it : vct_channel)
		{
			rapidxml::xml_node<> *p_node_channel = NULL;
			WriteChannelMeasureInfo(it, ppDoc, ppNodeFlag, &p_node_channel);
		}
	}
	p_node_parent->append_node(p_node_flag);
}
void CSegmentData::WriteSegmentFlag(
	const E_SEGMENT_FLAG &eFlag,
	rapidxml::xml_document<> **ppDoc,
	rapidxml::xml_node<> **ppNodeFlag)
{
	Q_ASSERT(*ppDoc);
	Q_ASSERT(*ppNodeFlag);
	rapidxml::xml_document<> * &p_doc = *ppDoc;
	rapidxml::xml_node<> * &p_node_flag = *ppNodeFlag;
	QByteArray sz_buff = QString::number((int)eFlag).toLocal8Bit();
	rapidxml::xml_attribute<> *p_attr_flag = p_doc->allocate_attribute(C_SEG_XML_FLAG_ATTR,
		p_doc->allocate_string(sz_buff.data()));
	p_node_flag->prepend_attribute(p_attr_flag);
}