/********************************************************************
* Copyright (C)  �����е������������޹�˾
* All rights reserved
*
* �ļ����ƣ�CPlotData.h
* ժ    Ҫ��
*		���ݴ����
* ��ʷ��¼��
*			Copy<2020/8/13>	xiongz,
* ********************************************************************/
#pragma once
#include <QVector>
#include <QMap>
#include <QPair>
#include "PublicDefs.h"
#include "Scale\PlotXMap.h"
class CPlotData
{
public:
	CPlotData();
	~CPlotData();
	//*****************************************************
	// Method��  	Init	
	// Purpose��	��ʼ��
	// Access��    	public
	// Returns��   	void
	// Parameter��	
	//		iPixWidth		-[input] ͼ����
	//*****************************************************
	void Init(const int iPixWidth);
	//*****************************************************
	// Method��  	AppendData	
	// Purpose��	�����ʾ���ݣ�����ǰ�����õ�ǰ���ʺ���һ������
	// Access��    	public
	// Returns��   	void
	// Parameter��	
	//		fData		-[input] ���ӵ�����
	//*****************************************************
	void AppendData(const double fData);
	//*****************************************************
	// Method��  	ApppendData	
	// Purpose��	�����ʾ������,����ǰ�����õ�ǰ���ʺ���һ�����ʣ�
	//				�Ƿ񻻵�ȡ��������������vct�����Ƿ�ﵽ���ޣ�����ԭʼ���ݳ��ȴﵽʱ��������
	// Access��    	public
	// Returns��   	void
	// Parameter��	
	//		vctData		-[input] ���ӵ���������
	//		cAxisX		-[input] ���ʱ����
	//		bChange		-[input] �Ƿ񻻵��ı�־��
	//*****************************************************
	void AppendData(const QVector<double> vctData, const CPlotXMap &cAxisX, const bool &bChange);

	//�������
	void Clear();
	//����/��ȡ����
	void SetCurRatio(const double &fRatio) { m_fCurRatio = fRatio; }
	double GetCurRatio() const { return m_fCurRatio; }
	//���õ�ǰ�����ţ�
	void SetIndex(const qint64 iIndex) { m_iCurIndex = iIndex; }
	void AddIndex(const qint64 iOffset) { m_iCurIndex += iOffset; }
	qint64 GetCurrentIndex() const { return m_iCurIndex; }

	//����/��ȡ��ǰʱ���ȣ�ֻ��ʵʱ��������ʱ��Ч
	void SetCurTimeWidth(const int &iTimeWidth) { m_iTimeWidth = iTimeWidth; }
	qint64 GetCurTimeWidth() const { return m_iTimeWidth; }
	//*****************************************************
	// Method��  	GetDisplayUpEvn	
	// Purpose��	��ȡ���ݵ��ϰ�����ʾ
	// Access��    	public
	// Returns��   	�ϰ�������
	// Parameter��	
	//		xMap		-[input] ��ǰҳ���ʱ����
	//*****************************************************
	QVector<double>GetDisplayUpEvn(const CPlotXMap &xMap) const;

	//*****************************************************
	// Method��  	GetDisplayDownEvn	
	// Purpose��	��ȡ���ݵ��°�����ʾ
	// Access��    	public
	// Returns��   	�°�������
	// Parameter��	
	//		xMap		-[input] ��ǰҳ���ʱ����
	//*****************************************************
	QVector<double>GetDisplayDowmEvn(const CPlotXMap &xMap) const;
	//��ȡԭʼ����
	const int &GetRawDataLen() const {}

	int GetPixWidth() const { return m_iPixWidth; }

	//*****************************************************
	// Method��  	ModifyShowData	
	// Purpose��	ʱ����仯ʱ���޸ĵ�ǰ��ʾֵ����m_vctCurEvn
	// Access��    	public
	// Returns��   	void		
	// Parameter��	
	//		pairRatio		--[input] first���޸�ǰ�ı��ʣ�second�޸ĺ�ı���
	//*****************************************************
	void ModifyShowData(const QPair<double,double> &pairRatio);

	//��ȡ������ʵ����ӣ���ǰ����*����
	double GetPreRatioScale() const { return m_fPreScale; }
	double GetNextRatioScale() const { return m_fNextScale; }
	//��ȡ/����ǰ������
	void SetStep(const qint64 &iStep) { m_iStep = iStep; }
	qint64 GetStep() const { return m_iStep; }
	//ƽ������ add <2020/10/9> xiongz
	void MoveCurve();


	
private:
	QVector<double> m_vctRawData;					//�ӱ����ļ���ȡԭʼ����
	QVector<QPair<double, double>> m_vctCurData;	//��ʾ����
	qint64 m_iCurIndex;								//��ǰ������ͬ��Ҳ�����ݳ���
	int m_iPixWidth;								//�ռ�����ؿ��
	int m_iReseverdWidth;							//Ԥ�����ڵ����ؿ��
	int m_iTimeWidth;								//��ǰʱ��������ݿ��
	double m_fPreScale;								//������һ�����ʵ�����
	double m_fNextScale;							//������һ�����ʵ�����
	double m_fCurRatio;								//��ǰ����

	qint64	m_iStep;								//������һ����ǰ������������Ԥ�����ؿ�ȣ�


};

