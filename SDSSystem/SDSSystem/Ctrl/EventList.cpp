#include "EventList.h"
#include <QPainter>
#include <QString>
#include <QDebug>
#include <QEvent>
#include <Config.h>
//控件样式表字符串配置
static const char C_BTN_FONT_SIZE[] = "font-family:arial;font:bold;font-size:30px;";
static const char C_BTN_BORDER_STYLE[] = "border-radius:3px;";
static const char C_EDIT_BORDER_STYLE[] = "border-radius:3px;";
static const char C_EDIT_BORDER_WIDTH[] = "border:5px groove rgb(160,30,240,255) ;";
static const char C_BTN_MEDICINE_BACKGROUND[] = "background-color:rgb(255,255,0,255);";
static const char C_BTN_BED_UP_BACKGROUND[] = "background-color:rgb(0,255,0,255);";
static const char C_BTN_BED_DOWN_BACKGROUND[] = "background-color:rgb(255,106,106,255);";
static const char C_BTN_FIRST_AID_BACKGROUND[] = "background-color:rgb(255,193,37,255);";
static const char C_EDIT_INPUT_BACKGROUND[] = "background-color:rgb(160,30,240,255);";
//控件空间参数配置(间距，线宽度)
static const int C_CTRL_SPACING = 5;			//控件之间的间隙
static const int C_CTRL_MARGINS = 10;			//边界间隙
static const int C_WIDGET_BORDER_WIDTH = 5;		//控件线宽度
static const int C_WIDGET_ROUND_X = 3;			//控件圆角矩形X轴角度
static const int C_WIDGET_ROUND_Y = 20;			//控件圆角矩形Y轴角度
static const QColor C_WIDGET_BORDER_COLOR = QColor(139, 131, 120);

EventList::EventList(QWidget *parent)
	: QWidget(parent)
{
	Clear();
}

EventList::~EventList()
{
	Clear();
}
void EventList::paintEvent(QPaintEvent *event)
{
	QPainter pp(this);
	pp.setRenderHint(QPainter::Antialiasing, true);
	QPen pen;
	pen.setWidth(C_WIDGET_BORDER_WIDTH);
	pen.setColor(C_WIDGET_BORDER_COLOR);
	pp.setPen(pen);
	QBrush brush(Qt::black);
	pp.setBrush(brush);
	QRect rc(QPoint(0,0),QSize(this->size().width()-1,this->size().height()-1));
	pp.drawRoundRect(rc,C_WIDGET_ROUND_X,C_WIDGET_ROUND_Y);
}
void EventList::Init(const QSize &size)
{
	this->resize(size);
	m_mapMarkStyle = CConfig::Instance()->GetEventMarksParam();
	//设置尺寸策略
	QLayout		*layer = NULL;				//布局
	QSizePolicy policy;
	policy.setHorizontalPolicy(QSizePolicy::Fixed);
	policy.setVerticalPolicy(QSizePolicy::Expanding);
	////定义布局
	if (!layer)
	{
		layer = new QHBoxLayout();
	}
	//设置布局边界
	layer->setMargin(C_CTRL_MARGINS);
	//设置布局间隙
	layer->setSpacing(C_CTRL_SPACING);
	//控件宽度
	int i_count = m_mapMarkStyle.count();
	int i_ctrl_width = (this->size().width() - 2 * C_CTRL_MARGINS - 4 * C_CTRL_SPACING ) / i_count;
	QString str_style_sheet;
	for (auto iter = m_mapMarkStyle.begin();
		iter != m_mapMarkStyle.end();
		++iter)
	{
		E_EVENT_TYPE e_event_type = iter.key();
		QString str_bold = "";
		if (iter->fontMark.bold())
		{
			str_bold = "bold";
		}
		str_style_sheet = QString("font-family:%1;font:%2;font-size:%3px;\
			background-color:%4;border-%5;border:%6px %7")
			.arg(iter->fontMark.family())
			.arg(str_bold)
			.arg(iter->fontMark.pixelSize())
			.arg(MarkStyleSheetParam::Color2QString(iter->colorBackground))
			.arg(iter->stBorder.strStyle)
			.arg(iter->stBorder.iWidth)
			.arg(MarkStyleSheetParam::Color2QString(iter->stBorder.colorBorder));
		if (E_EVENT_USER_DEF == e_event_type)
		{
			QLineEdit *p_edit = new QLineEdit(this);
			p_edit->setStyleSheet(str_style_sheet);
			p_edit->setSizePolicy(policy);
			p_edit->setAlignment(Qt::AlignHCenter);
			p_edit->setFixedWidth(i_ctrl_width);
			p_edit->setText(iter->strDescripiton);
			m_mapWidget[e_event_type] = static_cast<QWidget *>(p_edit);
			connect(p_edit, SIGNAL(returnPressed()),this,SLOT(SlotEditInputReturnPressed()));
			connect(p_edit, SIGNAL(SigEditGetFocus()), this, SIGNAL(SigClearSubCtrlFocus()));
		}
		else
		{
			QPushButton *p_button = new QPushButton(this);	
			QString str_des(iter->strDescripiton);
			p_button->setStyleSheet(str_style_sheet);
			p_button->setSizePolicy(policy);
			p_button->setFixedWidth(i_ctrl_width);
			p_button->setText(str_des);
			m_mapWidget[e_event_type] = static_cast<QWidget *>(p_button);
			connect(p_button,SIGNAL(clicked()),this,SLOT(OnClickBtn()));
		}
		if (layer)
		{
			layer->addWidget(m_mapWidget[e_event_type]);
		}
	}
	this->setLayout(layer);
}
void EventList::Clear()
{
	m_mapMarkStyle.clear();
	for (auto &iter : m_mapWidget)
	{
		if (NULL != iter)
		{
			delete iter;
			iter = NULL;
		}
	}
	m_mapWidget.clear();
}
void EventList::OnClickBtn()
{
	QObject *p_obj = QObject::sender();
	E_EVENT_TYPE e_event_type= E_EVENT_NONE;
	for (auto iter : m_mapWidget)
	{
		if (p_obj == iter)
		{
			e_event_type = m_mapWidget.key(iter);
			MarkStyleSheetParam st_mark_param = m_mapMarkStyle.value(e_event_type);
			emit SigEventMarkParam(e_event_type,st_mark_param.colorBackground,st_mark_param.strDescripiton);
			//debug <2020/12/9> xiongz
			qDebug() << st_mark_param.strDescripiton;
		}
	}
}
void EventList::SlotEditInputReturnPressed()
{
	//static_cast转化不安全
	const QLineEdit *p_edit =dynamic_cast<QLineEdit *>( m_mapWidget.value(E_EVENT_USER_DEF));
	if (NULL == p_edit)
	{
		return;
	}
	MarkStyleSheetParam st_mark_param = m_mapMarkStyle.value(E_EVENT_USER_DEF);
	QString str_text = p_edit->text();
	emit SigEventMarkParam(E_EVENT_USER_DEF,st_mark_param.colorBackground,str_text);
	qDebug() << str_text;


}
void EventList::keyPressEvent(QKeyEvent *ev)
{
	//拦截enter键，避免qpushbutton同时响应
	qDebug() << "key down";
	
}
