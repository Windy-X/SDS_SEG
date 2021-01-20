/********************************************************************
* Copyright (C)  深圳市德力凯电子有限公司
* All rights reserved
*
* 文件名称：SegMenuItem.h
* 摘    要：选段菜单的子项
*		
* 历史记录：
*			<2020/10/30>	xiongz,
* ********************************************************************/
#pragma once
#include <QWidget>
#include <QString>
#include <QEvent>
#include <QMouseEvent>
#include <QObject>

//边框配置
const int C_UNHOVER_BORDER_WIDTH = 1;	// 鼠标未悬浮边界线宽度
const int C_HOVER_BORDER_WIDTH = 3;		//鼠标悬浮边界线宽度
const QColor C_COLOR_HOVER = Qt::green;
const QColor C_COLOR_UNHOVER = Qt::black;
const QColor C_COLOR_DEFAULT_BACKGROUND = Qt::white;
const QColor C_COLOR_FONT = Qt::white;
const int C_FONT_DEFAULT_SIZE = 20;
const char C_FONT_FAMILY[] = "arial";
struct ItemCfg 
{
	QColor colorBorder;		//边框颜色
	QColor colorBackground;	//背景色
	QColor colorFont;		//字体颜色
	int iWidth;				//边框宽度
	QFont font;				//菜单字体
	inline ItemCfg()
	{
		colorBorder = C_COLOR_UNHOVER;
		colorBackground = C_COLOR_DEFAULT_BACKGROUND;
		colorFont = C_COLOR_FONT;
		font = QFont(C_FONT_FAMILY, C_FONT_DEFAULT_SIZE, QFont::Thin, false);
		iWidth = C_UNHOVER_BORDER_WIDTH;
	}
};
class SegMenuItem : public QWidget
{
	Q_OBJECT

public:
	SegMenuItem(QWidget *parent = NULL);
	SegMenuItem(const QString &strTitle, QWidget *parent);
	~SegMenuItem();
public:
	//制定子控件的
	void InitItem(const QSize &size);
	QSize GetSize() const { return size(); }
	//设置空间背景颜色
	void SetBackground(const QColor& color);
	QColor GetBackground() const { return m_stItemCfg.colorBackground; }
	//设置/获取菜单文本
	void SetText(const QString &strCaption);
	QString GetText() const { return m_strCaption; }
	//设置/获取菜单字体
	void SetFont(const QFont &font);
	QFont GetFont() const { return m_stItemCfg.font; };
	//设置/获取子菜单配置
	void SetItemCfg(const ItemCfg &stItemCfg);
	ItemCfg GetItemCfg() const { return m_stItemCfg; }
signals:
	void Sigtriggered(const QColor &);
protected slots:
	//重载event函数
	virtual bool event(QEvent *e) override;
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *ev) override;
private:
	//绘制菜单字体
	void DrawText(QPainter *pp);

private:

	QString m_strCaption;		//菜单名
	QColor m_colorBackground;	//背景色
	ItemCfg m_stItemCfg;		//背景色；


};
