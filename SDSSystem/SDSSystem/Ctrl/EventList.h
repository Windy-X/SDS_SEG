#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>
#include "PublicDefs.h"



class EventList : public QWidget
{
	Q_OBJECT

public:
	EventList(QWidget *parent = NULL);
	~EventList();
	//*****************************************************
	// Method：  	Init	
	// Purpose：	初始化控件
	// Access：    	public
	// Returns：   	void
	// Parameter：	
	//		size		-[input] 控件尺寸
	//*****************************************************
	void Init(const QSize &size);
	void Clear();
signals:
	void SigEventMarkParam(const E_EVENT_TYPE &eEventType,const QColor &colorBackground,const QString &strDescription);
	void SigClearSubCtrlFocus();
private slots:
	virtual void paintEvent(QPaintEvent *event) override; 
	void OnClickBtn();
	void SlotEditInputReturnPressed();
	virtual void keyPressEvent(QKeyEvent *ev);

private:

private:
	QMap<E_EVENT_TYPE, QWidget *> m_mapWidget;
	QMap<E_EVENT_TYPE, MarkStyleSheetParam> m_mapMarkStyle;
	
	
};
