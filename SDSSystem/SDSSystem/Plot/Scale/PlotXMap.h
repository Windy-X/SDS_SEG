/********************************************************************
* Copyright (C) 深圳市德力凯医疗设备股份有限公司
* All rights reserved.
*
* 文件名称：PlotXScale.h
* 摘    要：
*		时间轴（X轴）标尺类，将帧号与像素点相互转化
* 历史记录：
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
	// Purpose:		初始化X轴
	// Access:    	pubulic
	// Returns:   	void
	// Parameter: 	
	//		iPixWidth		-[input] 像素宽度
	//		iDataWidth		-[input] 数据宽度（共多少帧数据）
	//		iStartIndex		-[input] 页面起始帧号
	//*****************************************************
	void Init(const int iPixWidth, const int iDataWidth, const int iStartIndex);

	//*****************************************************
	// Method:    	Trans
	// Purpose:		将帧号根据配置转换为像素位置
	// Access:    	public
	// Returns:   	像素位置
	// Parameter: 	
	//		iDataIndex		-[input] 帧号
	//***************************************************
	int Trans(const int iDataIndex) const;

	//*****************************************************
	// Method:    	InvTrans
	// Purpose:		将像素位置转换为帧号
	// Access:    	public
	// Returns:   	帧号
	// Parameter: 	iPixIndex		-[input] 像素位置
	//*****************************************************
	int InvTrans(const int iPixIndex) const;

	// 获取压缩比例
	double GetRatio() const { return double(m_iDataWidth) / m_iPixWidth; }

	int GetPixWidth() const { return m_iPixWidth; }
	int GetBegIndex() const { return m_iBegIndex; }
	int GetDataWidth() const { return m_iDataWidth; }
	//设置起始帧号
	void SetBegIndex(const int iBegIndex) { m_iBegIndex = iBegIndex; }
	void SetPageDataLen(const int iDataLen) { assert(iDataLen >= m_iPixWidth); m_iDataWidth = iDataLen; }

	//设置/获取页面起始的列号
	void SetBegCol(const int &iCol) { m_iBegCol = iCol; }
	int GetBegCol() const { return m_iBegCol; }
	//设置/获取当前索引和列号
	void SetCurIndex(const int &iCurIndex) { m_iCurIndex = iCurIndex; }
	int GetCurCol() const { return (m_iCurIndex / GetRatio()); }
	int GetCurIndex() const { return m_iCurIndex; }

	//获取当前页面最后一帧帧号 add <2020/10/10> xiongz
	int GetEndIndex() const { return m_iBegIndex + m_iDataWidth; }
	int GetEndCol() const { return m_iBegCol + m_iPixWidth; }
private:
	int m_iPixWidth;
	int m_iDataWidth;
	int m_iBegIndex;

	int m_iBegCol;		//页面起始列号
	int m_iCurIndex;	//当前帧号
};