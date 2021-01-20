/********************************************************************
* Copyright (C) �����е�����ҽ���豸�ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�PlotYMap.h
* ժ    Ҫ��
*		Y����
* ��ʷ��¼��
*		<2020/04/27> dingmh, Created
*********************************************************************/
#pragma once
class CPlotYMap
{
public:
	CPlotYMap();
	~CPlotYMap();

	//*****************************************************
	// Method:    	Init
	// Purpose:		��ʼ��Y��
	// Access:    	public
	// Returns:   	void
	// Parameter: 	
	//		fDownBound		-[input] ��������
	//		fUpBound		-[input] ��������
	//		iPixDownBound	-[input] ���������޶�Ӧ��Y������λ��
	//		iPixUpBound		-[input] ���������޶�Ӧ��Y������λ�ã�ͨ��Ϊ0��
	//*****************************************************
	void Init(const double fDownBound, const double fUpBound,
		const int iPixDownBound, const int iPixUpBound);

	//*****************************************************
	// Method:    	Trans
	// Purpose:		������ת��Ϊ����λ��
	// Access:    	public
	// Returns:   	����λ��
	// Parameter: 	fDataValue	-[input] ����ֵ
	//*****************************************************
	int Trans(const double fDataValue) const;

	//*****************************************************
	// Method:    	InvTrans
	// Purpose:		������λ��ת��Ϊ����
	// Access:    	public
	// Returns:   	��ֵ
	// Parameter: 	iPixValue	-[input] ����ֵ
	//*****************************************************
	double InvTrans(const int iPixValue) const;

	//*****************************************************
	// Method:    	SetDataRange
	// Purpose:		����Y������
	// Access:    	public
	// Returns:   	void
	// Parameter: 	
	//		fDownBound		-[input] ��������
	//		fUpBound		-[input] ��������
	//*****************************************************
	void SetDataRange(const double fDownBound, const double fUpBound);

	//*****************************************************
	// Method:    	ResizeScale
	// Purpose:		����Y�����سߴ�
	// Access:    	public
	// Returns:   	void
	// Parameter: 	
	//		iPixDownBound	-[input] ���������޶�Ӧ��Y������λ��
	//		iPixUpBound		-[input] ���������޶�Ӧ��Y������λ�ã�ͨ��Ϊ0��
	//*****************************************************
	void ResizeScale(const int iPixDownBound, const int iPixUpBound);

	//����Ƿ���Ч ========================================
	bool IsValid() const;

	// �ж��Ƿ��ڱ�߷�Χ�� ===============================
	bool Contains(const double fValue) const { return (fValue >= m_fDownBound) && (fValue <= m_fUpBound); }

	double GetValue2PixRatio() const { return m_fRatio; }
private:
	double m_fDownBound;
	double m_fUpBound;
	int	m_iPixDownBound;
	int m_iPixUpBound;
	double m_fRatio;
};

