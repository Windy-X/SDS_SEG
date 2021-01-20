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
	//�ж������Ƿ���ʾ������ʾ�ͷ���
	if (!stCurveInfo.bVisible)
	{
		qDebug() << stCurveInfo.strLegend << "doesn't show";
		return;
	}
	//�������ߣ���ʱ��������Чֵ����������м�
	QPainter pp(&m_pixChannel);
	//���з�����
	pp.setRenderHint(QPainter::Antialiasing,true);

	//���赱ǰ��ֵ������Чֵ
	DrawLineWithoutInvalid(pp,stCurveInfo,cData,cAxisX,cAxisY);

}
//�����������ߣ���������Чֵ
void CPlotCurvePixmap::DrawLineWithoutInvalid(  QPainter &painter,
												const LineCurveInfo &stLineCurveInfo,
												const CPlotData &cData,
												const CPlotXMap &cAxisX,
												const CPlotYMap &cAxiY)
{
	//��ȡ��ʾ����
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
		//���������Ϊ��һ����Чֵ
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
		//����Чֵ��ӵ���ͼ·��
		//ÿ����������β����
		QPoint point_up;
		point_up.setX(i_pix);
		point_up.setY(cAxiY.Trans(vct_up.at(i_pix)));
		path_up.lineTo(point_up);
		//������͵�
		QPoint point_down;
		point_down.setX(i_pix);
		point_down.setY(cAxiY.Trans(vct_down.at(i_pix)));
		//path_down.lineTo(point_down);
		path_up.lineTo(point_down);

	}
	//debug�����ͼʱ��
	qint64 t1 = QDateTime::currentMSecsSinceEpoch();
	DrawValidLine(painter,stLineCurveInfo,path_up,path_down);
	qint64 t2 = QDateTime::currentMSecsSinceEpoch();
	qDebug() << "painterpath paint time:" << t2 - t1 << "ms";
	//TODO:���лط�ʱ����ȡǰһҳ����Ч���ݣ��͵�ǰ��ǰҳ�ĵ�һ����Ч���ݽ�������
	//TODO:���лط�ʱ������ǰҳ���һ����Ч���ݺ���һҳ�ĵ�һ����Ч���ݽ�������


	
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
	//���ƹ켣
	//path_total.connectPath(pathDown.toReversed());
	painter.drawPath(path_total);

}