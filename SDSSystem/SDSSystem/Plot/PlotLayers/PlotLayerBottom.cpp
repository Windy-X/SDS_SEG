#include "PlotLayerBottom.h"
#include <QPainter>
#include "Config.h"
#include "PlotWidgets/PlotLegendPixmap.h"
CPlotLayerBottom::CPlotLayerBottom()
{

}
CPlotLayerBottom::~CPlotLayerBottom()
{

}
void CPlotLayerBottom::UpdatePix(const ChannelParam &stParam)
{
	m_Pix = m_Vacant;
	DrawChannelBackground(stParam);
	DrawChannelGrid(stParam);

	DrawChannelLegends(stParam);
	DrawChannelScale(stParam);
	DrawResevedWidget(stParam);
}
//绘制底色
void CPlotLayerBottom::DrawChannelBackground(const ChannelParam &stParam)
{
	QPainter pp(&m_Pix);
	pp.setPen(Qt::NoPen);
	pp.setBrush(CConfig::Instance()->GetBackgroundColor());
	pp.drawRect(stParam.rectPlot);
}
//绘制网格
void CPlotLayerBottom::DrawChannelGrid(const ChannelParam& stParam)
{
	QPainter pp(&m_Pix);
	QPen pen;
	pen.setStyle(Qt::SolidLine);
	pen.setColor(CConfig::Instance()->GetGridColor());
	pen.setWidth(1);
	pp.setPen(pen);
	const QRect & rect_plot = stParam.rectPlot;
	//绘制纵向网格
	double f_step = double(rect_plot.width()-1) / CConfig::Instance()->GetVerticalGrid();
	for (double i = rect_plot.left(); i <= rect_plot.right(); i += f_step)
	{
		pp.drawLine(QPoint(i,rect_plot.top()),QPoint(i,rect_plot.bottom()));
	}
	//绘制右侧网格线
	pp.drawLine(rect_plot.topRight(), rect_plot.bottomRight());
	//绘制横向网格
	f_step = double(rect_plot.height()-1) / stParam.stConfig.stPlotConfig.iScaleDiv;
	for (int i = 0; i < stParam.stConfig.stPlotConfig.iScaleDiv; ++i)
	{
		int i_y = f_step * i;
		QPoint point_beg;
		point_beg.setX(rect_plot.left());
		point_beg.setY(i_y + rect_plot.top());
		QPoint point_end;
		point_end.setX(rect_plot.right());
		point_end.setY(i_y + rect_plot.top());
		pp.drawLine(point_beg, point_end);
	}
	//绘制底部网格线
	pp.drawLine(rect_plot.bottomLeft(),rect_plot.bottomRight());
}
//绘制坐标轴刻度值
void CPlotLayerBottom::DrawChannelScale(const ChannelParam& stParam)
{
	QPainter pp(&m_Pix);
	pp.setPen(Qt::black);
	QFont font_tick = CConfig::Instance()->GetScaleFont();
	QFontMetrics fontsize(font_tick);
	QString str_lable;
	int i_pos_x = 0;
	int i_pos_y = 0;
	int i_scale_margin_h = CConfig::Instance()->GetScaleHMargin();
	int i_scale_margin_v = CConfig::Instance()->GetScaleVMargin();
	//绘图区域
	const QRect &rect_plot = stParam.rectPlot;
	//刻度颜色
	const QColor cl_back = CConfig::Instance()->GetBackgroundColor();

	//参数判断
	//左侧纵轴
	if (stParam.cAxisLeft.IsValid())
	{
		//绘制上边界刻度
		double f_up_value = stParam.stConfig.stPlotConfig.stLeftAxis.fUpBound;
		if (abs(f_up_value) <= C_DOUBLE_ZERO)
		{
			f_up_value = 0.0;
		}
		str_lable.setNum(f_up_value,'f',0);
		i_pos_x = rect_plot.left() + i_scale_margin_h;
		i_pos_y = rect_plot.top() + font_tick.pointSize() + i_scale_margin_v;
		pp.setPen(CConfig::Instance()->GetForegroundColor());
		pp.setFont(font_tick);
		pp.drawText(i_pos_x,i_pos_y,str_lable);
		//绘制下边界刻度
		double f_down_value = stParam.stConfig.stPlotConfig.stLeftAxis.fLowBound;
		if (abs(f_down_value) <= C_DOUBLE_ZERO)
		{
			f_down_value = 0.0;
		}
		str_lable.setNum(f_down_value,'f',0);
		i_pos_x = rect_plot.left() + i_scale_margin_h;
		i_pos_y = rect_plot.bottom() - i_scale_margin_v;
		pp.drawText(i_pos_x,i_pos_y,str_lable);

	}
	if (stParam.cAxisRight.IsValid())
	{
		//绘制上边界刻度
		double f_up_value = stParam.stConfig.stPlotConfig.stRightAxis.fUpBound;
		if (abs(f_up_value) <= C_DOUBLE_ZERO)
		{
			f_up_value = 0.0;
		}
		str_lable.setNum(f_up_value,'f',0);
		i_pos_x = rect_plot.right() - i_scale_margin_h - fontsize.width(str_lable);
		i_pos_y = rect_plot.top() + font_tick.pointSize() + i_scale_margin_v;
		pp.setPen(CConfig::Instance()->GetBackgroundColor());
		pp.setPen(Qt::red);
		pp.setFont(font_tick);
		pp.drawText(i_pos_x, i_pos_y, str_lable);
		//绘制下边界刻度
		double f_down_value = stParam.stConfig.stPlotConfig.stRightAxis.fLowBound;
		if (abs(f_down_value) <= C_DOUBLE_ZERO)
		{
			f_down_value = 0.0;
		}
		str_lable.setNum(f_down_value, 'f', 0);
		i_pos_x = rect_plot.right() - i_scale_margin_h - fontsize.width(str_lable);
		i_pos_y = rect_plot.bottom() - i_scale_margin_v;
		pp.drawText(i_pos_x, i_pos_y, str_lable);
	}
}
void CPlotLayerBottom::DrawChannelLegends(const ChannelParam &stParam)
{
	QVector<QString> vct_left_names;
	QVector<QColor> vct_left_colors;
	QVector<QString> vct_right_names;
	QVector<QColor> vct_right_colors;
	for (int i = 0; i < stParam.stConfig.stCurveConfig.vctLineCurves.size(); ++i)
	{
		auto curveinfo = stParam.stConfig.stCurveConfig.vctLineCurves.at(i);
		auto st_data = CConfig::Instance()->GetDataInfo(curveinfo.CurveData);
		if (curveinfo.bVisible && !curveinfo.strLegend.isEmpty())
		{
			if (E_AXIS_LEFT == curveinfo.eAxis)
			{
				vct_left_colors.append(st_data.clLine);
				vct_left_names.append(curveinfo.strLegend);
			}
			else
			{
				vct_right_colors.append(st_data.clLine);
				vct_right_names.append(curveinfo.strLegend);
			}
		}
	}
	//绘制曲线名称
	QPainter pp(&m_Pix);
	//绘制左侧曲线名称
	const QRect &rect_left = stParam.rectLeftLegend;//曲线名称位置待定
	CPlotLegendPixmap pix_left(QSize(rect_left.height(),rect_left.width()));//定义定义曲线名称pixmap
	pix_left.DrawLegends(vct_left_names,vct_left_colors);
	//对pixmap进行坐标变换，将原点修改为rect左下角顶点，并且逆时针方向旋转90度
	pp.translate(rect_left.bottomLeft());//pixmap重置坐标原点,曲线名称区域左下角定点
	pp.rotate(-90);
	pp.drawPixmap(QPoint(0,0),pix_left.GetPixmap());
	pp.resetTransform();

	//绘制右侧的曲线名称
	const QRect &rect_right = stParam.rectRightLegend;//曲线名称区域
	CPlotLegendPixmap pix_right(QSize(rect_right.height(), rect_right.width()));//定义曲线名称（右侧）
	pix_right.DrawLegends(vct_right_names,vct_right_colors);
	//进行坐标变换，将原点修改为右上角，在将整个pixmap顺时针旋转90度
	pp.translate(rect_right.topRight());
	pp.rotate(90);
	pp.drawPixmap(QPoint(0,0),pix_right.GetPixmap());
	pp.resetTransform();
}
void CPlotLayerBottom::DrawResevedWidget(const ChannelParam &stParam)
{
	QPainter pp(&m_Pix);
	QPen pen;
	pen.setStyle(Qt::DashLine);
	pen.setColor(Qt::red);
	pp.setPen(pen);
	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	brush.setColor(QColor(0,0,0,122));
	pp.setBrush(brush);
	pp.drawRect(stParam.rectReservedWidget);
	//绘制底部边界线
	pp.drawLine(stParam.rectReservedWidget.bottomLeft().x(),
		stParam.rectReservedWidget.bottomLeft().y()-1,
		stParam.rectReservedWidget.bottomRight().x(),
		stParam.rectReservedWidget.bottomRight().y() - 1);

}