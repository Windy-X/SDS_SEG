/********************************************************************
* Copyright (C)  �����е������������޹�˾
* All rights reserved
*
* �ļ����ƣ�CSegmentData.h
* ժ    Ҫ������Ͷ�ȡѡ�����ݣ���Ƴɵ���ģʽ(û�б�����ɫ����)
*		
* ��ʷ��¼��
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
	//����/��ȡ�ļ���
	void SetFileName(const QString &);
	QString GetFileName() const;
	//����/��ȡ�ļ�·��
	void SetDir(const QString &);
	QString GetDir() const { return m_strDir; }

	//*****************************************************
	// Method��  	OpenFile	
	// Purpose��	��xml�ļ�
	// Access��    	public
	// Returns��   	������
	// Parameter��	
	//
	//*****************************************************
	int OpenFile();
	//��ȡxml�ļ�����
	void ReadSegmentMarks();
	//*****************************************************
	// Method��  	SaveSegmentMarks	
	// Purpose��	����ѡ�α�ǵ�xml�ļ�
	// Access��    	public
	// Returns��   	������		
	// Parameter��	
	//		vctSegmentMarks		-[input] ѡ�α��
	//*****************************************************
	int SaveSegmentMarks(const QVector<SegmentMarks> &vctSegmentMarks);
	//*****************************************************
	// Method��  	GetSegmentMark	
	// Purpose��	�������֡�ź��յ�֡�Ŵ�ѡ����ɸѡ��һ�����������ĵ�ѡ��
	// Access��    	public
	// Returns��   	ѡ�α�ǽṹ��
	// Parameter��	
	//	iStartindex			-[input] ѡ�����֡��
	//	iEndIndex			-[input] ѡ���յ�֡��
	//*****************************************************
	SegmentMarks GetSegmentMark(const int &iStartIndex, const int &iEndIndex) const;
	void Clear();

private:
	CSegmentData();
	CSegmentData(const CSegmentData &) = delete;
	CSegmentData & operator =(const CSegmentData &) = delete;
	//*****************************************************
	// Method��  	CreatFile	
	// Purpose��	�������ڱ���ѡ�α�ǵ�xml�ļ�
	// Access��    	private
	// Returns��   			
	// Parameter��	������
	//
	//*****************************************************
	int CreateFile();
	//*****************************************************
	// Method��  	WriteXmlHead	
	// Purpose��	��xml�ļ���д���ļ�ͷ�����Ϣ
	// Access��    	private
	// Returns��   	void
	// Parameter��	
	//		szHeadNode		-[input]			ͷ�����Ϣ
	//		ppDoc			-[input]/[output]	ָ��xml�ļ�������Ϣָ���ָ�룬Ҫ���ֵָ�����ݷǿ�
	//*****************************************************
	void WriteXmlHead(const char *szHeadNode,  rapidxml::xml_document<> **ppDoc);
	//*****************************************************
	// Method��  	WriteXmlRootNode	
	// Purpose��	��xml�ļ���д����ڵ�
	// Access��    	private
	// Returns��   	void
	// Parameter��	
	//		szRootNode			-[input]			���ڵ���Ϣ
	//		ppDoc				-[input]/[output]	ָ��xml�ļ�������Ϣָ���ָ�룬Ҫ���ֵָ�����ݷǿ�
	//		ppNodeRoot			-[output]			���ڵ�ָ��
	//*****************************************************
	void WriteXmlRootNode(const char *szRootNode,
		rapidxml::xml_document<> **ppDoc,
		rapidxml::xml_node<> **ppNodeRoot);
	//*****************************************************
	// Method��  	WriteCurveInfo	
	// Purpose��	��ÿ�����ߵ�ͳ����Ϣ��Ϊ������Ա���
	// Access��    	private
	// Returns��   	void		
	// Parameter��	
	//		stCurveInfo			-[input]			ÿ�����ߵ�ͳ����Ϣ
	//		ppDoc				-[input]/[output]	ָ��xml�ļ�������Ϣָ���ָ�룬Ҫ���ֵָ�����ݷǿ�
	//		ppNodeParent		-[intput]/[output]	item�ĸ��ڵ��ָ�루����ָ�룩
	//		ppNodeItem			-[output]			���item��㣨����ָ�룩
	//*****************************************************
	void WriteCurveInfo(
		const SegmentCurveParam &stCurveInfo,
		rapidxml::xml_document<> **ppDoc,
		rapidxml::xml_node<> **ppNodeParent,
		rapidxml::xml_node<> **ppNodeItem);
	//*****************************************************
	// Method��  	WriteChannelMeasureInfo	
	// Purpose��	��һ����ͳ����Ϣд�뵽SegmentFlag�ڵ���
	// Access��    	private
	// Returns��   	void		
	// Parameter��	
	//		stChannelMeasure	-[input]			һ����ͳ����Ϣ
	//		ppDoc				-[input]/[output]	ָ��xml�ļ�������Ϣָ���ָ�룬Ҫ���ֵָ�����ݷǿ�
	//		ppNodeParent		-[intput]/[output]	channel�ĸ��ڵ��ָ�루����ָ�룩
	//		ppNodeChannel		-[output]			���һ��ͳ����Ϣ�Ľڵ㣨����ָ�룩
	//*****************************************************
	void WriteChannelMeasureInfo(
		const SegmentChannelMeasure &stChannelMeasure,
		rapidxml::xml_document<> **ppDoc,
		rapidxml::xml_node<> **ppNodeParent,
		rapidxml::xml_node<> **ppNodeChannel);
	//*****************************************************
	// Method��  	WriteChannelType	
	// Purpose��	��xml�ļ���д��ÿһ����Ӧ������
	// Access��    	private
	// Returns��   	void
	// Parameter��	
	//		eType				-[input]			����������
	//		ppDoc				-[input]/[output]	ָ��xml�ļ�������Ϣָ���ָ�룬Ҫ���ֵָ�����ݷǿ�
	//		ppNodeChaneel		-[input]/[output]	������Ӧ�Ľڵ�ָ�루����ָ�룩
	//*****************************************************
	void WriteChannelType(
		const E_CHANNEL_TYPE &eType,
		rapidxml::xml_document<> **ppDoc,
		rapidxml::xml_node<> **ppNodeChannel);
	//*****************************************************
	// Method��  	WriteMeasureInfo	
	// Purpose��	д��һ��ѡ����Ϣ��xml�ļ�
	// Access��    	private
	// Returns��   	void
	// Parameter��	
	//		stMarks				-[input]			һ��ѡ��ͳ����Ϣ
	//		ppDoc				-[input]/[output]	ָ��xml�ļ�������Ϣָ���ָ�룬Ҫ���ֵָ�����ݷǿ�
	//		ppNodeParent		-[intput]/[output]	Flag�ڵ�ĸ��ڵ�(����ָ��)
	//		ppNodeFlag			-[output]			���һ��ѡ�εĽڵ㣨����ָ�룩
	//*****************************************************
	void WriteMeasureInfo(
		const SegmentMarks &stMarks,
		rapidxml::xml_document<> **ppDoc,
		rapidxml::xml_node<> **ppNodeParent,
		rapidxml::xml_node<> **ppNodeFlag);
	//*****************************************************
	// Method��  	WriteSegmentFlag	
	// Purpose��	��xml�ļ���д��ѡ�α��
	// Access��    	private
	// Returns��   	void
	// Parameter��	
	//		eFlag				-[input]			ѡ�α��
	//		ppDoc				-[input]/[output]	ָ��xml�ļ�������Ϣָ���ָ�룬Ҫ���ֵָ�����ݷǿ�
	//		ppNodeFlag			-[input]/[output]	ѡ�α�ǽڵ㣨����ָ�룩
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
	QString m_strDir;			//�ļ�·��
	QMap<E_SEGMENT_FLAG, SegmentMarks> m_mapSegmentMarks;
	//QMap<SegmentMarks> m_vctSegmentMarks;
};