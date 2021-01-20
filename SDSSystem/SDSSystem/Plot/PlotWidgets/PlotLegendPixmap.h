#pragma once
#include <QPixmap>
class CPlotLegendPixmap
{
public:
	CPlotLegendPixmap(const QSize &size);
	~CPlotLegendPixmap();
	const QPixmap &GetPixmap() const { return m_pix; }
	void DrawLegends(const QVector<QString> &vctLegneds, const QVector<QColor> &vctColors);
private:
	QPixmap m_pix;
	QPixmap m_vacant;
};

