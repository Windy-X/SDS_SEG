#include "PlotLayerSeg.h"
#include <QPainter>
#include <QTextCodec>
#include <QDebug>
#include "PublicDefs.h"
CPlotLayerSeg::CPlotLayerSeg()
	:m_iStartIndex(-1)
	,m_iEndIndex(-1)
	,m_bTracking(false)
	,m_bIsEventMode(false)
{

}
CPlotLayerSeg::~CPlotLayerSeg()
{

}
void CPlotLayerSeg::SetStartPos(const int &iStartPos)
{
	m_iStartIndex = iStartPos;
}
void CPlotLayerSeg::SetStartPos(const QPoint &point)
{
	m_iStartIndex = point.x();
}
void CPlotLayerSeg::SetEndPos(const int &iEndPos)
{
	m_iEndIndex = iEndPos;
}
void CPlotLayerSeg::SetEndPos(const QPoint &point)
{
	m_iEndIndex = point.x();
}
//用户选段
void CPlotLayerSeg::DrawUserTrack(const QPoint& point)
{
	m_Pix = m_lastPix;
	if (point.x() == m_iStartIndex)
	{
		return;
	}
	
	QPainter pp(&m_Pix);
	pp.setPen(Qt::NoPen);
	pp.setBrush(C_COLOR_SEG);
	QRect rct;
	rct.setTopLeft(QPoint(m_iStartIndex, 0));
	rct.setBottomRight(QPoint(point.x(), m_Pix.height() - 1));
	pp.drawRect(rct);
}
//更新图层
void CPlotLayerSeg::UpdatePix(const QVector<SegmentMarks>& vctSegment,const SegmentView &stView,const CPlotXMap &cAxisX)
{
	//判断参数
	if(vctSegment.empty())
	{
		return;
	}
	//遍历序列，绘制图层
	m_Pix = m_Vacant;
	QPainter pp(&m_Pix);
	pp.setPen(Qt::NoPen);
	pp.setBrush(C_COLOR_SEG);
	int i_width = stView.iWidth;
	for(QVector<SegmentMarks>::const_iterator it = vctSegment.begin();
		it != vctSegment.end();
		++ it)
	{
		//参数判断
		//当前页起点列号
		qint64 i_beg = cAxisX.Trans(it->iBegIndex) - cAxisX.GetBegCol();
		qint64 i_end = cAxisX.Trans(it->iEndIndex) - cAxisX.GetBegCol();
		if(i_beg < 0 || i_end >= m_Pix.width())
		{
			continue;
		}
		QColor color(it->stView.colorBackground);
		color.setAlpha(it->stView.iAlpha);
		pp.setBrush(color);
		pp.setPen(Qt::NoPen);
		QRect rct;
		rct.setTopLeft(QPoint(i_beg, 0));
		rct.setBottomRight(QPoint(i_end, m_Pix.height() - 1));
		pp.drawRect(rct);
		//绘制统计信息
		int i_begin_col = cAxisX.Trans(it->iBegIndex) - cAxisX.GetBegCol();
		int i_end_col = cAxisX.Trans(it->iEndIndex) - cAxisX.GetBegCol();
		//统计信息绘制区域，左对齐，当超出pix时右对齐
		QRect rect_seg;
		int i_rect_left = i_begin_col + i_width <= cAxisX.GetPixWidth() ? i_begin_col : i_end_col - i_width;
		rect_seg.setTop(0);
		rect_seg.setLeft(i_rect_left);
		rect_seg.setSize(QSize(i_width, m_Pix.height()));
		DrawSegmentMeasureInfoTitle(&pp, stView.fontTitle, stView.colorTitle, it->stMeasureInfo.iPulses, it->stMeasureInfo.iTime, rect_seg);
		//计算每导区域
		double f_sum = 0.0f;
		for(auto iter = (it->stMeasureInfo.mapChannelMeasure).begin();
			iter != (it->stMeasureInfo.mapChannelMeasure).end();
			++ iter)
			{
				for (auto iter_channel : iter.value())
				{
					if (iter_channel.bIsVisible)
					{
						f_sum += iter_channel.fStrechFactor;
					}
				}
			}
		double f_unit_height = m_Pix.height()/f_sum;
		double f_current_y = 0.0f;
		for (auto iter = it->stMeasureInfo.mapChannelMeasure.begin();
			iter != it->stMeasureInfo.mapChannelMeasure.end();
			++iter)
		{
			for (auto iter_channel : iter.value())
			{
				if (true == iter_channel.bIsVisible)
				{
					int i_y = f_current_y;
					double f_height = f_unit_height * iter_channel.fStrechFactor;

					QRect rct_channel;
					rct_channel.setTop(i_y);
					rct_channel.setLeft(i_rect_left);
					rct_channel.setSize(QSize(i_width, f_height));
					f_current_y += f_height;
					//绘制菜单
					DrawSegmentMeasureInfo(&pp, iter_channel, stView.fontCurveIndex, rct_channel);
				}
			}
			
		}
	}
	//记录最后状态
	StoreLastPixmap();
	
}
void CPlotLayerSeg::DrawBottom()
{
	QPainter pp(&m_Pix);
	pp.setPen(Qt::NoPen);
	pp.setBrush(Qt::NoBrush);
	pp.fillRect(QRect(0,0,m_Pix.width() - 1,m_Pix.height() - 1),QColor(255, 0, 0, 125));
}

//记录最后状态
void CPlotLayerSeg::StoreLastPixmap()
{
	m_lastPix = m_Pix;
}
void CPlotLayerSeg::RestoreLastPixmap()
{
	m_Pix = m_lastPix;
}
void CPlotLayerSeg::SetTracking(bool bIsTracking)
{
	if(m_bTracking == bIsTracking)
	{
		return;
	}
	m_bTracking = bIsTracking;
	if(m_bTracking)
	{
		m_lastPix = m_Pix;
	}
}
void CPlotLayerSeg::SetMarkEvent(const bool &bIsEventMode)
{
	if(bIsEventMode == m_bIsEventMode)
	{
		return;
	}
	m_bIsEventMode = bIsEventMode;
}
void CPlotLayerSeg::DrawMarkEventLine(const QVector<StampedEvent> &vctMark,const CPlotXMap &cAxisX)
{
	//判断参数
	if(vctMark.empty())
	{
		return;
	}
	//重置图层
	m_Pix = m_Vacant;
	QPainter pp(&m_Pix);
	QFont font;
	QPen pen(Qt::DotLine);
	pen.setWidth(3);
	font.setPixelSize(20);
	for(QVector<StampedEvent>::const_iterator it = vctMark.begin();
		it != vctMark.end();
		++ it)
	{
		//设置画笔
		pen.setColor(it->lineColor);
		pp.setPen(pen);
		//设置字体
		font.setFamily("arial");
		pp.setFont(font);
		//计算标记线坐标
		qint64 i_beg = cAxisX.Trans(it->iIndex) - cAxisX.GetBegCol();
		if (i_beg < 0 || i_beg >= m_Pix.width())
		{
			continue;
		}
		QLine line;
		line.setP1(QPoint(i_beg, 0));
		line.setP2(QPoint(i_beg,m_Pix.height() - 1));
		pp.drawLine(line);
		//绘制文字,设置字体
		pp.drawText(QPoint(i_beg, m_Pix.height() / 2), it->strDescription);
	}
}
void CPlotLayerSeg::DrawSegmentMeasureInfo(
	QPainter * pp,
	const SegmentChannelMeasure &stChannel,
	const QFont &font,
	const QRect &rectChannel)
{
	//检查参数
	Q_ASSERT(!rectChannel.isEmpty());
	//m_Pix = m_lastPix;
	//QPainter pp(&m_Pix);
	pp->setFont(font);
	QPen pen(Qt::SolidLine);
	QVector<SegmentCurveParam> vct_curve_info = stChannel.vctCurveParam;
	MeasureInfo st_measure = stChannel.vctCurveParam.front().stMeasureInfo;
	QPoint point_top_left = rectChannel.topLeft();
	//绘制每一条曲线的统计信息
	int i_gain_heigh = rectChannel.height() / stChannel.iCount;
	int i_gain_width = rectChannel.width() / 4;
	for (auto it : vct_curve_info)
	{
		//设置每一条曲线的区域
		QRect rct_min;
		QRect rct_max;
		QRect rct_mean;
		QRect rct_max_min;

		rct_mean.setLeft(point_top_left.x());
		rct_mean.setTop(point_top_left.y() + it.iIndex * i_gain_heigh);
		rct_mean.setSize(QSize(i_gain_width,i_gain_heigh));
		
		

		rct_max.setLeft(point_top_left.x() + i_gain_width);
		rct_max.setTop(point_top_left.y() + it.iIndex * i_gain_heigh);
		rct_max.setSize(QSize(i_gain_width, i_gain_heigh));


		rct_min.setLeft(point_top_left.x() + 2 * i_gain_width);
		rct_min.setTop(point_top_left.y() + it.iIndex * i_gain_heigh);
		rct_min.setSize(QSize(i_gain_width, i_gain_heigh));

		rct_max_min.setLeft(point_top_left.x() + 3 * i_gain_width);
		rct_max_min.setTop(point_top_left.y() + it.iIndex * i_gain_heigh);
		rct_max_min.setSize(QSize(i_gain_width, i_gain_heigh));

		pen.setColor(it.color);
		pp->setPen(pen);
		pp->drawText(rct_mean, it.stMeasureInfo.strMean, Qt::AlignBottom | Qt::AlignLeft);
		pp->drawText(rct_max,it.stMeasureInfo.strMax,Qt::AlignBottom | Qt::AlignLeft);
		pp->drawText(rct_min, it.stMeasureInfo.strMin, Qt::AlignBottom | Qt::AlignLeft);
		int i_max_div_min = 100 * it.stMeasureInfo.strMax_Min.toFloat();
		if(i_max_div_min != 0)
		{ 
			QString str_max_div_min = QString("%1%").arg(i_max_div_min);
			pp->drawText(rct_max_min, str_max_div_min, Qt::AlignBottom | Qt::AlignLeft);
		}
			

	}
	//保存最后的pixmap
	//StoreLastPixmap();
}
void CPlotLayerSeg::DrawSegmentMeasureInfoTitle(QPainter * pp,
	const QFont &font,
	const QColor &color,
	const int &iPulses,
	int iTime,
	const QRect &rectSeg)
{
	//RestoreLastPixmap();
	//QPainter pp(&m_Pix);
	QPen pen(Qt::SolidLine);
	pen.setColor(color);
	pp->setFont(font);
	int i_gain_width = rectSeg.width() / 4;
	int i_gain_heigh = rectSeg.height() / 2;
	QPoint point_top_left = rectSeg.topLeft();
	QString strTmp;

	QRect rct_min;
	QRect rct_max;
	QRect rct_mean;
	QRect rct_max_min;
	QRect rct_pulse;
	QRect rct_time;

	rct_mean.setLeft(point_top_left.x());
	rct_mean.setTop(0);
	rct_mean.setSize(QSize(i_gain_width, i_gain_heigh));



	rct_max.setLeft(point_top_left.x() + i_gain_width);
	rct_max.setTop(0);
	rct_max.setSize(QSize(i_gain_width, i_gain_heigh));


	rct_min.setLeft(point_top_left.x() + 2 * i_gain_width);
	rct_min.setTop(0);
	rct_min.setSize(QSize(i_gain_width, i_gain_heigh));

	rct_max_min.setLeft(point_top_left.x() + 3 * i_gain_width);
	rct_max_min.setTop(0);
	rct_max_min.setSize(QSize(i_gain_width, i_gain_heigh));

	rct_pulse.setLeft(point_top_left.x());
	rct_pulse.setTop(point_top_left.y() +  i_gain_heigh);
	rct_pulse.setSize(QSize(2 * i_gain_width,i_gain_heigh));

	rct_time.setLeft(point_top_left.x() + 2 * i_gain_width);
	rct_time.setTop(point_top_left.y() +  i_gain_heigh);
	rct_time.setSize(QSize(2 * i_gain_width, i_gain_heigh));

	pen.setColor(color);
	pp->setPen(pen);
	pp->drawText(rct_mean, "Mean", Qt::AlignTop | Qt::AlignLeft);
	pp->drawText(rct_max, "Max", Qt::AlignTop | Qt::AlignLeft);
	pp->drawText(rct_min, "Min", Qt::AlignTop | Qt::AlignLeft);
	pp->drawText(rct_max_min, "Max/Min", Qt::AlignTop | Qt::AlignLeft);
	//转码unicode;
	QTextCodec * p_codec = QTextCodec::codecForLocale();
	QTextCodec::setCodecForLocale(p_codec);
	strTmp = QString("Pulses:%1").arg(iPulses);
	pp->drawText(rct_pulse, strTmp, Qt::AlignLeft | Qt::AlignBottom);
	strTmp = QString::fromLocal8Bit("Δt=%1").arg(iTime);
	pp->drawText(rct_time, strTmp, Qt::AlignLeft | Qt::AlignBottom);
	
}
void CPlotLayerSeg::UpdateMeasureInfoPix(const QVector<SegmentMarks>& vctSegment, const SegmentConfig &stSegConfig,const CPlotXMap &cAxisX)
{
	//参数判断
	//Q_ASSERT(!vctSegment.isEmpty());
	////m_Pix = m_lastPix;
	//QPainter pp(&m_Pix);
	//int i_width = stSegConfig.stView.iWidth;
	//for (auto it : vctSegment)
	//{
	//	int i_begin_col = cAxisX.Trans(it.iBegIndex) - cAxisX.GetBegCol();
	//	int i_end_col = cAxisX.Trans(it.iEndIndex) - cAxisX.GetBegCol();
	//	//统计信息绘制区域，左对齐，当超出pix时右对齐
	//	QRect rect_seg;
	//	int i_rect_left = i_begin_col + i_width <= cAxisX.GetPixWidth() ? i_begin_col : i_end_col - i_width;
	//	rect_seg.setTop(0);
	//	rect_seg.setLeft(i_rect_left);
	//	rect_seg.setSize(QSize(i_width,m_Pix.height()));
	//	DrawSegmentMeasureInfoTitle(&pp,stSegConfig.fontTitle,stSegConfig.colorTitle,it.stMeasureInfo.iPulses,it.stMeasureInfo.iTime,rect_seg);
	//	//计算每导区域
	//	double f_sum = 0.0f;
	//	for (int i = 0; i<it.stMeasureInfo.vctChannel.size(); ++i)
	//	{
	//		f_sum += it.stMeasureInfo.vctChannel[i].fStrechFactor;
	//	}
	//	double f_unit_height = m_Pix.height();
	//	double f_current_y = 0.0f;
	//	for (int i = 0; i < it.stMeasureInfo.vctChannel.size(); ++i)
	//	{
	//		SegMeasureInfo st_measure_info = it.stMeasureInfo;
	//		SegmentChannelMeasure st_channel = it.stMeasureInfo.vctChannel[i];
	//		int i_y = f_current_y;
	//		double f_height = f_unit_height * it.stMeasureInfo.vctChannel[i].fStrechFactor;
	//		
	//		QRect rct_channel;
	//		rct_channel.setTop(i_y);
	//		rct_channel.setLeft(i_rect_left);
	//		rct_channel.setSize(QSize(i_width,f_height));
	//		//绘制菜单
	//		DrawSegmentMeasureInfo(&pp,st_channel, stSegConfig.fontCurveIndex, rct_channel);
	//	}
	//	
	//}
	//StoreLastPixmap();
}