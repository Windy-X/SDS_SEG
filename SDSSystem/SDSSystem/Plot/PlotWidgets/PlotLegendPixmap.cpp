#include "PlotLegendPixmap.h"
#include <QPainter>
#include <QDebug>
#include "Config.h"
CPlotLegendPixmap::CPlotLegendPixmap(const QSize &size)
{
	m_pix = QPixmap(size);
	m_pix.fill(Qt::transparent);
	m_vacant = m_pix;
}
CPlotLegendPixmap::~CPlotLegendPixmap()
{

}
void CPlotLegendPixmap::DrawLegends(const QVector<QString> &vctLegneds, const QVector<QColor> &vctColors)
{
	QPainter pp(&m_pix);
	QFont font_legend = CConfig::Instance()->GetLegendFont();
	QFontMetrics font_size(font_legend);
	int i_width_sum = 0;
	QVector<int> vct_size;
	QVector<QString> vct_legend_valid;
	for (int i = 0; i < vctLegneds.size(); ++i)
	{
		auto str_legends = vctLegneds.at(i);
		if (str_legends.isEmpty())
		{
			continue;
		}
		int i_width = font_size.width(str_legends);
		vct_size.append(i_width);
		vct_legend_valid.append(str_legends);
		i_width_sum += i_width;
	}
	if (vct_legend_valid.isEmpty())
	{
		return;
	}

	//将文本框分成n+1等分，从左到右写
	double i_space = double(m_pix.width()-i_width_sum) / (vct_size.size() + 1);
	int i_pos_y = (m_pix.height() + font_legend.pointSize()) / 2;
	qDebug() << "legend height = " << font_size.ascent();
	double i_pos_x = i_space;
	for (int i = 0; i < vct_legend_valid.size(); ++i)
	{
		pp.setPen(vctColors.value(i,Qt::black));
		pp.drawText(i_pos_x,i_pos_y,vct_legend_valid.at(i));
		i_pos_x += (vct_size.at(i) + i_space);
	}
}
