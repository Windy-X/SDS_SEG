#include "PlotLayerCurve.h"
#include <QPainter>
#include <QDebug>
#include <QDateTime>
#include "PlotWidgets/PlotCurvePixmap.h"
CPlotLayerCurve::CPlotLayerCurve()
{

}
CPlotLayerCurve::~CPlotLayerCurve()
{

}
void CPlotLayerCurve::UpdatePix(const ChannelParam &stParam, const QMap<E_DATA_TYPE, CPlotData> &mapPlot, const CPlotXMap &cAxisX)
{
	//清空原先的绘图
	m_Pix = m_Vacant;
	QPainter pp(&m_Pix);
	//绘图区域
	const QRect &rect_curve = stParam.rectPlot;
	const QVector<LineCurveInfo> &vct_curve = stParam.stConfig.stCurveConfig.vctLineCurves;
	if (!stParam.stConfig.bVisible)
	{
		return;
	}

	for (int i = 0; i < vct_curve.size(); ++i)
	{
		const LineCurveInfo &st_line = stParam.stConfig.stCurveConfig.vctLineCurves.value(i);
		if (mapPlot.contains(st_line.CurveData))
		{
			const CPlotData &c_line_data = mapPlot.value(st_line.CurveData);
			CPlotCurvePixmap pix_curve(rect_curve.size());

			//debug输出绘图时间
			//qint64 t1 = QDateTime::currentMSecsSinceEpoch();
			pix_curve.UpdatePix(st_line, c_line_data, cAxisX, stParam.cAxisLeft, stParam.cAxisRight);
			//qint64 t2 = QDateTime::currentMSecsSinceEpoch();
			//qDebug() << "curve paint time:" << t2 - t1 << "ms";
			pp.drawPixmap(QPoint(0,0),pix_curve.GetPixmap());

		}
	}
		
}
