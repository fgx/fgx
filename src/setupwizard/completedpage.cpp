
#include <QVBoxLayout>
#include <QLabel>

#include "completedpage.h"


CompletedPage::CompletedPage(MainObject *mob, QWidget *parent) :
    QWizardPage(parent)
{


	mainObject = mob;

	setTitle("Completed Setup");
	setSubTitle("Click Done");


	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	QLabel *lblDone = new QLabel("Done");
	mainLayout->addWidget(lblDone);

}
