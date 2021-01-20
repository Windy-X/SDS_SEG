/********************************************************************
* Copyright (C)  �����е������������޹�˾
* All rights reserved
*
* �ļ����ƣ�SegMenuItem.h
* ժ    Ҫ��ѡ�β˵�������
*		
* ��ʷ��¼��
*			<2020/10/30>	xiongz,
* ********************************************************************/
#pragma once
#include <QWidget>
#include <QString>
#include <QEvent>
#include <QMouseEvent>
#include <QObject>

//�߿�����
const int C_UNHOVER_BORDER_WIDTH = 1;	// ���δ�����߽��߿��
const int C_HOVER_BORDER_WIDTH = 3;		//��������߽��߿��
const QColor C_COLOR_HOVER = Qt::green;
const QColor C_COLOR_UNHOVER = Qt::black;
const QColor C_COLOR_DEFAULT_BACKGROUND = Qt::white;
const QColor C_COLOR_FONT = Qt::white;
const int C_FONT_DEFAULT_SIZE = 20;
const char C_FONT_FAMILY[] = "arial";
struct ItemCfg 
{
	QColor colorBorder;		//�߿���ɫ
	QColor colorBackground;	//����ɫ
	QColor colorFont;		//������ɫ
	int iWidth;				//�߿���
	QFont font;				//�˵�����
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
	//�ƶ��ӿؼ���
	void InitItem(const QSize &size);
	QSize GetSize() const { return size(); }
	//���ÿռ䱳����ɫ
	void SetBackground(const QColor& color);
	QColor GetBackground() const { return m_stItemCfg.colorBackground; }
	//����/��ȡ�˵��ı�
	void SetText(const QString &strCaption);
	QString GetText() const { return m_strCaption; }
	//����/��ȡ�˵�����
	void SetFont(const QFont &font);
	QFont GetFont() const { return m_stItemCfg.font; };
	//����/��ȡ�Ӳ˵�����
	void SetItemCfg(const ItemCfg &stItemCfg);
	ItemCfg GetItemCfg() const { return m_stItemCfg; }
signals:
	void Sigtriggered(const QColor &);
protected slots:
	//����event����
	virtual bool event(QEvent *e) override;
	virtual void paintEvent(QPaintEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *ev) override;
private:
	//���Ʋ˵�����
	void DrawText(QPainter *pp);

private:

	QString m_strCaption;		//�˵���
	QColor m_colorBackground;	//����ɫ
	ItemCfg m_stItemCfg;		//����ɫ��


};
