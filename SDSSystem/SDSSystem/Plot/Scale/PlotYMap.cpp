/********************************************************************
* Copyright (C) �����е�����ҽ���豸�ɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�PlotYMap.cpp
* ժ    Ҫ��
*		Y����
* ��ʷ��¼��
*		<2020/04/27> dingmh, Created
*********************************************************************/
#include "PlotYMap.h"

CPlotYMap::CPlotYMap()
	: m_fDownBound(0.0)
	, m_fUpBound(0.0)
	, m_iPixDownBound(0)
	, m_iPixUpBound(0)
	, m_fRatio(0)
{
}

CPlotYMap::~CPlotYMap()
{
}

// �жϱ���Ƿ���Ч =======================================
bool CPlotYMap::IsValid() const
{
	return (m_fUpBound != m_fDownBound && m_iPixDownBound != m_iPixUpBound);
}

// ��ʼ�� =================================================
void CPlotYMap::Init(const double fDownBound, const double fUpBound, const int iPixDownBound, const int iPixUpBound)
{
	m_fDownBound = fDownBound;
	m_fUpBound = fUpBound;
	m_iPixDownBound = iPixDownBound;
	m_iPixUpBound = iPixUpBound;
	if (IsValid())
	{
		m_fRatio = (m_fUpBound - m_fDownBound) / (m_iPixUpBound - m_iPixDownBound);
	}
	else
	{
		m_fRatio = 0;
	}	
}

// ת�� ===================================================
int CPlotYMap::Trans(const double fDataValue) const 
{
	if (!IsValid())
	{
		return 0;
	}
	int i_pix = (fDataValue - m_fDownBound) / m_fRatio;
	return i_pix + m_iPixDownBound;
}

// ����ת�� ===============================================
double CPlotYMap::InvTrans(const int iPixValue) const
{
	if (!IsValid())
	{
		return 0.0;
	}
	double f_value = (iPixValue - m_iPixDownBound) * m_fRatio;
	return f_value + m_fDownBound;
}

// ��������Y������=========================================
void CPlotYMap::SetDataRange(const double fDownBound, const double fUpBound)
{
	m_fDownBound = fDownBound;
	m_fUpBound = fUpBound;
	if (IsValid())
	{
		m_fRatio = (m_fUpBound - m_fDownBound) / (m_iPixUpBound - m_iPixDownBound);
	}
	else
	{
		m_fRatio = 0;
	}
}

// ��������Y�����سߴ� ====================================
void CPlotYMap::ResizeScale(const int iPixDownBound, const int iPixUpBound)
{
	m_iPixDownBound = iPixDownBound;
	m_iPixUpBound = iPixUpBound;
	if (IsValid())
	{
		m_fRatio = (m_fUpBound - m_fDownBound) / (m_iPixUpBound - m_iPixDownBound);
	}
	else
	{
		m_fRatio = 0;
	}
}