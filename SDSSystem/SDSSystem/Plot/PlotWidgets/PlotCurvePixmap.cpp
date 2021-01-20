#include "PlotCurvePixmap.h"
#include <QPainter>
#include <QDebug>
#include <QDateTime>
#include <QVector>
#include "Config.h"
 CPlotCurvePixmap::CPlotCurvePixmap()
{

}
 CPlotCurvePixmap::CPlotCurvePixmap(const QSize &size)
{
	InitPixmap(size);
}
 CPlotCurvePixmap::~CPlotCurvePixmap()
{

}
void CPlotCurvePixmap::InitPixmap(const QSize &size)
{
	m_pixChannel = QPixmap(size);
	m_pixChannel.fill(Qt::transparent);
	m_pixVacant = m_pixChannel;
}
void CPlotCurvePixmap::ResizePixmap(const QSize &size)
{
	m_pixChannel = m_pixVacant.scaled(size);
	m_pixVacant = m_pixChannel;
}
void CPlotCurvePixmap::UpdatePix(const LineCurveInfo &stCurveInfo,
								 const CPlotData &cData,
								 const CPlotXMap &cAxisX,
								 const CPlotYMap &cAxisLeft,
								 const CPlotYMap &cAxisRight)
{

		const CPlotYMap &c_axis_y = (E_AXIS_LEFT == stCurveInfo.eAxis) ? cAxisLeft : cAxisRight;
		DrawLinePix(stCurveInfo, cData,cAxisX,c_axis_y);
}
void CPlotCurvePixmap::DrawLinePix(const LineCurveInfo &stCurveInfo, const CPlotData &cData, const CPlotXMap &cAxisX, const CPlotYMap &cAxisY)
{
	//判断曲线是否显示，不显示就返回
	if (!stCurveInfo.bVisible)
	{
		qDebug() << stCurveInfo.strLegend << "doesn't show";
		return;
	}
	//绘制曲线，暂时不考虑无效值的情况，进行简化
	QPainter pp(&m_pixChannel);
	//进行反走样
	pp.setRenderHint(QPainter::Antialiasing,true);

	//假设当前的值都是有效值
	DrawLineWithoutInvalid(pp,stCurveInfo,cData,cAxisX,cAxisY);

}
//绘制线性曲线，不包括无效值
void CPlotCurvePixmap::DrawLineWithoutInvalid(  QPainter &painter,
												const LineCurveInfo &stLineCurveInfo,
												const CPlotData &cData,
												const CPlotXMap &cAxisX,
												const CPlotYMap &cAxiY)
{
	//获取显示数据
	QVector <double>vct_up = cData.GetDisplayUpEvn(cAxisX);
	QVector <double>vct_down = cData.GetDisplayDowmEvn(cAxisX);
	if (vct_down.isEmpty() || vct_up.isEmpty())
	{
		qDebug() << stLineCurveInfo.strLegend << "data empty";
		return;
	}
	QPainterPath path_up;
	QPainterPath path_down;
	int  i_pix = 0;
	for (; i_pix < vct_up.size() && i_pix < vct_down.size(); ++i_pix)
	{
		//将起点设置为第一个有效值
		if (C_INVALID_VALUE != vct_up.at(i_pix) && C_INVALID_VALUE != vct_down.at(i_pix))
		{
			QPoint point_up;
			point_up.setX(i_pix);
			point_up.setY(cAxiY.Trans(vct_down.at(i_pix)));
			path_up.moveTo(point_up);

			QPoint point_down;
			point_down.setX(i_pix);
			point_down.setY(cAxiY.Trans(vct_down.at(i_pix)));
			path_down.moveTo(point_down);

			break;
		}
	}
	for (; i_pix < vct_up.size() && i_pix < vct_down.size(); ++i_pix)
	{
		if (C_INVALID_VALUE == vct_up.at(i_pix) || C_INVALID_VALUE == vct_down.at(i_pix))
		{
			continue;
		}
		//将有效值添加到绘图路径
		//每相邻两行首尾相连
		QPoint point_up;
		point_up.setX(i_pix);
		point_up.setY(cAxiY.Trans(vct_up.at(i_pix)));
		path_up.lineTo(point_up);
		//连接最低点
		QPoint point_down;
		point_down.setX(i_pix);
		point_down.setY(cAxiY.Trans(vct_down.at(i_pix)));
		//path_down.lineTo(point_down);
		path_up.lineTo(point_down);

	}
	//debug输出绘图时间
	qint64 t1 = QDateTime::currentMSecsSinceEpoch();
	DrawValidLine(painter,stLineCurveInfo,path_up,path_down);
	qint64 t2 = QDateTime::currentMSecsSinceEpoch();
	qDebug() << "painterpath paint time:" << t2 - t1 << "ms";
	//TODO:进行回放时，获取前一页的有效数据，和当前当前页的第一个有效数据进行连接
	//TODO:进行回放时，将当前页最后一个有效数据和下一页的第一个有效数据进行连接


	
}
void CPlotCurvePixmap::DrawValidLine(QPainter &painter,	
									 const LineCurveInfo &stLineCurveInfo, 
									 const QPainterPath &pathUp, 
									 const QPainterPath &pathDown)
{
	if (!stLineCurveInfo.bVisible)
	{
		return;
	}
	if (pathUp.isEmpty() || pathDown.isEmpty())
	{
		//return;
	}
	E_DATA_TYPE e_type = stLineCurveInfo.CurveData;
	QPen pen;
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(stLineCurveInfo.iLineWidth);
	QColor cl_band = CConfig::Instance()->GetDataInfo(e_type).clLine;
	int i_alpha = CConfig::Instance()->GetCurveAlpha();
	pen.setColor(cl_band);
	painter.setPen(pen);

	QPainterPath path_total = pathUp;
	//绘制轨迹
	//path_total.connectPath(pathDown.toReversed());
	painter.drawPath(path_total);

}