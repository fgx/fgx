
#include <QDebug>
#include <QMessageBox>

#include "utilities/messagebox.h"


MessageBox::MessageBox(QWidget *parent) :
    QMessageBox(parent)
{
	
}



//** Show Message **
void MessageBox::showMessage(){

	QMessageBox msgBox;
	msgBox.setText("The document has been modified.");
	msgBox.exec();
	
	
}
