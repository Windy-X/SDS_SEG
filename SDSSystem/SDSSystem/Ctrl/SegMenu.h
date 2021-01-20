#pragma once
/********************************************************************
* Copyright (C)  深圳市德力凯电子有限公司
* All rights reserved
*
* 文件名称：SegMenu.h
* 摘    要：定制选段菜单
*		
* 历史记录：
*			<2020/10/30>	xiongz,
* ********************************************************************/

#include <QWidget>
#include <QVector>
#include <QEvent>
#include <QPainter>
#include <QLine>
#include <QPropertyAnimation>
#include <QTimer>
#include "windows.h"
#include "SegMenuItem.h"

static const int C_MENU_DISPEAR_TIME = 2000;
static const int C_MENU_TIMER_OUT = 8000;
class SegMenu : public QWidget
{
	Q_OBJECT

public:
	SegMenu(QWidget *parent = NULL);
	~SegMenu();
	void SetItemSize(const QSize &size);
	QSize GetItemSize() const { return m_sizeItem; }
	//添加子菜单项
	void AddItem(SegMenuItem *pItem);
	//*****************************************************
	// Method：  	AddItem	
	// Purpose：	添加菜单子项
	//				输入子菜单的标题名，函数类动态生成子菜单并返回子菜单窗口指针
	// Access：    	public
	// Returns：   	子菜单窗口指针		
	// Parameter：	
	//		strTitle		-[input] 子菜单标题
	//*****************************************************
	SegMenuItem * AddItem(const QString &strTitle);
	//*****************************************************
	// Method：  	InsertItem	
	// Purpose：	插入子菜单
	// Access：    	public
	// Returns：   	void
	// Parameter：	
	//		i			-[input] 插入的索引
	//		pItem		-[input] 插入对象的指针
	//*****************************************************
	void InsertItem(int i, SegMenuItem *pItem);
	//*****************************************************
	// Method：  	DeleteItem	
	// Purpose：	删除指定项子菜单
	// Access：    	public
	// Returns：   	void
	// Parameter：	
	//		i			-[input] 指定项索引
	//*****************************************************
	void DeleteItem(int i);

	//删除所有子菜单
	void DeleteAllItem();
	void Clear();
	//获取子菜单
	QVector<SegMenuItem *> GetMenuItems() const { return m_vctItem; }
	//*****************************************************
	// Method：  	Popup	
	// Purpose：	菜单弹出位置
	// Access：    	public
	// Returns：   	void
	// Parameter：	
	//		p			-[input]全局坐标
	//
	//*****************************************************
	void Popup(const QPoint &p);
	
protected:
	virtual bool event(QEvent *ev) override;
	virtual void paintEvent(QPaintEvent *ev) override;
signals:
	void SigUpdateSize();
public slots:
	void InitMenu();
	//更新菜单布局
	void UpdatePlot();
	//debug xiongz
	void OnTimerDispear();
	void ClearAnimation();
private:
	QVector<SegMenuItem *> m_vctItem;
	QVector<QRect> m_vctRect;			//子菜单的rect和m_vct相关联
	QSize m_sizeItem;					//子菜单尺寸
	QPropertyAnimation *m_pAnimation;	//显示颜色渐变	 @xiongz [2020/11/6]
	QTimer m_timerDispear;				//定时出发颜色渐变退出操作	 @xiongz [2020/11/6]
};
