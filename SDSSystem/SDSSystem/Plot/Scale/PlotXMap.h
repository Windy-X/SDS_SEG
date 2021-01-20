/********************************************************************
* Copyright (C) �����е�����ҽ���豸�ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�PlotXScale.h
* ժ    Ҫ��
*		ʱ���ᣨX�ᣩ����࣬��֡�������ص��໥ת��
* ��ʷ��¼��
*		<2020/04/26> dingmh, Created
*********************************************************************/
#pragma once
#include <assert.h>

class CPlotXMap
{
public:
	CPlotXMap();
	~CPlotXMap();
	//*****************************************************
	// Method:    	Init
	// Purpose:		��ʼ��X��
	// Access:    	pubulic
	// Returns:   	void
	// Parameter: 	
	//		iPixWidth		-[input] ���ؿ��
	//		iDataWidth		-[input] ���ݿ�ȣ�������֡���ݣ�
	//		iStartIndex		-[input] ҳ����ʼ֡��
	//*****************************************************
	void Init(const int iPixWidth, const int iDataWidth, const int iStartIndex);

	//*****************************************************
	// Method:    	Trans
	// Purpose:		��֡�Ÿ�������ת��Ϊ����λ��
	// Access:    	public
	// Returns:   	����λ��
	// Parameter: 	
	//		iDataIndex		-[input] ֡��
	//***************************************************
	int Trans(const int iDataIndex) const;

	//*****************************************************
	// Method:    	InvTrans
	// Purpose:		������λ��ת��Ϊ֡��
	// Access:    	public
	// Returns:   	֡��
	// Parameter: 	iPixIndex		-[input] ����λ��
	//*****************************************************
	int InvTrans(const int iPixIndex) const;

	// ��ȡѹ������
	double GetRatio() const { return double(m_iDataWidth) / m_iPixWidth; }

	int GetPixWidth() const { return m_iPixWidth; }
	int GetBegIndex() const { return m_iBegIndex; }
	int GetDataWidth() const { return m_iDataWidth; }
	//������ʼ֡��
	void SetBegIndex(const int iBegIndex) { m_iBegIndex = iBegIndex; }
	void SetPageDataLen(const int iDataLen) { assert(iDataLen >= m_iPixWidth); m_iDataWidth = iDataLen; }

	//����/��ȡҳ����ʼ���к�
	void SetBegCol(const int &iCol) { m_iBegCol = iCol; }
	int GetBegCol() const { return m_iBegCol; }
	//����/��ȡ��ǰ�������к�
	void SetCurIndex(const int &iCurIndex) { m_iCurIndex = iCurIndex; }
	int GetCurCol() const { return (m_iCurIndex / GetRatio()); }
	int GetCurIndex() const { return m_iCurIndex; }

	//��ȡ��ǰҳ�����һ֡֡�� add <2020/10/10> xiongz
	int GetEndIndex() const { return m_iBegIndex + m_iDataWidth; }
	int GetEndCol() const { return m_iBegCol + m_iPixWidth; }
private:
	int m_iPixWidth;
	int m_iDataWidth;
	int m_iBegIndex;

	int m_iBegCol;		//ҳ����ʼ�к�
	int m_iCurIndex;	//��ǰ֡��
};