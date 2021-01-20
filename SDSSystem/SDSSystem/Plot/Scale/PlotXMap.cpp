/********************************************************************
* Copyright (C) 深圳市德力凯医疗设备股份有限公司
* All rights reserved.
*
* 文件名称：PlotXScale.cpp
* 摘    要：
*		X轴标尺类
* 历史记录：
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

// 初始化 =================================================
void CPlotXMap::Init(const int iPixWidth, const int iDataWidth, const int iStartIndex)
{
	m_iPixWidth = iPixWidth;
	m_iDataWidth = iDataWidth;
	m_iBegIndex = iStartIndex;

}

// 转化为像素位置 =========================================
int CPlotXMap::Trans(const int iDataIndex) const 
{
	int i_dist = iDataIndex;
	double fRatio = double(m_iPixWidth) / m_iDataWidth;
	return fRatio * i_dist;
}

// 逆向转化为帧号 =========================================
int CPlotXMap::InvTrans(const int iPixIndex) const
{
	double fRatio = double(m_iDataWidth) / m_iPixWidth;
	//int i_dist = fRatio * iPixIndex;
	//return m_iBegIndex + i_dist;
	return ceil(iPixIndex * fRatio);
}