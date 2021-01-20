/********************************************************************
* Copyright (C)  �����е������������޹�˾
* All rights reserved
*
* �ļ����ƣ�PlotSegmentData.h
* ժ    Ҫ��ѡ�Σ��¼�������ݹ���
*		
* ��ʷ��¼��
*			Modify <2020/10/16>	xiongz,
* ********************************************************************/
#pragma once
#include <QObject>
#include <QMutex>
#include <QVector>
#include "Scale/PlotXMap.h"
#include "PublicDefs.h"
#include "CSegmentData.h"
class CPlotSegmentData
{
	
public:
	static CPlotSegmentData* GetInstance();
	static void ReleaseInstance();
	//�����Դ
	void Clear();
	//��ʼ��������ҳ����
	void Init(const qint64& iWidth);
	void Init();
	////����/��ȡҳ����
	//void SetPixWidth(const qint64&);
	qint64 GetPixWidth() const { return m_cAxisX.GetPixWidth(); }
	////����ÿһҳ�����ݳ���
	//void SetPageDataLen(const qint64&);
	qint64 GetPageDataLen() const { return m_cAxisX.GetDataWidth(); }
	////����/��ȡ��ǰҳ��ʼ����
	//void SetBegCol(const qint64 &iBegCol) { m_iBegCol = iBegCol; }
	qint64 GetBegCol() const { return m_cAxisX.GetBegCol(); }
	qint64 GetCurCol() const { return m_cAxisX.GetCurCol(); }
	qint64 GetCurIndex() const { return m_cAxisX.GetCurIndex(); }
	qint64 GetBegIndex() const { return m_cAxisX.GetCurIndex(); }
	//��ȡѹ����
	double GetPageRatio() const { double f_ratio = double(m_cAxisX.GetDataWidth()) / m_cAxisX.GetPixWidth(); return f_ratio; }
	//���ѡ�μ�¼
	void AppendSeg(const SegmentMarks& stSeg);
	void AppendSeg(const QVector<SegmentMarks>& vctSeg);

	//ɾ��ѡ�μ�¼
	void DeleteSeg(const qint64 &iIndex);

	//*****************************************************
	// Method��  	AddSegment	
	// Purpose��	��ѡ�οؼ��н���ѡ�Σ����ؼ��е�����ת��Ϊ����֡��
	// Access��    	public
	// Returns��   	void
	// Parameter��	
	//		iBegCol			-[input] ���λ�ã�����к�
	//		iEndCol			-[input] �յ�λ�ã�����к�
	//		color			-[input] ѡ�α���ɫ add <2020/11/4> xiongz
	//*****************************************************
	void AddSegment(const int &iBegCol,const int &iEndCol,const QColor &color = C_COLOR_SEG);
	//*****************************************************
	// Method��  	AppendEvent	
	// Purpose��	����¼����
	// Access��    	public
	// Returns��   	void
	// Parameter��	
	//		stEvent			-[input] �¼���ǽṹ��
	//*****************************************************
	void AppendEventMark(const StampedEvent &stEvent);
	void AppendEventMark(const QVector<StampedEvent> &vctEvent);
	//*****************************************************
	// Method��  	DeleteEvent	
	// Purpose��	���ݱ��֡�ź�ʱ��������������ɾ���¼�
	// Access��    	public
	// Returns��   	void
	// Parameter��	
	//		iIndex			-[input] �¼��ڷ�����֡��
	//		eType			-[input] �¼�����
	//*****************************************************
	void DeleteEvent(const qint64 &iIndex, const E_EVENT_TYPE &eType);
	//��ȡѡ�μ�¼
	QVector<SegmentMarks> GetSegMarks() const { return m_vctSeg; }
	//��ȡ�¼���Ǽ�¼
	QVector<StampedEvent> GetEventMarks() const { return m_vctEvent; }
	//����֡�ź�ҳ���кŻ���
	//*****************************************************
	// Method��  	TransFrame2Col	
	// Purpose��	������֡��ת��Ϊ��ǰҳ�����
	// Access��    	public
	// Returns��   	��ǰҳ�����
	// Parameter��	
	//		iFrame			-[input] ���ݵ�֡��
	//*****************************************************
	qint64 TransFrame2Col(const qint64& iFrame);
	qint64 TransCol2Frame(const qint64 &iCol);

	//����ʱ����
	void SetTimeMap(const CPlotXMap &cAxisX);
	//��ȡʱ����
	CPlotXMap GetTimeMap() const { return m_cAxisX; }
	//���õ�ǰѡ�εı���	 @xiongz [2020/11/5]
	void SetCurSegBackground(const QColor &color);


	//TODO:ѡ��������Ҫ���棬���Ա���Ϊxml�ļ����ڷ���

	void SaveSegmentMarks();
	void DeleteSegment(const E_SEGMENT_FLAG &eFlag);
	//*****************************************************
	// Method��		UpdateSegmentMarklist	
	// Purpose��		���ѡ�������в���Ҫ����ı�ǣ��������E_FLAG_NULL�ı�ǣ���
	//				����ӱ�Ǻͱ�����ʱ����
	// Access��    	private
	// Returns��   			
	// Parameter��	
	//
	//*****************************************************
	void UpdateSegemntMarkList();

private:
	CPlotSegmentData();
	~CPlotSegmentData();
	CPlotSegmentData& operator= (const CPlotSegmentData&) = delete;
	CPlotSegmentData(const CPlotSegmentData&) = delete;
private:
	static CPlotSegmentData* m_pInstance;
	static QMutex m_Mutex;
	static QMutex m_MutexSeg;						//�����������
	static QMutex m_MutexEvent;						//�����¼����
	QVector<SegmentMarks> m_vctSeg;					//�������
	QVector<StampedEvent> m_vctEvent;				//�¼����
	CPlotXMap m_cAxisX;
	SegmentMarks m_stCurSeg;
};

