#include "Plot.h"
#include <QPainter>
#include <QDebug>
#include <QDateTime>
#include "PlotLayers/PlotLayerFactory.h"
#include "Config.h"

CPlot::CPlot(QWidget *parent)
	: QWidget(parent)
	,m_bIsSeg(false)
	,m_bIsEvent(false)
	,m_bIsSegTracking(false)
{
	//setMouseTracking(true);
}

CPlot::~CPlot()
{
}
void CPlot::InitPlot(const QSet<E_DATA_TYPE>& setDataType, const ChannelParam &stParam)
{

	//获取预留窗口的像素宽度
	m_iReservedWidth = CConfig::Instance()->GetReservedWidth();
	//构建无效矩形区域
	m_stParam = stParam;
	m_stParam.rectLeftLegend.setSize(QSize());
	m_stParam.rectPlot.setSize(QSize());
	m_stParam.rectReservedWidget.setSize(QSize()); //add <2020/9/27> xiongz
	m_stParam.cAxisLeft.SetDataRange(0,0);
	m_stParam.cAxisRight.SetDataRange(0,0);
	//设置X轴
	int i_legend_width = CConfig::Instance()->GetLegendWidth();//设置导联名称宽度
	int i_pix_width = this->width() - i_legend_width * 2;//绘图区的宽度(不包括指数区宽度)
	int i_data_width = CConfig::Instance()->GetPageDataLen(0);
	//TODO:指定比率
	//设置左边导联名称区宽度
	m_stParam.rectLeftLegend.setLeft(0);
	m_stParam.rectLeftLegend.setWidth(i_legend_width);
	//设置绘图区宽度
	m_stParam.rectPlot.setLeft(i_legend_width);
	m_stParam.rectPlot.setWidth(i_pix_width);
	//m_stParam.rectPlot.setLeft(0);
	//m_stParam.rectPlot.setWidth(this->width());
	//设置右边导联宽度
	m_stParam.rectRightLegend.setLeft(i_legend_width + i_pix_width);
	m_stParam.rectRightLegend.setWidth(i_legend_width);
	m_AxisX.Init(m_stParam.rectPlot.width(),i_data_width,0);
	
	emit SigAxisXChanged(m_AxisX);//debug
	
	//设置y轴
	int i_height = this->height();
	//设置绘图区高度
	m_stParam.rectPlot.setTop(0);
	m_stParam.rectPlot.setHeight(i_height);
	m_stParam.rectLeftLegend.setTop(0);

	//设置导联名称区高度
	m_stParam.rectLeftLegend.setHeight(i_height);
	m_stParam.rectRightLegend.setTop(0);
	m_stParam.rectRightLegend.setHeight(i_height);

	//设置预留窗口区域 add <2020/9/27> xiongz
	m_stParam.rectReservedWidget.setLeft(i_legend_width + i_pix_width - m_iReservedWidth);
	m_stParam.rectReservedWidget.setTop(0);
	m_stParam.rectReservedWidget.setWidth(m_iReservedWidth);
	m_stParam.rectReservedWidget.setHeight(i_height);
	//设置曲线数据
	CPlotData c_data;
	c_data.Init(i_pix_width);
	if(!setDataType.isEmpty())
	{
		for( auto it : setDataType)
		{
			m_mapData[it] = c_data;
		}
	}
	//创建图层
	CPlotLayerFactory layer_creator;
	for (E_LAYER_INDEX e_layer = E_LAYER_BOTTOM; e_layer <= E_LAYER_CURVE; e_layer = E_LAYER_INDEX(e_layer + 1))
	{
		QSize size_layer = GetLayerSize(e_layer);
		CPlotLayerBase *p_layer = layer_creator.CreatLayer(e_layer, size_layer);
		m_mapLayers[e_layer] = p_layer;
	}
	//初始化y轴
	if (m_stParam.stConfig.stPlotConfig.stLeftAxis.IsValid())
	{
		m_stParam.cAxisLeft.Init(
			m_stParam.stConfig.stPlotConfig.stLeftAxis.fLowBound,
			m_stParam.stConfig.stPlotConfig.stLeftAxis.fUpBound, 
			i_height, 
			0);
	}
	if (m_stParam.stConfig.stPlotConfig.stRightAxis.IsValid())
	{
		m_stParam.cAxisRight.Init(
			m_stParam.stConfig.stPlotConfig.stRightAxis.fLowBound,
			m_stParam.stConfig.stPlotConfig.stRightAxis.fUpBound, 
			i_height, 
			0);
	}
}
void CPlot::paintEvent(QPaintEvent *event)
{
	QPainter pp(this);
	for (E_LAYER_INDEX e_layer = E_LAYER_BOTTOM
		; e_layer <= E_LAYER_CURVE
		; e_layer = E_LAYER_INDEX(e_layer + 1))
	{
		if (!IsLayerVisible(e_layer))
		{
			continue;
		}
		const QPoint pt_layer = this->GetLayerTopLeft(e_layer);
		pp.drawPixmap(pt_layer,m_mapLayers.value(e_layer)->GetPixmap());
	}
	//pp.fillRect(m_stParam.rectPlot,QColor(255,0,0,125));
	//pp.fillRect(m_stParam.rectLeftLegend, QColor(0, 255, 0, 125));
	//pp.fillRect(m_stParam.rectRightLegend, QColor(0, 0, 255, 125));
}
void CPlot::UpdateLayerPix(const E_LAYER_INDEX &eLayer)
{
	CPlotLayerBase *p_layer = m_mapLayers.value(eLayer, NULL);
	if (NULL != p_layer)
	{
		switch (eLayer)
		{
		case E_LAYER_BOTTOM:
		{
			CPlotLayerBottom *p_bottom = dynamic_cast<CPlotLayerBottom *>(p_layer);
			if (NULL != p_bottom)
			{
				p_bottom->UpdatePix(m_stParam);
			}
			break;
		}
		case E_LAYER_CURVE:
		{
			CPlotLayerCurve *p_curve = dynamic_cast<CPlotLayerCurve *>(p_layer);
			if (NULL != p_curve)
			{
				p_curve->UpdatePix(m_stParam, m_mapData, m_AxisX);
			}
			break;
		}
		default:
			break;
		}
	}
}
void CPlot::UpdatePlot()
{
	for (E_LAYER_INDEX e_layer = E_LAYER_INDEX(E_LAYER_BOTTOM)
		; e_layer <= E_LAYER_CURVE
		; e_layer = E_LAYER_INDEX(e_layer + 1))
	{
		UpdateLayerPix(e_layer);
	}
	update();
}
void CPlot::SetLayerVisible(const E_LAYER_INDEX &eLayer, const bool bVisible)
{
	if (NULL == m_mapLayers.value(eLayer, NULL) || m_mapLayers.value(eLayer)->IsVisible() == bVisible)
	{
		return;
	}
	m_mapLayers.value(eLayer)->SetVisible(bVisible);
	update();
}
bool CPlot::IsLayerVisible(const E_LAYER_INDEX &eLayer)
{
	bool b_result = false;
	if (NULL != m_mapLayers.value(eLayer, NULL) && m_mapLayers.value(eLayer)->IsVisible())
	{
		b_result = true;
	}
	return b_result;
}
void CPlot::Clear()
{

	CPlotLayerBase *p_layer = NULL;
	for (E_LAYER_INDEX e_layer = E_LAYER_BOTTOM; e_layer <= E_LAYER_BOTTOM; E_LAYER_INDEX(e_layer + 1))
	{
		p_layer = m_mapLayers.value(e_layer,NULL);
		if (NULL == p_layer)
		{
			continue;
		}
		p_layer->Clear();
	}
	update();
}
void CPlot::mousePressEvent(QMouseEvent *event)
{
	qDebug() << "CPlot mouse press event";
}
 QSize CPlot::GetLayerSize(const E_LAYER_INDEX &eLayer) const
{
	QSize size_ret;
	switch (eLayer)
	{
	case E_LAYER_BOTTOM:
	{
		size_ret = this->size();
		break;
	}
	case E_LAYER_CURVE:
	{
		size_ret = m_stParam.rectPlot.size();
		break;
	}
	default:
		break;
	}
	return size_ret;
}
 QPoint CPlot::GetLayerTopLeft(const E_LAYER_INDEX &eLayer)const
{
	QPoint pt_rt(0,0);
	switch (eLayer)
	{
	case E_LAYER_BOTTOM:
	{
		pt_rt.setX(0);
		break;
	}
	case E_LAYER_CURVE:
	{
		pt_rt = m_stParam.rectPlot.topLeft();
		break;
	}
	default:
		break;
	}
	return pt_rt;
}
 void CPlot::AppendData(const E_DATA_TYPE &eType, const QVector<double> &vctData,const bool &bIsOverView)
 {
	 if (vctData.isEmpty())
		 return;
	 for (auto it : m_stParam.stConfig.stCurveConfig.vctLineCurves)
	 {
		 if (eType == it.CurveData)
		 {
			 break;
		 }
		 return;
	 }
	 int i_len = vctData.size();
	 bool b_change = false;
	//TODO:获取原始数据长度,调整X轴的压缩比率；
	 int i_data_len = i_len + m_mapData.value(eType).GetCurrentIndex();
	 //更新时间轴当前索引
	 m_AxisX.SetCurIndex(i_data_len);
	 //判断当前索引是否超过当前页面的最大索引，若是，则跳转页面，继续在当前页面绘制曲线 add <2020/10/12> xiongz
	 if (i_data_len > m_AxisX.GetEndIndex())
	 {
		 //跳转页面，页面的起点帧和终点帧号变化，设置页面的起点帧和终点帧，起点列和终点列 add <2020/10/12> xiongz
		 //全局模式下起点为0,时间轴宽度拓展
		 if(bIsOverView)
		 {
			 int i_pre_width = m_mapData[eType].GetCurTimeWidth();
			 double f_ratio_scale = m_mapData[eType].GetNextRatioScale();
			 double f_new_width = f_ratio_scale * i_pre_width;
			 m_mapData[eType].SetCurTimeWidth(ceil(f_new_width));
			 double f_ratio = double(m_mapData.value(eType).GetCurTimeWidth()) / m_mapData.value(eType).GetPixWidth();
			 m_mapData[eType].SetCurRatio(f_ratio);
			 int i_next_len = m_mapData.value(eType).GetCurTimeWidth();
			 m_AxisX.SetBegIndex(0);
			 m_AxisX.SetBegCol(0);
			 m_AxisX.SetPageDataLen(i_next_len);
			 
		 }
		 else
		 {
			 m_AxisX.SetBegCol(m_AxisX.GetBegCol() + m_iReservedWidth);
			 m_AxisX.SetBegIndex(m_AxisX.GetBegCol() * m_AxisX.GetRatio());
		 }

		 b_change = true;		
	 }
	 //添加数据
	 for(auto iter = m_mapData.begin(); iter != m_mapData.end(); ++iter)
	 {
		 if (eType == iter.key())
		 {
			 iter->AppendData(vctData,m_AxisX,b_change);
			 int i_ch_len = iter->GetCurrentIndex();
			 if (i_ch_len < i_data_len)
			 {
				 QVector<double> vct_data(i_data_len - i_ch_len, C_INVALID_VALUE);
				 iter->AppendData(vct_data,m_AxisX,b_change);
			 }
		 }
		 else 
		 {
			 //QVector<double> vct_data(i_len, C_INVALID_VALUE);
			 //iter->AppendData(vct_data);
			 continue;
		 }


	 }
	 
	 //更新绘图
	 //进行选段操作时页面不更新
	 if(!m_bIsSegTracking )
	 {
		 //选段时不更新选段控件时间轴
		 emit SigAxisXChanged(m_AxisX);
		 UpdateLayerPix(E_LAYER_CURVE);
		 update();
	 }
	
 }
 //设置选段
 void CPlot::SetSegMode(const bool &bSeg)
 {
	 if (bSeg != m_bIsSeg)
	 {
		 m_bIsSeg = bSeg;
	 }
	 if (!m_bIsSeg)
	 {
		 emit SigAxisXChanged(m_AxisX);
	 }
 }
 void CPlot::SetEventMode(const bool &bEvent)
 {
	 if (bEvent != m_bIsEvent)
	 {
		 m_bIsEvent = bEvent;
	 }
	 if (!m_bIsEvent)
	 {
		 emit SigAxisXChanged(m_AxisX);
	 }
 }
 void CPlot::SetSegTracking(const bool &bSegTracking)
 {
	 m_bIsSegTracking = bSegTracking;
 }