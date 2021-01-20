/********************************************************************
* Copyright (C)  �����е������������޹�˾
* All rights reserved
*
* �ļ����ƣ�MarkWidget.h
* ժ    Ҫ��
*		��ǲ�ؼ�����������ͼʾͼ��ʾ��������͸��
* ��ʷ��¼��
*			<2020/8/20>	xiongz,
* ********************************************************************/
#pragma once

#include <QWidget>
#include <QResizeEvent>
#include <QThread>
#include "PlotLayers/PlotLayerSeg.h"
#include "PlotData/PlotSegmentData.h"
#include "Scale/PlotXMap.h"
#include "SegMenu.h"
const QColor C_COLOR_BASIC_STATUS = QColor(255,255,255);
const QColor C_COLOR_P0 = QColor(0,255,0);
const QColor C_COLOR_P1 = QColor("#15d6ba");
const QColor C_COLOR_P2 = QColor(135, 206, 250);
const QColor C_COLOR_P3 = QColor(173, 255, 47);
const QColor C_COLOR_P4 = QColor(255,215,0);
const QColor C_COLOR_P5 = QColor(255, 105, 180);



class CMarkWidget : public QWidget
{
	Q_OBJECT

public:
	CMarkWidget(QWidget *parent );
	virtual ~CMarkWidget();
signals:
	void SigAxisXChanged(const CPlotXMap& cAxisX);
	//����ѡ��֡��
	void SigSendSegment(const int& iBeginIndex, const int &iEndIndex);
	//
	void SigSendSegTracking(const bool &bIsTracking);
private slots:
	virtual void paintEvent(QPaintEvent *) override;
	virtual void resizeEvent(QResizeEvent *event) override;
	//������¼�
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;

	//debug�޸ĵ�ǰѡ����ɫ <2020/11/27> xiongz
	void SetCurrentSegBackground(const QColor &color);
public slots:
	void SetAxisX(const CPlotXMap& cAxisX);
	//���õ�ǰ��
	void SetCurCol(const qint64 &iCurCol) { m_iCurCol = iCurCol; }
	//*****************************************************
	// Method��  	SlotMarkEvent	
	// Purpose��	ʵʱ��������ʱ�����¼����
	//				ʵʱɨ��ʱ���¼�������͹̶�����ҩ�������������½������Ⱥ��Զ���
	//				������ѡ������¼����ģʽ�����źţ��ú�����ͣ����ˢ�£������һ֡���ݽ�β���Ʊ����
	//				������¼�����
	// Access��    	public
	// Returns��   	void
	// Parameter��	
	//		eEventType		-[input] �¼�����
	//		colorBackground	-[input] ����ɫ
	//		strDesctiption	-[input] �¼�����
	//*****************************************************
	void SlotMarkEvent(const E_EVENT_TYPE &eEventType, const QColor &colorBackground, const QString &strDescription);
	//*****************************************************
	// Method��  	SetCurrentLayout	
	// Purpose��	���õ�ǰ�Ĳ������ú͵�ǰ�Ĳ��֣���������ĵ磩,���Ҹ���ѡ�β���SegmentMarks
	// Access��    	
	// Returns��   			
	// Parameter��	
	//
	//*****************************************************
	void SetCurrentLayout(const E_LAYOUT_TYPE &eLayout, const QMap<E_LAYOUT_TYPE,QVector<SegmentChannelParam>> &);
	//*****************************************************
	// Method��  	SlotSetMeasureInfo	
	// Purpose��	����ÿ�����ߵ�ͳ����Ϣ�Ĳۺ���
	//				��Ҫ�Ϳؼ��ⲿͳһ�ṹ��
	// Access��    	pbulic
	// Returns��   	void
	// Parameter��	
	//		mapMearsureInfo			-[input] ͳ����Ϣ�ṹ��ӳ��
	//
	//*****************************************************
	void SlotSetMearsureInfo(const QMap<E_DATA_TYPE,MeasureInfo> &mapMearsureInfo);

public:
	//*****************************************************
	// Method��  	InitDlg	
	// Purpose��	��ʼ����ǿؼ�
	// Access��    	public
	// Returns��   	void	
	// Parameter��	
	//		rect		-[input] �ؼ�λ��
	//*****************************************************
	void InitDlg();
	void UpdatePlot();
	//*****************************************************
	// Method��  	AppendSegMarks	
	// Purpose��	���ѡ������,�ⲿ���ã���ѡ��ģʽ�£�����ѡ�β���֮�����
	// Access��    	public
	// Returns��   	void	
	// Parameter��	
	//		stMarks		-[input] ѡ������
	//*****************************************************
	void AppendSegMarks(const SegmentMarks& stMarks);
	void AppendSegMarks(const QVector<SegmentMarks>& vctMarks);
	//�����ڴ�
	void Clear();

	//*****************************************************
	// Method��  	SetTrack	
	// Purpose��	���õ�ǰ�Ƿ�ʹ��������ѡ�β���,��������������
	// Access��    	public
	// Returns��   	void		
	// Parameter��	
	//		bTracking		-[input] �Ƿ����ѡ�β�����true - ����ѡ�Σ�false - û��ѡ��
	//*****************************************************
	void SetTracking(const bool& bTracking);
	//��ȡ��ǰ��ѡ�β���״̬
	bool GetTracking()const { return m_bIsTracking; }
	//*****************************************************
	// Method��  	SetSegMode	
	// Purpose��	���ý���/�˳�ѡ��ģʽ
	// Access��    	public
	// Returns��   	void
	// Parameter��	
	//		bSegmode		-[input] �Ƿ����ѡ��ģʽ
	//*****************************************************
	void SetSegMode(const bool& bSegmode) { m_bIsSegMode = bSegmode; }
	bool GetSegMode() const { return m_bIsSegMode; }

	//����/��ȡ�¼����״̬
	void SetEventMode(const bool &bEvent) { m_bIsSegMode = bEvent; }
	bool GetEventMode() const { return m_bIsEventMode; }
	//��ȡѡ��ͼ��ɼ�״̬
	bool IsLayerVisible() const;
	//����ͼ���Ƿ�ɼ�
	void SetLayerVisible(const bool& bVisible);

	//TODO:��������ģ�����ͼ�������
	//*****************************************************
	// Method��  	CaculateChannelRect	
	// Purpose��	��������ģ�����ʾ����
	// Access��    	
	// Returns��   			
	// Parameter��	
	//
	//*****************************************************
	void CaculateChannelRect(const E_CHANNEL_TYPE &eType,QRect *pRect);
	void SetCurrentSegment(const SegMeasureInfo &stMeasureInfo) { m_stSegMarks.stMeasureInfo = stMeasureInfo; };

	//TODO:��Ӳۺ�����ˢ�µ�ǰ��ʾ�ĵ�
	//TODO:����ÿ�����ߵ���Ϣ����
	//TODO:����������ʾ����ɫ
	void SetCurveColor(const E_CHANNEL_CURVE &eCurve,const QColor);
	//debug <2020/12/1>xiongz
	void SetMeasureInfo(QMap<E_DATA_TYPE,MeasureInfo> &mapMeasureInfo);

	//����ѡ��
	void SaveSegmentMarks();


	//TODO:�����������ͼ���Ҳ�ʱ�������Լ�����
	//TODO:��ѡ�δ���ʾ����ͼ�Ĳ�����ʱ����������������ֵ�����ֵ����Сֵ����Сֵ/���ֵ
private:
	//��ѡ�α��״̬�°��������� add <2020/10/17> xiongz
	void OnSegMousePress(QMouseEvent *pEvent);
	//�¼�����°���������
	void OnEventMousePress(QMouseEvent *pEvent);
	//ѡ�α��״̬���ɿ�������
	void OnSegMouseRelease(QMouseEvent *pEvent);
	//����ѡ�α�ǲ���
	void MarkSeg(QMouseEvent *pEvent);
	//��ѡ��ģʽ�£����������ʽ
	void EnterEvent(QEvent* pEvent);
	//����˳��¼�
	void LeaveEvent(QEvent *pEvent);
	//�Ƚ������յ�λ�ã������С���յ�λ�ã���������ֵ

private:
	CPlotLayerSeg *m_pSegLayer;

	bool m_bIsTracking;					//�Ƿ��ڽ���ѡ��
	bool m_bIsSegMode;					//�Ƿ���ѡ��ģʽ

	bool m_bIsEventMode;				//�Ƿ�����ʱ��ģʽ add <2020/10/16> xiongz

	CPlotSegmentData *m_pSegData;		//���ݲ����ָ��	
	qint64 m_iBegCol;					//�����꣬��ʼ������
	qint64 m_iEndCol;					//�ɿ���꣬����������
	qint64 m_iCurCol;					//��ǰ��

	SegMenu *m_pMenu;					//ѡ�β˵��ؼ�
	//����<2020/11/30> xiongz
	SegmentMarks m_stSegMarks;			//��ǰѡ��
	E_LAYOUT_TYPE m_eCurrentLayout;		//��ǰ���֣�
	SegmentConfig m_SegConfig;

	
	
				
};
