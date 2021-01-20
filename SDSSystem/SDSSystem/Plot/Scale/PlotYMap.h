/********************************************************************
* Copyright (C) 深圳市德力凯医疗设备股份有限公司
* All rights reserved.
*
* 文件名称：PlotYMap.h
* 摘    要：
*		Y轴标尺
* 历史记录：
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
	// Purpose:		初始化Y轴
	// Access:    	public
	// Returns:   	void
	// Parameter: 	
	//		fDownBound		-[input] 数据下限
	//		fUpBound		-[input] 数据上限
	//		iPixDownBound	-[input] 与数据下限对应的Y轴像素位置
	//		iPixUpBound		-[input] 与数据上限对应的Y轴像素位置（通常为0）
	//*****************************************************
	void Init(const double fDownBound, const double fUpBound,
		const int iPixDownBound, const int iPixUpBound);

	//*****************************************************
	// Method:    	Trans
	// Purpose:		将数据转化为像素位置
	// Access:    	public
	// Returns:   	像素位置
	// Parameter: 	fDataValue	-[input] 数据值
	//*****************************************************
	int Trans(const double fDataValue) const;

	//*****************************************************
	// Method:    	InvTrans
	// Purpose:		将像素位置转化为数据
	// Access:    	public
	// Returns:   	数值
	// Parameter: 	iPixValue	-[input] 像素值
	//*****************************************************
	double InvTrans(const int iPixValue) const;

	//*****************************************************
	// Method:    	SetDataRange
	// Purpose:		设置Y轴量程
	// Access:    	public
	// Returns:   	void
	// Parameter: 	
	//		fDownBound		-[input] 数据下限
	//		fUpBound		-[input] 数据上限
	//*****************************************************
	void SetDataRange(const double fDownBound, const double fUpBound);

	//*****************************************************
	// Method:    	ResizeScale
	// Purpose:		重设Y轴像素尺寸
	// Access:    	public
	// Returns:   	void
	// Parameter: 	
	//		iPixDownBound	-[input] 与数据下限对应的Y轴像素位置
	//		iPixUpBound		-[input] 与数据上限对应的Y轴像素位置（通常为0）
	//*****************************************************
	void ResizeScale(const int iPixDownBound, const int iPixUpBound);

	//标尺是否有效 ========================================
	bool IsValid() const;

	// 判断是否在标尺范围内 ===============================
	bool Contains(const double fValue) const { return (fValue >= m_fDownBound) && (fValue <= m_fUpBound); }

	double GetValue2PixRatio() const { return m_fRatio; }
private:
	double m_fDownBound;
	double m_fUpBound;
	int	m_iPixDownBound;
	int m_iPixUpBound;
	double m_fRatio;
};

