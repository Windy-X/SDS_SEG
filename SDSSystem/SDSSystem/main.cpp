#include "MainDlg.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainDlg w;
    w.show();
    return a.exec();
}
