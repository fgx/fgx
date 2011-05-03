
#include <QDebug>

#include "xstatusbar.h"

XStatusBar::XStatusBar(QWidget *parent) :
    QStatusBar(parent)
{
}

//** Show Error **
void XStatusBar::showError(QString mesage, int timeout){
	setStyleSheet("color: #AA0000;");
	QStatusBar::showMessage(mesage, timeout);
}
void XStatusBar::showError(QString mesage){
	setStyleSheet("color: #AA0000;");
	QStatusBar::showMessage(mesage);
}


//** Show Message **
void XStatusBar::showMessage(QString mesage, int timeout){
	setStyleSheet("color: black;");
    QStatusBar::showMessage(mesage, timeout);
}
void XStatusBar::showMessage(QString mesage){
    setStyleSheet("color: black;");
    QStatusBar::showMessage(mesage);
}
