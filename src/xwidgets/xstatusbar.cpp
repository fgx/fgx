
#include <QDebug>

#include "xstatusbar.h"

XStatusBar::XStatusBar(QWidget *parent) :
    QStatusBar(parent)
{
}

//** Show Error **
void XStatusBar::showError(QString mesage, int timeout){
	setStyleSheet("color: #ff0000; background: #333333; font-size: 12px; font-weight: normal; font-family: courier, monospaced");
	QStatusBar::showMessage(mesage, timeout);
}
void XStatusBar::showError(QString mesage){
	setStyleSheet("color: #ff0000; background: #333333; font-size: 12px; font-weight: normal; font-family: courier, monospaced");
	QStatusBar::showMessage(mesage);
}


//** Show Message **
void XStatusBar::showMessage(QString mesage, int timeout){
	setStyleSheet("color: #00ff00; background: #333333; font-size: 12px; font-weight: normal; font-family: courier, monospaced");
    QStatusBar::showMessage(mesage, timeout);
}
void XStatusBar::showMessage(QString mesage){
    setStyleSheet("color: #00ff00; background: #333333; font-size: 12px; font-weight: normal; font-family: courier, monospaced");
    QStatusBar::showMessage(mesage);
}
