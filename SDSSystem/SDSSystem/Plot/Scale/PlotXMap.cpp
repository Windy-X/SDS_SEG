/********************************************************************
* Copyright (C) �����е�����ҽ���豸�ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�PlotXScale.cpp
* ժ    Ҫ��
*		X������
* ��ʷ��¼��
*		<2020/04/26> dingmh, Created
*********************************************************************/
#include "PlotXMap.h"
#include "Config.h"

CPlotXMap::CPlotXMap()
	:m_iPixWidth(0)
	,m_iDataWidth(0)
	,m_iBegIndex(0)
	,m_iBegCol(0)
	,m_iCurIndex(-1)
{
}

CPlotXMap::~CPlotXMap()
{
}

// ��ʼ�� =================================================
void CPlotXMap::Init(const int iPixWidth, const int iDataWidth, const int iStartIndex)
{
	m_iPixWidth = iPixWidth;
	m_iDataWidth = iDataWidth;
	m_iBegIndex = iStartIndex;

}

// ת��Ϊ����λ�� =========================================
int CPlotXMap::Trans(const int iDataIndex) const 
{
	int i_dist = iDataIndex;
	double fRatio = double(m_iPixWidth) / m_iDataWidth;
	return fRatio * i_dist;
}

// ����ת��Ϊ֡�� =========================================
int CPlotXMap::InvTrans(const int iPixIndex) const
{
	double fRatio = double(m_iDataWidth) / m_iPixWidth;
	//int i_dist = fRatio * iPixIndex;
	//return m_iBegIndex + i_dist;
	return ceil(iPixIndex * fRatio);
}