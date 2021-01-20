#include "MyLineEdit.h"

MyLineEdit::MyLineEdit(QWidget *parent)
	: QLineEdit(parent)
{
}

MyLineEdit::~MyLineEdit()
{
}
void MyLineEdit::focusInEvent(QFocusEvent *ev)
{
	emit SigEditGetFocus();	
	QLineEdit::focusInEvent(ev);
}