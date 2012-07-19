// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // statusbar.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information


#include <QDebug>

#include "utilities/statusbar.h"


StatusBar::StatusBar(QWidget *parent) :
    QStatusBar(parent)
{
	
}

//** Show Error **
void StatusBar::showError(QString mesage, int timeout){
	QStatusBar::showMessage(mesage, timeout);
}
void StatusBar::showError(QString mesage){
	QStatusBar::showMessage(mesage);
}


//** Show Message **
void StatusBar::showMessage(QString mesage, int timeout){
    QStatusBar::showMessage(mesage, timeout);
}
void StatusBar::showMessage(QString mesage){
    QStatusBar::showMessage(mesage);
}
