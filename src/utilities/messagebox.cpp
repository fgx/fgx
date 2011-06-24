
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
