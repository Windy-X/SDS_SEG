/********************************************************************
* Copyright (C)  �����е������������޹�˾
* All rights reserved
*
* �ļ����ƣ�PlotLayerSeg.h
* ժ    Ҫ��
*		ѡ��ͼ��
* ��ʷ��¼��
*			Created <2020/9/2>	xiongz,
* ********************************************************************/
#pragma once
#include <QFont>
#include "PlotLayers/PlotLayerBase.h"
#include "Scale/PlotXMap.h"
#include "PublicDefs.h"
class CPlotLayerSeg : public CPlotLayerBase
{
public:
	 CPlotLayerSeg();
	virtual ~CPlotLayerSeg();
	//����/��ȡ���
	void SetStartPos(const int &iStartPos);
	void SetStartPos(const QPoint &point);
	int GetStartIndex() const { return m_iStartIndex; }
	//����/��ȡ�յ�
	void SetEndPos(const int &iEndPos);
	void SetEndPos(const QPoint &point);
	int GetEndIndex() const { return m_iEndIndex; }

	//��ȡѡ��λ��
	void GetSegIndex(int *pStart, int *pLen) const { (*pStart) = m_iStartIndex; (*pLen) = m_iEndIndex - m_iStartIndex; }

	//�Ƿ�ʼѡ��
	void SetTracking(bool bIsTracking);
	//�Ƿ����¼�
	void SetMarkEvent(const bool &bIsEventMode);
	//�û�ѡ��
	void DrawUserTrack(const QPoint& point);
	//����ͼ�����ͼ��
	void UpdatePix(const QVector<SegmentMarks>& vctSegment,const SegmentView &stView,const CPlotXMap &cAxisX);
	//���Ƶ�ɫ
	void DrawBottom();
	//�����¼������
	void DrawMarkEventLine(const QVector<StampedEvent> &vctMark,const CPlotXMap &cAxisX);
	//��¼���pixmap״̬
	void StoreLastPixmap();
	//������pixmap״̬ǰ��һ��
	void RestoreLastPixmap();
	
	//===================����ѡ��ͳ����Ϣ <2020/11/18> xiongz==================
	//*****************************************************
	// Method��  	DrawSegmentMeasureInfo	
	// Purpose��	����ѡ�β�����Ϣ
	// Access��    	public
	// Returns��   	void 		
	// Parameter��	
	//		stChannel		-[input] ÿһ��Ҫ��ʾ���������ã����壬��ɫ������
	//		font			-[input] ����
	//		rectChannel		-[input] ÿһ����rectλ��
	//*****************************************************
	void DrawSegmentMeasureInfo(QPainter * pp,const SegmentChannelMeasure &stChannel,const QFont &font,const QRect &rectChannel);
	//���Ʊ���
	void DrawSegmentMeasureInfoTitle(QPainter * pp,
		const QFont &font,
		const QColor &color,
		const int &iPulses,
		int iTime,
		const QRect &rectSeg);
	void UpdateMeasureInfoPix(const QVector<SegmentMarks>& vctSegment, const SegmentConfig &stSegConfig,const CPlotXMap &cAxisX);




private:
	QPixmap m_lastPix;			//��¼���ѡ��״̬��pixmap

	int m_iStartIndex;
	int m_iEndIndex;
	bool m_bTracking;
	bool m_bIsEventMode;


};

