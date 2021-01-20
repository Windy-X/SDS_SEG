/********************************************************************
* Copyright (C)  �����е������������޹�˾
* All rights reserved
*
* �ļ����ƣ�MyLineEdit.h
* ժ    Ҫ������QLineEdit�ؼ����ؼ���ȡ����ʱ�򸸴��ڷ����ź�
*		
* ��ʷ��¼��
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
