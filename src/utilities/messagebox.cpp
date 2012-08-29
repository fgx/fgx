// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // messagebox.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information


#include <QDebug>
#include <QMessageBox>

#include "utilities/messagebox.h"


MessageBox::MessageBox(QWidget *parent) :
    QMessageBox(parent)
{
	
}



//** Show Message **
void MessageBox::showWindowMessage(QString alertMessage){

	QMessageBox msgBox;
	msgBox.setText(alertMessage);
	msgBox.exec();
	
	
}
