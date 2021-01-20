#pragma once

#include <QtWidgets/QDialog>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QPropertyAnimation>
#include "Plot.h"
#include "Mark/MarkWidget.h"
#include "Ctrl/EventList.h"
#include "ui_MainDlg.h"

class MainDlg : public QDialog
{
    Q_OBJECT

public:
    MainDlg(QWidget *parent = Q_NULLPTR);
signals:	
	void SigAxisXChanged(const CPlotXMap& cAxisX);
	void SigUpdateLayout(const E_LAYOUT_TYPE &,const QMap<E_LAYOUT_TYPE,QVector<SegmentChannelParam>> &);
private slots:
	void InitDlg();
	void OnTimerStart();
	void on_btn_start_clicked();
	void on_btn_seg_mark_clicked();
	void on_btn_zoom_out_clicked();
	void on_btn_zoom_in_clicked();
	void on_btn_event_checked(bool);
	void on_btn_layout_clicked();
	void on_btn_set_measure_info_clicked();
	void on_btn_save_segment_clicked();
protected slots:
	virtual void mousePressEvent(QMouseEvent *event) override;
private:
	void InitLayout();
private:
    Ui::MainDlgClass ui;
	CMarkWidget *m_pMarkDlg;
	QTimer *m_pTimerStart;
	EventList *m_pEventList;
	QMenu *m_pMenu;
	QMap<E_LAYOUT_TYPE, QVector<SegmentChannelParam>> m_mapMeasureInfo;

};
