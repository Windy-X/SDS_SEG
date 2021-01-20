/********************************************************************
* Copyright (C)  深圳市德力凯电子有限公司
* All rights reserved
*
* 文件名称：TrendWidget.h
* 摘    要：
*		趋势图显示的区域,暂时先不做
* 历史记录：
*			<2020/8/20>	xiongz,
* ********************************************************************/
#pragma once
//TODO:主窗口的子窗口，所有绘图创建属于该窗口的子窗口
#include <QWidget>
#include "Plot.h"

class CTrendWidget : public QWidget
{
	Q_OBJECT

public:
	CTrendWidget(QWidget *parent);
	~CTrendWidget();
private:
	
};
