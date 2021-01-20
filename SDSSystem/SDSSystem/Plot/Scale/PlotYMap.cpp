/********************************************************************
* Copyright (C) 深圳市德力凯医疗设备股份有限公司
* All rights reserved.
*
* 文件名称：PlotYMap.cpp
* 摘    要：
*		Y轴标尺
* 历史记录：
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

// 判断标尺是否有效 =======================================
bool CPlotYMap::IsValid() const
{
	return (m_fUpBound != m_fDownBound && m_iPixDownBound != m_iPixUpBound);
}

// 初始化 =================================================
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

// 转化 ===================================================
int CPlotYMap::Trans(const double fDataValue) const 
{
	if (!IsValid())
	{
		return 0;
	}
	int i_pix = (fDataValue - m_fDownBound) / m_fRatio;
	return i_pix + m_iPixDownBound;
}

// 逆向转化 ===============================================
double CPlotYMap::InvTrans(const int iPixValue) const
{
	if (!IsValid())
	{
		return 0.0;
	}
	double f_value = (iPixValue - m_iPixDownBound) * m_fRatio;
	return f_value + m_fDownBound;
}

// 重新设置Y轴量程=========================================
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

// 重新设置Y轴像素尺寸 ====================================
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