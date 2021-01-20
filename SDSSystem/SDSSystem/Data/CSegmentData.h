/********************************************************************
* Copyright (C)  深圳市德力凯电子有限公司
* All rights reserved
*
* 文件名称：CSegmentData.h
* 摘    要：保存和读取选段数据，设计成单例模式(没有保存颜色配置)
*		
* 历史记录：
*			<2020/11/24>	xiongz,
* ********************************************************************/
#pragma once
#include <QMutex>
#include "PublicDefs.h"
#include "FileFunction.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

class CSegmentData
{
public:
	static CSegmentData * Instance();
	static void ReleaseInstance();
	//设置/获取文件名
	void SetFileName(const QString &);
	QString GetFileName() const;
	//设置/获取文件路径
	void SetDir(const QString &);
	QString GetDir() const { return m_strDir; }

	//*****************************************************
	// Method：  	OpenFile	
	// Purpose：	打开xml文件
	// Access：    	public
	// Returns：   	错误码
	// Parameter：	
	//
	//*****************************************************
	int OpenFile();
	//读取xml文件内容
	void ReadSegmentMarks();
	//*****************************************************
	// Method：  	SaveSegmentMarks	
	// Purpose：	保存选段标记导xml文件
	// Access：    	public
	// Returns：   	错误码		
	// Parameter：	
	//		vctSegmentMarks		-[input] 选段标记
	//*****************************************************
	int SaveSegmentMarks(const QVector<SegmentMarks> &vctSegmentMarks);
	//*****************************************************
	// Method：  	GetSegmentMark	
	// Purpose：	根据起点帧号和终点帧号从选段中筛选第一个符合条件的的选段
	// Access：    	public
	// Returns：   	选段标记结构体
	// Parameter：	
	//	iStartindex			-[input] 选段起点帧号
	//	iEndIndex			-[input] 选段终点帧号
	//*****************************************************
	SegmentMarks GetSegmentMark(const int &iStartIndex, const int &iEndIndex) const;
	void Clear();

private:
	CSegmentData();
	CSegmentData(const CSegmentData &) = delete;
	CSegmentData & operator =(const CSegmentData &) = delete;
	//*****************************************************
	// Method：  	CreatFile	
	// Purpose：	创建用于保存选段标记的xml文件
	// Access：    	private
	// Returns：   			
	// Parameter：	错误码
	//
	//*****************************************************
	int CreateFile();
	//*****************************************************
	// Method：  	WriteXmlHead	
	// Purpose：	在xml文件中写入文件头结点信息
	// Access：    	private
	// Returns：   	void
	// Parameter：	
	//		szHeadNode		-[input]			头结点信息
	//		ppDoc			-[input]/[output]	指向xml文件内容信息指针的指针，要求初值指向内容非空
	//*****************************************************
	void WriteXmlHead(const char *szHeadNode,  rapidxml::xml_document<> **ppDoc);
	//*****************************************************
	// Method：  	WriteXmlRootNode	
	// Purpose：	在xml文件中写入根节点
	// Access：    	private
	// Returns：   	void
	// Parameter：	
	//		szRootNode			-[input]			根节点信息
	//		ppDoc				-[input]/[output]	指向xml文件内容信息指针的指针，要求初值指向内容非空
	//		ppNodeRoot			-[output]			根节点指针
	//*****************************************************
	void WriteXmlRootNode(const char *szRootNode,
		rapidxml::xml_document<> **ppDoc,
		rapidxml::xml_node<> **ppNodeRoot);
	//*****************************************************
	// Method：  	WriteCurveInfo	
	// Purpose：	将每条曲线的统计信息作为结点属性保存
	// Access：    	private
	// Returns：   	void		
	// Parameter：	
	//		stCurveInfo			-[input]			每条曲线的统计信息
	//		ppDoc				-[input]/[output]	指向xml文件内容信息指针的指针，要求初值指向内容非空
	//		ppNodeParent		-[intput]/[output]	item的父节点的指针（二级指针）
	//		ppNodeItem			-[output]			输出item结点（二级指针）
	//*****************************************************
	void WriteCurveInfo(
		const SegmentCurveParam &stCurveInfo,
		rapidxml::xml_document<> **ppDoc,
		rapidxml::xml_node<> **ppNodeParent,
		rapidxml::xml_node<> **ppNodeItem);
	//*****************************************************
	// Method：  	WriteChannelMeasureInfo	
	// Purpose：	将一导的统计信息写入到SegmentFlag节点中
	// Access：    	private
	// Returns：   	void		
	// Parameter：	
	//		stChannelMeasure	-[input]			一导的统计信息
	//		ppDoc				-[input]/[output]	指向xml文件内容信息指针的指针，要求初值指向内容非空
	//		ppNodeParent		-[intput]/[output]	channel的父节点的指针（二级指针）
	//		ppNodeChannel		-[output]			输出一导统计信息的节点（二级指针）
	//*****************************************************
	void WriteChannelMeasureInfo(
		const SegmentChannelMeasure &stChannelMeasure,
		rapidxml::xml_document<> **ppDoc,
		rapidxml::xml_node<> **ppNodeParent,
		rapidxml::xml_node<> **ppNodeChannel);
	//*****************************************************
	// Method：  	WriteChannelType	
	// Purpose：	在xml文件中写入每一导对应的类型
	// Access：    	private
	// Returns：   	void
	// Parameter：	
	//		eType				-[input]			导联的类型
	//		ppDoc				-[input]/[output]	指向xml文件内容信息指针的指针，要求初值指向内容非空
	//		ppNodeChaneel		-[input]/[output]	导联对应的节点指针（二级指针）
	//*****************************************************
	void WriteChannelType(
		const E_CHANNEL_TYPE &eType,
		rapidxml::xml_document<> **ppDoc,
		rapidxml::xml_node<> **ppNodeChannel);
	//*****************************************************
	// Method：  	WriteMeasureInfo	
	// Purpose：	写入一个选段信息到xml文件
	// Access：    	private
	// Returns：   	void
	// Parameter：	
	//		stMarks				-[input]			一个选段统计信息
	//		ppDoc				-[input]/[output]	指向xml文件内容信息指针的指针，要求初值指向内容非空
	//		ppNodeParent		-[intput]/[output]	Flag节点的父节点(二级指针)
	//		ppNodeFlag			-[output]			输出一个选段的节点（二级指针）
	//*****************************************************
	void WriteMeasureInfo(
		const SegmentMarks &stMarks,
		rapidxml::xml_document<> **ppDoc,
		rapidxml::xml_node<> **ppNodeParent,
		rapidxml::xml_node<> **ppNodeFlag);
	//*****************************************************
	// Method：  	WriteSegmentFlag	
	// Purpose：	在xml文件中写入选段标记
	// Access：    	private
	// Returns：   	void
	// Parameter：	
	//		eFlag				-[input]			选段标记
	//		ppDoc				-[input]/[output]	指向xml文件内容信息指针的指针，要求初值指向内容非空
	//		ppNodeFlag			-[input]/[output]	选段标记节点（二级指针）
	//*****************************************************
	void WriteSegmentFlag(
		const E_SEGMENT_FLAG &eFlag,
		rapidxml::xml_document<> **ppDoc,
		rapidxml::xml_node<> **ppNodeFlag
	);

private:
	static QMutex m_Mutex;
	static QMutex m_MutexSegment;
	static CSegmentData *m_pInstance;
	QString m_strFileName;
	QString m_strDir;			//文件路径
	QMap<E_SEGMENT_FLAG, SegmentMarks> m_mapSegmentMarks;
	//QMap<SegmentMarks> m_vctSegmentMarks;
};