#include "MainDlg.h"
#include <QMultiHash>
#include <QtMath>
#include <QDebug>
#include "Config/Config.h"

MainDlg::MainDlg(QWidget *parent)
    : QDialog(parent)
	,m_pMarkDlg(NULL)
	,m_pEventList(NULL)
{
    ui.setupUi(this);
	QTimer::singleShot(100,this,SLOT(InitDlg()));
	m_pTimerStart = new QTimer(this);
	connect(m_pTimerStart,SIGNAL(timeout()),this,SLOT(OnTimerStart()));
	connect(ui.btn_event,SIGNAL(toggled(bool)),this,SLOT(on_btn_event_checked(bool)));

}
   
void MainDlg::InitDlg()
{
	//建立哈希表
	QMultiHash<E_CHANNEL_INDEX,E_DATA_TYPE> hash_index_data;
	hash_index_data.insert(CHANNEL_HR,E_DATA_HR);
	hash_index_data.insert(CHANNEL_NIBP,E_DATA_SYS);
	hash_index_data.insert(CHANNEL_VMR_PIR,E_DATA_VMR);
	hash_index_data.insert(CHANNEL_VMR_PIR,E_DATA_PIR);
	hash_index_data.insert(CHANNEL_VML_PIL, E_DATA_VML);
	hash_index_data.insert(CHANNEL_VML_PIL, E_DATA_PIL);
	m_pMenu = new QMenu(this);
	m_pMenu->addAction("P0");
	m_pMenu->addAction("P1");
	m_pMenu->addAction("P2");
	m_pMenu->addAction("P3");
	QList<QAction *>list_action = m_pMenu->actions();

	//设置渐变
	QPropertyAnimation *animation = new QPropertyAnimation(m_pMenu, "windowOpacity");
	animation->setStartValue(1);
	animation->setEndValue(0);
	animation->setDuration(6000);
	animation->start();
	connect(animation,SIGNAL(finished()),m_pMenu,SLOT(close()));

	QSet<E_DATA_TYPE> set_data;
	for (E_DATA_TYPE e_type = E_DATA_HR; e_type <= E_DATA_MAX; e_type = E_DATA_TYPE(e_type + 1))
	{
		set_data.insert(e_type);
	}
	ChannelParam  st_param1,st_param2;
	QVector<LineCurveInfo>vct_line_curve;
	//添加数据类型
	QVector<ChannelConfig>vct_channel = CConfig::Instance()->ReadChannelsConfig();
	const QMap<E_DATA_TYPE, DataInfo> map_datainfo = CConfig::Instance()->GetDataInfo();
	for(auto it : vct_channel)
	{
		if(E_CHANNEL_INDEX(1) == it.eIndex)
		{
			st_param1.stConfig = it;
			
		}


	}
	if (NULL == m_pMarkDlg)
	{
		m_pMarkDlg = new CMarkWidget(this);
	}
	//关联信号和槽函数
	//重新设置选段控件的时间轴
	connect(ui.plot_ecg, SIGNAL(SigAxisXChanged(const CPlotXMap&)), m_pMarkDlg, SLOT(SetAxisX(const CPlotXMap&)));
	ui.plot_ecg->InitPlot(set_data,st_param1);
	//ui.plot_ecg->AddCurve(st_param1.stConfig.stCurveConfig.vctLineCurves);
	//添加曲线
	ui.plot_ecg->UpdatePlot();


	m_pMarkDlg->resize(ui.plot_ecg->GetChannelParam().rectPlot.size());
	QPoint point_left = ui.plot_ecg->mapTo(this,ui.plot_ecg->GetChannelParam().rectPlot.topLeft());
	m_pMarkDlg->move(point_left);
	m_pMarkDlg->UpdatePlot();
	m_pMarkDlg->show();

	m_pEventList = new EventList(this);
	m_pEventList->Init(QSize(700,80));

	//弹出窗口
	m_pEventList->move(QPoint(20, 500));
	m_pEventList->show();
	connect(m_pEventList,SIGNAL(SigEventMarkParam(const E_EVENT_TYPE &, const QColor &, const QString &)),
		m_pMarkDlg,SLOT(SlotMarkEvent(const E_EVENT_TYPE &, const QColor &, const QString &)));
	connect(this, SIGNAL(SigUpdateLayout(const E_LAYOUT_TYPE &, const QMap<E_LAYOUT_TYPE, QVector<SegmentChannelParam>> &)),
		m_pMarkDlg,SLOT(SetCurrentLayout(const E_LAYOUT_TYPE &, const QMap<E_LAYOUT_TYPE, QVector<SegmentChannelParam>> &)));
	connect(m_pMarkDlg,SIGNAL(SigSendSegTracking(const bool &)),
		ui.plot_ecg,SLOT(SetSegTracking(const bool &)));
}
void MainDlg::OnTimerStart()
{
	QVector<double> vct_hr;
	for(int i = 0; i < 125; ++i)
	{
		double f_value_hr = 50 * sin((double)i/C_PI/4) + 75;
		//double f_value_hr = i  + 25;
		vct_hr.append(f_value_hr);
	}
	ui.plot_ecg->AppendData(E_DATA_HR,vct_hr,false);
	
}
void MainDlg::on_btn_start_clicked()
{
	assert(m_pTimerStart);
	if(m_pTimerStart->isActive())
	{
		m_pTimerStart->stop();
		ui.btn_start->setText("start");
	}
	else
	{
		m_pTimerStart->start(200);
		ui.btn_start->setText("stop");
	}
}
void MainDlg::on_btn_seg_mark_clicked()
{
	static bool b_is_tracking = true;
	if(b_is_tracking)
	{
		m_pMarkDlg->SetSegMode(true);
		ui.plot_ecg->SetSegMode(true);
		ui.btn_seg_mark->setText("stop marking");
	}
	else
	{
		m_pMarkDlg->SetSegMode(false);
		ui.plot_ecg->SetSegMode(false);
		ui.btn_seg_mark->setText("mark");
		//更新绘制趋势
		ui.plot_ecg->UpdatePlot();
	}
	b_is_tracking = !b_is_tracking;
}
void MainDlg::on_btn_zoom_out_clicked()
{
	
}
void MainDlg::on_btn_zoom_in_clicked()
{

}
void MainDlg::on_btn_event_checked(bool bChecked)
{
	if(bChecked)
	{
		ui.plot_ecg->SetEventMode(true);
		m_pMarkDlg->SetEventMode(true);
		ui.btn_event->setText("stop event");

		

	}
	else
	{
		ui.plot_ecg->SetEventMode(false);
		m_pMarkDlg->SetEventMode(false);
		ui.btn_event->setText("event");
	}
}
void MainDlg::mousePressEvent(QMouseEvent *event)
{
	if (Qt::RightButton == event->button())
	{
		m_pMenu->popup(event->globalPos());
	}
}
void MainDlg::on_btn_layout_clicked()
{
	InitLayout();
	E_LAYOUT_TYPE e_layout = E_LAYOUT_PARAMS;
	//
	m_mapMeasureInfo[E_LAYOUT_PARAMS][0].bSelected = true;
	m_mapMeasureInfo[E_LAYOUT_PARAMS][1].bSelected = true;
	m_mapMeasureInfo[E_LAYOUT_PARAMS][11].bSelected = true;
	m_mapMeasureInfo[E_LAYOUT_PARAMS][10].bSelected = true;
	emit SigUpdateLayout(e_layout, m_mapMeasureInfo);
}
void MainDlg::on_btn_set_measure_info_clicked()
{
	QMap<E_DATA_TYPE, MeasureInfo> map_measureinfo;
	for (E_DATA_TYPE e_data = E_DATA_HR; e_data < E_DATA_MAX; e_data = (E_DATA_TYPE)(e_data + 1))
	{
		MeasureInfo st_measureinfo;
		st_measureinfo.SetMeasureInfo("120", "165", "110", "2.04");
		map_measureinfo[e_data] = st_measureinfo;
	}
	m_pMarkDlg->SetMeasureInfo(map_measureinfo);
}
void MainDlg::on_btn_save_segment_clicked()
{
	if (m_pMarkDlg)
	{
		m_pMarkDlg->SaveSegmentMarks();
	}
}
void MainDlg::InitLayout()
{
	SegmentChannelParam st_channel;
	CurveParam st_curve;
	//ECG
	st_channel.bSelected = false;
	st_channel.iIndex = 0;
	st_channel.eChannelType = E_CHANNEL_HR;
	st_curve.crCurve = Qt::green;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_PARAMS].append(st_channel);
	st_channel.vctCurveItem.clear();
	//NIBP
	st_channel.iIndex = 1;
	st_channel.eChannelType = E_CHANNEL_NIBP;
	st_curve.crCurve = Qt::red;
	st_channel.vctCurveItem.append(st_curve);
	st_channel.vctCurveItem.append(st_curve);
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_PARAMS].append(st_channel);
	st_channel.vctCurveItem.clear();
	//VM-R
	st_channel.iIndex = 2;
	st_channel.eChannelType = E_CHANNEL_VM_PI_R;
	st_curve.crCurve = Qt::white;
	st_channel.vctCurveItem.append(st_curve);
	st_curve.crCurve = Qt::blue;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_PARAMS].append(st_channel);
	st_channel.vctCurveItem.clear();
	//VM-L
	st_channel.iIndex = 3;
	st_channel.eChannelType = E_CHANNEL_VM_PI_L;
	st_curve.crCurve = Qt::white;
	st_channel.vctCurveItem.append(st_curve);
	st_curve.crCurve = Qt::blue;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_PARAMS].append(st_channel);
	st_channel.vctCurveItem.clear();
	//SV
	st_channel.iIndex = 4;
	st_channel.eChannelType = E_CHANNEL_SV;
	st_curve.crCurve = Qt::darkYellow;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_PARAMS].append(st_channel);
	st_channel.vctCurveItem.clear();
	//co
	st_channel.iIndex = 5;
	st_channel.eChannelType = E_CHANNEL_CO;
	st_curve.crCurve = Qt::darkYellow;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_PARAMS].append(st_channel);
	st_channel.vctCurveItem.clear();
	//SVR
	st_channel.iIndex = 6;
	st_channel.eChannelType = E_CHANNEL_SVR;
	st_curve.crCurve = Qt::darkYellow;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_PARAMS].append(st_channel);
	st_channel.vctCurveItem.clear();
	//RR
	st_channel.iIndex = 7;
	st_channel.eChannelType = E_CHANNEL_SVR;
	st_curve.crCurve = Qt::darkYellow;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_PARAMS].append(st_channel);
	st_channel.vctCurveItem.clear();
	//SpO2
	st_channel.iIndex = 8;
	st_channel.eChannelType = E_CHANNEL_SVR;
	st_curve.crCurve = Qt::darkYellow;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_PARAMS].append(st_channel);
	st_channel.vctCurveItem.clear();
	//HRV
	st_channel.iIndex = 9;
	st_channel.eChannelType = E_CHANNEL_HRV;
	st_curve.crCurve = Qt::red;
	st_channel.vctCurveItem.append(st_curve);
	st_curve.crCurve = Qt::darkYellow;
	st_channel.vctCurveItem.append(st_curve);
	st_curve.crCurve = Qt::darkGreen;
	st_channel.vctCurveItem.append(st_curve);
	st_curve.crCurve = Qt::darkBlue;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_PARAMS].append(st_channel);
	st_channel.vctCurveItem.clear();
	//LFHF
	st_channel.iIndex = 10;
	st_channel.eChannelType = E_CHANNEL_LFHF;
	st_curve.crCurve = Qt::green;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_PARAMS].append(st_channel);
	st_channel.vctCurveItem.clear();
	//slop
	st_channel.iIndex = 11;
	st_channel.eChannelType = E_CHANNEL_BRS;
	st_curve.crCurve = Qt::green;
	st_channel.vctCurveItem.append(st_curve);
	st_curve.crCurve = Qt::darkCyan;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_PARAMS].append(st_channel);
	st_channel.vctCurveItem.clear();
	//ECG_I
	st_channel.iIndex = 0;
	st_channel.eChannelType = E_CHANNEL_ECG_I;
	st_curve.crCurve = Qt::green;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_ECG].append(st_channel);
	st_channel.vctCurveItem.clear();
	//ECG_II
	st_channel.iIndex = 1;
	st_channel.eChannelType = E_CHANNEL_ECG_II;
	st_curve.crCurve = Qt::green;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_ECG].append(st_channel);
	st_channel.vctCurveItem.clear();
	//ECG_III
	st_channel.iIndex = 2;
	st_channel.eChannelType = E_CHANNEL_ECG_III;
	st_curve.crCurve = Qt::green;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_ECG].append(st_channel);
	st_channel.vctCurveItem.clear();
	//ECG_AVR
	st_channel.iIndex = 3;
	st_channel.eChannelType = E_CHANNEL_ECG_AVR;
	st_curve.crCurve = Qt::green;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_ECG].append(st_channel);
	st_channel.vctCurveItem.clear();
	//ECG_AVL
	st_channel.iIndex = 4;
	st_channel.eChannelType = E_CHANNEL_ECG_AVL;
	st_curve.crCurve = Qt::green;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_ECG].append(st_channel);
	st_channel.vctCurveItem.clear();
	//ECG_AVF
	st_channel.iIndex = 5;
	st_channel.eChannelType = E_CHANNEL_ECG_AVF;
	st_curve.crCurve = Qt::green;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_ECG].append(st_channel);
	st_channel.vctCurveItem.clear();
	//ECG_V1
	st_channel.iIndex = 6;
	st_channel.eChannelType = E_CHANNEL_ECG_V1;
	st_curve.crCurve = Qt::green;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_ECG].append(st_channel);
	st_channel.vctCurveItem.clear();
	//ECG_V2
	st_channel.iIndex = 7;
	st_channel.eChannelType = E_CHANNEL_ECG_V2;
	st_curve.crCurve = Qt::green;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_ECG].append(st_channel);
	st_channel.vctCurveItem.clear();
	//ECG_V3
	st_channel.iIndex = 8;
	st_channel.eChannelType = E_CHANNEL_ECG_V3;
	st_curve.crCurve = Qt::green;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_ECG].append(st_channel);
	st_channel.vctCurveItem.clear();
	//ECG_V4
	st_channel.iIndex = 9;
	st_channel.eChannelType = E_CHANNEL_ECG_V4;
	st_curve.crCurve = Qt::green;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_ECG].append(st_channel);
	st_channel.vctCurveItem.clear();
	//ECG_V5
	st_channel.iIndex = 10;
	st_channel.eChannelType = E_CHANNEL_ECG_V5;
	st_curve.crCurve = Qt::green;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_ECG].append(st_channel);
	st_channel.vctCurveItem.clear();
	//ECG_V6
	st_channel.iIndex = 11;
	st_channel.eChannelType = E_CHANNEL_ECG_V6;
	st_curve.crCurve = Qt::green;
	st_channel.vctCurveItem.append(st_curve);
	m_mapMeasureInfo[E_LAYOUT_ECG].append(st_channel);
	st_channel.vctCurveItem.clear();
}