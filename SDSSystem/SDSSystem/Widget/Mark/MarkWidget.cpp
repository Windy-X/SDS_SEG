#include "MarkWidget.h"
#include <QPalette>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QDateTime>
#include "Plot.h"
#include "Config.h"
using namespace std;

CMarkWidget::CMarkWidget(QWidget *parent)
	: QWidget(parent)
	,m_pSegLayer(NULL)
	,m_bIsSegMode(false)
	,m_bIsTracking(false)
	,m_bIsEventMode(false)
	,m_iBegCol(-1)
	,m_iEndCol(-1)
	,m_pSegData(NULL)
	,m_iCurCol(0)
	,m_pMenu(NULL) //��ʼ���˵�	 @xiongz [2020/11/3]
{
	setMouseTracking(true);
	InitDlg();
}

CMarkWidget::~CMarkWidget()
{
	if( m_pSegLayer )
	{
		delete m_pSegLayer;
		m_pSegLayer = NULL;
	}
	Clear();
}
void CMarkWidget::InitDlg()
{
	QRect rct = this->rect();
	if (NULL == m_pSegLayer)
	{
		m_pSegLayer = new CPlotLayerSeg();
	}
	m_pSegLayer->InitLayer(rct.size());
	m_pSegData = CPlotSegmentData::GetInstance();
	m_pSegData->Init();
	//���ò˵� @xiongz [2020/11/3]
	if (!m_pMenu)
	{
		m_pMenu = new SegMenu(this);
	}
	m_pMenu->SetItemSize(QSize(200,40));
	SegMenuItem *p_item = new SegMenuItem(tr("set basic status"),m_pMenu);
	p_item->SetBackground(C_COLOR_BASIC_STATUS);
	m_pMenu->AddItem(p_item);
	p_item = new SegMenuItem("*P0", m_pMenu);
	p_item->SetBackground(C_COLOR_P0);
	m_pMenu->AddItem(p_item);
	connect(p_item,SIGNAL(Sigtriggered(const QColor &)),this,SLOT(SetCurrentSegBackground(const QColor &)));
	p_item = new SegMenuItem("P1", m_pMenu);
	p_item->SetBackground(C_COLOR_P1);
	m_pMenu->AddItem(p_item);
	p_item = new SegMenuItem("P2", m_pMenu);
	p_item->SetBackground(C_COLOR_P2);
	m_pMenu->AddItem(p_item);
	p_item = new SegMenuItem("P3", m_pMenu);
	p_item->SetBackground(C_COLOR_P3);
	m_pMenu->AddItem(p_item);
	p_item = new SegMenuItem("P4", m_pMenu);
	p_item->SetBackground(C_COLOR_P4);
	m_pMenu->AddItem(p_item);
	p_item = new SegMenuItem("P5", m_pMenu);
	p_item->SetBackground(C_COLOR_P5);
	m_pMenu->AddItem(p_item);
	//��ȡͳ����Ϣ����
	m_SegConfig = CConfig::Instance()->GetSegmentConfig();
	m_stSegMarks.stView = m_SegConfig.stView;


}
void CMarkWidget::paintEvent(QPaintEvent *event)
{

	QPainter pp(this);
	QRect rct = this->rect();
	pp.drawPixmap(QPoint(0, 0), m_pSegLayer->GetPixmap());
}
void CMarkWidget::mousePressEvent(QMouseEvent *event)
{
	if (IsLayerVisible())
	{
		if(m_bIsSegMode)
		{
			OnSegMousePress(event);
		}
		if (m_bIsEventMode)
		{
			OnEventMousePress(event);
		}
		
	}
	
}
void CMarkWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (IsLayerVisible())
	{
		if (m_bIsSegMode & m_bIsTracking & !m_bIsEventMode)
		{

			OnSegMouseRelease(event);
		}
		else if (!m_bIsSegMode & m_bIsEventMode)
		{
			OnEventMousePress(event);
		}

	}
	
}
void CMarkWidget::mouseMoveEvent(QMouseEvent *event)
{
	//�ж�����Ƿ��ڿؼ���Χ
	if (this->rect().contains(event->pos()))
	{

		QEvent ev(QEvent::None);
		if(IsLayerVisible())
		{
			if (m_bIsSegMode && m_bIsTracking)
			{
				//�������
				EnterEvent(&ev);
				if (m_bIsTracking)
				{
					if ( Qt::LeftButton != event->button() && (Qt::LeftButton & (event->buttons())))
					{
						//debug
						qDebug() << "MarkWidget mouse move event";
						MarkSeg(event);

					}
				}
			}
			else
			{
				LeaveEvent(&ev);
			}
		}	

	}
}
void CMarkWidget::UpdatePlot()
{

	m_pSegLayer->UpdatePix(m_pSegData->GetSegMarks(),m_SegConfig.stView,m_pSegData->GetTimeMap());
	//m_pSegLayer->DrawMarkEventLine(m_pSegData->GetEventMarks(),m_pSegData->GetTimeMap());
}
void CMarkWidget::AppendSegMarks(const SegmentMarks& stMarks)
{
	m_pSegData->AppendSeg(stMarks);
}
void CMarkWidget::AppendSegMarks(const QVector<SegmentMarks>& vctMarks)
{
	m_pSegData->AppendSeg(vctMarks);
}
void CMarkWidget::Clear()
{
	
}
//ѡ��ģʽ�°���������
void CMarkWidget::OnSegMousePress(QMouseEvent *pEvent)
{
	if (Qt::LeftButton == pEvent->button())
	{
		qDebug() << "MarkWidget mouse press event";
		SetTracking(true);
		//==================����������������ж�=====================
		int i_col = pEvent->pos().x();
		//ѡ����к�С��0
		//�кŴ��ڵ�ǰ�����������еĵ�ǰ֡���к� - ��ǰҳ��ʼ֡�к�
		//debug <2020/11/18> xiongz
		int temp = m_pSegData->GetCurCol() - m_pSegData->GetBegCol();
		if (i_col < 0 ||
			i_col > m_pSegData->GetCurCol() - m_pSegData->GetBegCol())
		{
			SetTracking(false);
			return;
		}
		
		m_iBegCol = pEvent->pos().x();
		m_pSegLayer->SetStartPos(pEvent->pos());

		//����pixmap
		m_pSegLayer->StoreLastPixmap();
	}

}
//ѡ��ģʽ�½���ѡ�β���
void CMarkWidget::MarkSeg(QMouseEvent* pEvent)
{
	QPoint point = pEvent->pos();
	qint64 i_col = point.x();
	if(i_col < 0)
	{
		point.setX(0);
	}
	else if(0 == m_pSegData->GetBegCol() && i_col > m_pSegData->GetCurCol() - m_pSegData->GetBegCol())
	{
		point.setX(m_pSegData->GetCurCol() - m_pSegData->GetBegCol());
	}
	else if (0 != m_pSegData->GetBegCol() && i_col > m_pSegData->GetCurCol() - m_pSegData->GetBegCol())
	{
		point.setX(m_pSegData->GetCurCol() - m_pSegData->GetBegCol());
	}
	
	m_pSegLayer->DrawUserTrack(point);
	update();
}
//ѡ��ģʽ������ѡ�οؼ������ڵ������״
void CMarkWidget::EnterEvent(QEvent* pEvent)
{
	//�����жϣ��Ƿ���ѡ��ģʽ
	if(m_bIsSegMode & IsLayerVisible())
	{
		setCursor(Qt::SplitHCursor);
	}
}
void CMarkWidget::LeaveEvent(QEvent *pEvent)
{
	setCursor(Qt::ArrowCursor);
}
//����/��ȡͼ��ɼ�״̬
void CMarkWidget::SetLayerVisible(const bool& bVisible)
{
	if(m_pSegLayer)
	{
		m_pSegLayer->SetVisible(bVisible);
	}
}
bool CMarkWidget::IsLayerVisible() const
{
	bool b_flag = false;
	if(m_pSegLayer)
	{
		b_flag = m_pSegLayer->IsVisible();
	}
	return b_flag;
}
void CMarkWidget::resizeEvent(QResizeEvent *event)
{
	//����pixmap��С
	if(m_pSegLayer->GetPixmap().size() != event->size())
	{
		m_pSegLayer->ResizeLayer(this->size());
	}
	//debug --xiongz--[2020/9/10]
	//m_pSegLayer->DrawBottom();

}
void CMarkWidget::SetAxisX(const CPlotXMap& cAxisX)
{
	//��������ʱ����
	qDebug() << "CMarkWidget SetAxisX";		//������Ϣ����ɾ��xiongz--<2020/9/11>
	//debug ���Ը���ʱ�������ʱ
	QDateTime date = QDateTime::currentDateTime();
	QString str_time = date.toString("yyyy-MM-dd hh:mm:ss.zzz");
	qDebug() << "set time map begin time:" << str_time;
	//m_cAxisX = cAxisX;
	//TODO:�޸�PlotSegmentData�����֡�ź͵�ǰҳ�泤��
	m_pSegData->SetTimeMap(cAxisX);
	qDebug() << "set time map end time:" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
	//����ʱ�����ѡ�λ���һ�����

	UpdatePlot();
	update();
}

void CMarkWidget::OnSegMouseRelease(QMouseEvent *pEvent)
{
	if (Qt::LeftButton == pEvent->button())
	{
		SetTracking(false);
		//if (pEvent->pos().x() == m_iBegCol)
		//{
		//	return;
		//}
		qDebug() << "MarkWidget mouse release event";
		//==================���ѡ�μ�¼=====================
		m_pSegLayer->SetEndPos(pEvent->pos());
		m_iEndCol = pEvent->pos().x();
		//==================�ж������յ��д�С��ϵ=====================
		if (m_iEndCol < m_iBegCol)
		{
			std::swap<qint64, qint64>(m_iBegCol, m_iEndCol);
		}
		//����ѡ�ε�ʵ��֡��
		int i_beg_col = m_pSegData->GetBegCol();
		m_iBegCol += i_beg_col;
		m_iEndCol += i_beg_col;
		if (m_iEndCol > m_pSegData->GetCurCol())
		{
			m_iEndCol = m_pSegData->GetCurCol();
		}
		m_pSegLayer->SetEndPos(pEvent->pos());
		//m_pSegData->AddSegment(m_iBegCol, m_iEndCol);
		//�����ṹ�� <2020/11/20> xiongz
		SegmentMarks &st_mark = m_stSegMarks;
		st_mark.iBegIndex = m_pSegData->GetTimeMap().InvTrans(m_iBegCol);
		st_mark.iEndIndex = m_pSegData->GetTimeMap().InvTrans(m_iEndCol);
		AppendSegMarks(st_mark);
		//�����˵�	 @xiongz [2020/11/4]
		if (m_pMenu)
		{
			m_pMenu->Popup(pEvent->globalPos());
		}
		//�����¼�
		emit SigSendSegment(st_mark.iBegIndex, st_mark.iEndIndex);
		UpdatePlot();
		update();
	}
}
void CMarkWidget::OnEventMousePress(QMouseEvent *pEvent)
{

}
void CMarkWidget::SlotMarkEvent(
	const E_EVENT_TYPE &eEventType,
	const QColor &colorBackground, 
	const QString &strDescription)
{
	//step1:��ȡ��ǰ���һ֡����,����ṹ��StampedEvent���󣬽���StEvent����¼��ǰ֡��
	StampedEvent st_event;
	st_event.eType = eEventType;
	st_event.iIndex = m_pSegData->GetCurIndex();
	st_event.strDescription = strDescription;
	m_pSegData->AppendEventMark(st_event);
	//����ͼ�����������ߣ���ɫΪlineColor
	UpdatePlot();
	update();
}
void CMarkWidget::SetCurrentSegBackground(const QColor &color)
{
	m_stSegMarks.eFlag = E_FLAG_P0;
	AppendSegMarks(m_stSegMarks);
	SaveSegmentMarks();
}
void CMarkWidget::SetCurrentLayout(const E_LAYOUT_TYPE &eLayout,
	const QMap<E_LAYOUT_TYPE, QVector<SegmentChannelParam>> & mapChannel)
{
	m_eCurrentLayout = eLayout;
	SegMeasureInfo &st_mearsure = m_stSegMarks.stMeasureInfo;
	st_mearsure.mapChannelMeasure = m_SegConfig.mapChannel;
	//������ǰ��ѡ����ÿһ��,���ǵ�ǰ���ֵ��������ò��ɼ�
	for (auto iter = mapChannel.begin();
		iter != mapChannel.end();
		++iter)
	{
		E_LAYOUT_TYPE e_layout = iter.key();
		if (e_layout == eLayout)
		{
			//���������µĵ�������ͬ�ĵ���
			for (int i = 0; i < iter->size(); ++i)
			{
				E_CHANNEL_TYPE  e_channel_type = iter->value(i).eChannelType;
				for (auto &iter_seg_channel : st_mearsure.mapChannelMeasure[e_layout])
				{
					if (e_channel_type == iter_seg_channel.eType)
					{
						iter_seg_channel.bIsVisible = iter->value(i).bSelected;
						iter_seg_channel.iIndex = iter->value(i).iIndex;
						for (int j = 0; j < iter->value(i).vctCurveItem.size(); ++j)
						{
							iter_seg_channel.vctCurveParam[j].color = iter->value(i).vctCurveItem.value(j).crCurve;
						}
						break;
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < iter->size(); ++i)
			{
				E_CHANNEL_TYPE  e_channel_type = iter->value(i).eChannelType;
				for (auto iter_seg_channel : st_mearsure.mapChannelMeasure[e_layout])
				{
					if (e_channel_type == iter_seg_channel.eType)
					{
						iter_seg_channel.bIsVisible = false;
						iter_seg_channel.iIndex = iter->value(i).iIndex;
						for (int j = 0; j < iter->value(i).vctCurveItem.size(); ++j)
						{
							iter_seg_channel.vctCurveParam[j].color = iter->value(i).vctCurveItem.value(j).crCurve;
						}
						break;
					}
				}
			}
		}
	}


}
void CMarkWidget::SetMeasureInfo(QMap<E_DATA_TYPE, MeasureInfo> &mapMeasureInfo)
{
	for (auto iter  = mapMeasureInfo.begin();
		iter !=mapMeasureInfo.end();
		++iter)
	{
		E_DATA_TYPE e_data_type = iter.key();
		MeasureInfo st_measureinfo = iter.value();
		for (auto &iter_marks : m_stSegMarks.stMeasureInfo.mapChannelMeasure)
		{
			for (auto &iter_channel : iter_marks)
			{
				for (auto &iter_curve : iter_channel.vctCurveParam)
				{
					if (e_data_type == iter_curve.eType)
					{
						iter_curve.stMeasureInfo = st_measureinfo;
					}
				}
			}
		}
	}
}
void CMarkWidget::SaveSegmentMarks()
{
	m_pSegData->SaveSegmentMarks();
}
//===================����ÿ�����ߵ�ͳ����Ϣ==================
void CMarkWidget::SlotSetMearsureInfo(const QMap<E_DATA_TYPE, MeasureInfo> &mapMearsureInfo)
{

}
void CMarkWidget::SetTracking(const bool &bTracking)
{
	if (bTracking == m_bIsTracking)
	{
		return;
	}
	m_bIsTracking = bTracking;
	m_pSegLayer->SetTracking(bTracking);
	emit SigSendSegTracking(bTracking);
}