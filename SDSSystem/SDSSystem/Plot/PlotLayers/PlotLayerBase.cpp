#include "PlotLayerBase.h"
CPlotLayerBase::CPlotLayerBase()
{
	m_bVisible = true;
}
CPlotLayerBase::~CPlotLayerBase()
{

}
void CPlotLayerBase::InitLayer(const QSize &size)
{
	m_Pix = QPixmap(size);
	m_Pix.fill(Qt::transparent);
	m_Vacant = m_Pix;
	m_bVisible = true;
}
void CPlotLayerBase::InitLayer(const QSize &size, const QColor &clBackground)
{
	m_Pix = QPixmap(size);
	m_Pix.fill(clBackground);
	m_Vacant = m_Pix;
	m_bVisible = true;
}
void CPlotLayerBase::Clear()
{
	m_Pix = m_Vacant;
}
void CPlotLayerBase::ResizeLayer(const QSize &size)
{
	m_Pix = m_Vacant.scaled(size);
	m_Vacant = m_Pix;
}