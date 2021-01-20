#include "SegMenuItem.h"
#include <QPainter>
SegMenuItem::SegMenuItem(QWidget *parent)
	: QWidget(parent)
{
	//setAutoFillBackground(true);
	//开启鼠标悬浮事件
	setAttribute(Qt::WA_Hover, true);
}
SegMenuItem::SegMenuItem(const QString &strTitle,QWidget *parent /* = NULL */)
	:QWidget(parent)
	,m_strCaption(strTitle)
{
	//开启鼠标悬浮事件
	setAttribute(Qt::WA_Hover, true);
}
SegMenuItem::~SegMenuItem()
{
}
void SegMenuItem::InitItem(const QSize &size)
{
	if(size.isNull())
	{
		return;
	}
	resize(size);
}
bool SegMenuItem::event(QEvent *e)
{
	//鼠标
	if(QEvent::HoverEnter == e->type())
	{
		m_stItemCfg.colorBorder = C_COLOR_HOVER;
		m_stItemCfg.iWidth = C_HOVER_BORDER_WIDTH;
		m_stItemCfg.font.setBold(true);
		//鼠标悬浮设置焦点
		this->setFocus();
		update();
		return true;

	}
	if(QEvent::HoverLeave == e->type())
	{
		m_stItemCfg.colorBorder = C_COLOR_UNHOVER;
		m_stItemCfg.iWidth = C_UNHOVER_BORDER_WIDTH;
		m_stItemCfg.font.setBold(false);
		//鼠标离开widget失去焦点
		this->clearFocus();
		update();
		return true;
	}
	if(QEvent::KeyPress == e->type())
	{
		QKeyEvent *p_key_event = static_cast<QKeyEvent *>(e);
		if(Qt::Key_Enter == p_key_event->key()|| Qt::Key_Return == p_key_event->key())
		{
			emit Sigtriggered(m_colorBackground);
			return true;
		}
	}
	return QWidget::event(e);
}
void SegMenuItem::SetBackground(const QColor &color)
{
	m_colorBackground = color;
	m_stItemCfg.colorBackground = color;
}
void SegMenuItem::SetText(const QString &strCaption)
{
	m_strCaption = strCaption;
}
void SegMenuItem::paintEvent(QPaintEvent *ev)
{
	QPainter pp(this);
	DrawText(&pp);
}
void SegMenuItem::DrawText(QPainter *pp)
{
	QPen pen;
	pen.setWidth(m_stItemCfg.iWidth);
	pen.setColor(m_stItemCfg.colorFont);
	pp->setFont(m_stItemCfg.font);
	pp->drawText(rect(), Qt::AlignHCenter | Qt::AlignVCenter, m_strCaption);
}
void SegMenuItem::SetItemCfg(const ItemCfg &stItemCfg)
{
	m_stItemCfg = stItemCfg;
}
void SegMenuItem::mousePressEvent(QMouseEvent *ev)
{
	if(rect().contains(ev->pos()))
	{
		if(Qt::LeftButton == ev->button())
		{
			emit Sigtriggered(m_colorBackground);
			ev->ignore();
		}
	}
}
void SegMenuItem::SetFont(const QFont &font)
{
	m_stItemCfg.font = font;
}