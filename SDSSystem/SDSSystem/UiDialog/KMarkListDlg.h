#pragma once

#include <QDialog>
#include "ui_KMarkListDlg.h"

class KMarkListDlg : public QDialog
{
	Q_OBJECT

public:
	KMarkListDlg(QWidget *parent = Q_NULLPTR);
	~KMarkListDlg();

private:
	Ui::KMarkListDlg ui;
};
