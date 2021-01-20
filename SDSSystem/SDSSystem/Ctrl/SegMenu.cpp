#include "SegMenu.h"
#include <QTimer>
#include <QDebug>

SegMenu::SegMenu(QWidget *parent)
	: QWidget(parent)
	,m_pAnimation(NULL) //初始化指针	 @xiongz [2020/11/6]
{
	//QTimer::singleShot(10,this,SLOT(InitMenu()));
	InitMenu();
}

SegMenu::~SegMenu()
{
	Clear();
	ClearAnimation();
}
void SegMenu::InitMenu()
{
	//菜单设置为弹出窗口
	this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Popup);
	setAttribute(Qt::WA_TranslucentBackground);
	//开启鼠标悬浮事件
	//setAttribute(Qt::WA_Hover);
	m_pAnimation = new QPropertyAnimation(this, "windowOpacity");
	m_pAnimation->setDuration(C_MENU_DISPEAR_TIME);
	m_pAnimation->setStartValue(1);
	m_pAnimation->setEndValue(0);
	//m_pAnimation->start();
	connect(m_pAnimation, SIGNAL(finished()), this, SLOT(close()));
	//关联
	connect(this, SIGNAL(SigUpdateSize()), this, SLOT(UpdatePlot()));
	connect(&m_timerDispear,SIGNAL(timeout()),this,SLOT(OnTimerDispear()));
}
void SegMenu::SetItemSize(const QSize &size)
{
	m_sizeItem = size;
	emit SigUpdateSize();
}
void SegMenu::AddItem(SegMenuItem *pItem)
{
	if(!pItem)
	{
		return;
	}
	//如果包含相同的指针，则不再添加
	if (m_vctItem.contains(pItem))
	{
		return;
	}
	m_vctItem.append(pItem);
	//关联关闭按钮
	connect(pItem,SIGNAL(Sigtriggered(const QColor &)),this,SLOT(close()));

	emit SigUpdateSize();
}
SegMenuItem * SegMenu::AddItem(const QString &strTitle)
{
	SegMenuItem *p_item = new SegMenuItem(strTitle, this);
	AddItem(p_item);
	return p_item;
}
void SegMenu::UpdatePlot()
{
	if(m_vctItem.empty())
	{
		return;
	}
	resize(QSize(m_sizeItem.width(), m_sizeItem.height()*m_vctItem.count()));
	m_vctRect.clear();
	for(int i = 0;i < m_vctItem.count();++i)
	{
		//m_vctItem[i]->resize(m_sizeItem);
		
		int x_pos = 0;
		int y_pos = i * m_sizeItem.height();
		m_vctItem[i]->InitItem(m_sizeItem);
		m_vctItem[i]->move(x_pos, y_pos);
		//子菜单rect
		QRect rc;
		rc.setTopLeft(QPoint(0,y_pos));
		rc.setSize(m_sizeItem);
		m_vctRect.append(rc);
	}
}
void SegMenu::Clear()
{
	for(auto it : m_vctItem)
	{
		if(it)
		{
			delete it;
			it = NULL;
		}
	}
	m_vctItem.clear();
	m_vctRect.clear();

}
bool SegMenu::event(QEvent *ev)
{
	// class_ameneded 不能是custommenu的成员, 因为winidchange事件触发时, 类成员尚未初始化  
	static bool class_amended = false;
	if (ev->type() == QEvent::WinIdChange)
	{
		HWND hwnd = reinterpret_cast<HWND>(winId());
		if (class_amended == false)
		{
			class_amended = true;
			DWORD class_style = ::GetClassLong(hwnd, GCL_STYLE);
			class_style &= ~CS_DROPSHADOW;
			::SetClassLong(hwnd, GCL_STYLE, class_style); // windows系统函数  
		}
	}
	//鼠标
	if (QEvent::HoverEnter == ev->type())
	{
		qDebug() << "hover";
		if (m_pAnimation)
		{
			//m_pAnimation->stop();
		}

	}
	if (QEvent::HoverLeave == ev->type())
	{
		qDebug() << "hoverleaver";

	}

	if(QEvent::MouseButtonPress == ev->type())
	{
		close();
	}
	if (QEvent::Show == ev->type())
	{
		m_pAnimation->setCurrentTime(0);
		m_pAnimation->start();
		m_pAnimation->stop();
		m_timerDispear.start(C_MENU_TIMER_OUT);
	}
	return QWidget::event(ev);
}
void SegMenu::paintEvent(QPaintEvent *event)
{
	//绘制边界
	QPainter pp(this);
	QPen pen;
	//绘制子窗口背景
	for(int i = 0; i < m_vctItem.count();++i)
	{
		SegMenuItem *p_item = m_vctItem.at(i);
		Q_ASSERT(p_item != NULL);
		//绘制设置
		ItemCfg st_cfg = p_item->GetItemCfg();
		QBrush brush(p_item->GetBackground());
		pen.setColor(st_cfg.colorBorder);
		pen.setWidth(st_cfg.iWidth);
		pp.setPen(pen);
		pp.setBrush(brush);
		//获取绘制区域
		
		QRect rct = m_vctRect.at(i);
		pp.setClipRect(rct);
		if(rct.right() >= this->rect().width()-1)
		{
			rct.setRight(this->rect().width() - 2);
		}
		if(rct.bottom() >= this->rect().height()-1)
		{
			rct.setBottom(this->rect().height() - 2);
		}

		pp.drawRect(rct);

	}

}
//===================指定菜单弹出的位置==================
void SegMenu::Popup(const QPoint &p)
{
	QPoint point = p;
	QWidget *parent = parentWidget();
	if (parent != NULL)
	{
		QRect rc = parent->rect();
		point = parent->mapFromGlobal(p);
		
		if (width() + point.x() > parentWidget()->rect().right())
		{
			point.setX(point.x() - width());
		}
		if (height() + point.y() > parentWidget()->rect().height())
		{
			point.setY(point.y() - height());
		}
		QPoint p_tem = parent->mapToGlobal(point);
		move(p_tem);
		show();
	}
}
void SegMenu::InsertItem(int i,  SegMenuItem *pItem)
{
	Q_ASSERT(pItem);
	if (i < 0 && i > m_vctItem.size())
	{
		return;
	}
	//如果包含相同的指针，则不再添加
	if (m_vctItem.contains(pItem))
	{
		return;
	}
	m_vctItem.insert(i, pItem);
	emit SigUpdateSize();
}
void SegMenu::DeleteItem(int i)
{
	if (i < 0 && i >= m_vctItem.size())
	{
		return;
	}
	delete(m_vctItem.at(i));
	m_vctItem.remove(i);
	emit SigUpdateSize();
}
void SegMenu::DeleteAllItem()
{
	Clear();
	
}

void SegMenu::OnTimerDispear()
{	
	m_pAnimation->setCurrentTime(0);
	m_pAnimation->start();
	m_timerDispear.stop();
	
}
void SegMenu::ClearAnimation()
{
	if (m_pAnimation)
	{
		delete m_pAnimation;
		m_pAnimation = NULL;
		qDebug() << "clear animation";
	}
}