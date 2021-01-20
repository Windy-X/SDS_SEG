#include "PlotLayerFactory.h"
CPlotLayerFactory::CPlotLayerFactory()
{

}
CPlotLayerFactory::~CPlotLayerFactory()
{

}
CPlotLayerBase *CPlotLayerFactory::CreatLayer(const E_LAYER_INDEX eLayer, const QSize &size)
{
	CPlotLayerBase *p_layer = NULL;
	switch (eLayer)
	{
	case E_LAYER_BOTTOM:
		p_layer = new CPlotLayerBottom();
		p_layer->InitLayer(size);
		break;
	case E_LAYER_CURVE:
		p_layer = new CPlotLayerCurve();
		p_layer->InitLayer(size);
		break;
	default:
		break;
	}
	return p_layer;
}