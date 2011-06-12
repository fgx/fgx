
#include <QDebug>

#include "xstatusbar.h"

XStatusBar::XStatusBar(QWidget *parent) :
    QStatusBar(parent)
{
}

//** Show Error **
void XStatusBar::showError(QString mesage, int timeout){
	QStatusBar::showMessage(mesage, timeout);
}
void XStatusBar::showError(QString mesage){
	QStatusBar::showMessage(mesage);
}


//** Show Message **
void XStatusBar::showMessage(QString mesage, int timeout){
    QStatusBar::showMessage(mesage, timeout);
}
void XStatusBar::showMessage(QString mesage){
    QStatusBar::showMessage(mesage);
}
