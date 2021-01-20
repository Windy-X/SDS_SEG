/********************************************************************
* Copyright (C)  深圳市德力凯电子有限公司
* All rights reserved
*
* 文件名称：MyLineEdit.h
* 摘    要：重载QLineEdit控件，控件获取焦点时向父窗口发送信号
*		
* 历史记录：
*			<2020/10/23>	xiongz,
* ********************************************************************/
#pragma once

#include <QObject>
#include <QLineEdit>
#include <QFocusEvent>
class MyLineEdit : public QLineEdit
{
	Q_OBJECT
signals:
	void SigEditGetFocus();
protected slots:
	virtual void focusInEvent(QFocusEvent *ev) override;
public:
	MyLineEdit(QWidget *parent = NULL);
	~MyLineEdit();
};
