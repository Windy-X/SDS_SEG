#pragma once
/********************************************************************
* Copyright (C)  �����е������������޹�˾
* All rights reserved
*
* �ļ����ƣ�SegMenu.h
* ժ    Ҫ������ѡ�β˵�
*		
* ��ʷ��¼��
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
	//����Ӳ˵���
	void AddItem(SegMenuItem *pItem);
	//*****************************************************
	// Method��  	AddItem	
	// Purpose��	��Ӳ˵�����
	//				�����Ӳ˵��ı������������ද̬�����Ӳ˵��������Ӳ˵�����ָ��
	// Access��    	public
	// Returns��   	�Ӳ˵�����ָ��		
	// Parameter��	
	//		strTitle		-[input] �Ӳ˵�����
	//*****************************************************
	SegMenuItem * AddItem(const QString &strTitle);
	//*****************************************************
	// Method��  	InsertItem	
	// Purpose��	�����Ӳ˵�
	// Access��    	public
	// Returns��   	void
	// Parameter��	
	//		i			-[input] ���������
	//		pItem		-[input] ��������ָ��
	//*****************************************************
	void InsertItem(int i, SegMenuItem *pItem);
	//*****************************************************
	// Method��  	DeleteItem	
	// Purpose��	ɾ��ָ�����Ӳ˵�
	// Access��    	public
	// Returns��   	void
	// Parameter��	
	//		i			-[input] ָ��������
	//*****************************************************
	void DeleteItem(int i);

	//ɾ�������Ӳ˵�
	void DeleteAllItem();
	void Clear();
	//��ȡ�Ӳ˵�
	QVector<SegMenuItem *> GetMenuItems() const { return m_vctItem; }
	//*****************************************************
	// Method��  	Popup	
	// Purpose��	�˵�����λ��
	// Access��    	public
	// Returns��   	void
	// Parameter��	
	//		p			-[input]ȫ������
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
	//���²˵�����
	void UpdatePlot();
	//debug xiongz
	void OnTimerDispear();
	void ClearAnimation();
private:
	QVector<SegMenuItem *> m_vctItem;
	QVector<QRect> m_vctRect;			//�Ӳ˵���rect��m_vct�����
	QSize m_sizeItem;					//�Ӳ˵��ߴ�
	QPropertyAnimation *m_pAnimation;	//��ʾ��ɫ����	 @xiongz [2020/11/6]
	QTimer m_timerDispear;				//��ʱ������ɫ�����˳�����	 @xiongz [2020/11/6]
};
