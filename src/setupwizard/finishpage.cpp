


#include <QGridLayout>
#include <QFileDialog>
#include <QVBoxLayout>

#include "setupwizard/finishpage.h"
#include "xwidgets/xgroupboxes.h"


FinishPage::FinishPage(MainObject *mob, QWidget *parent) :
    QWizardPage(parent)
{

	mainObject = mob;

	setTitle("Finish Setup");
	setSubTitle("Click the finish button to run");


	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);


	XGroupVBox *grpFgExe = new XGroupVBox("FlightGear Executable");
	mainLayout->addWidget(grpFgExe);
	lblFgExeUsingDefault = new QLabel();
	grpFgExe->addWidget(lblFgExeUsingDefault);
	lblFgExePath = new QLabel();
	grpFgExe->addWidget(lblFgExePath);



	XGroupVBox *grpFgRoot = new XGroupVBox("FlightGear Data");
	mainLayout->addWidget(grpFgRoot);
	lblFgRootUsingDefault = new QLabel();
	grpFgRoot->addWidget(lblFgRootUsingDefault);
	lblFgRootPath = new QLabel();
	grpFgRoot->addWidget(lblFgRootPath);



	XGroupVBox *grpTerrasync = new XGroupVBox("TerraSync");
	mainLayout->addWidget(grpTerrasync);
	lblUsingTerraSync = new QLabel();
	grpTerrasync->addWidget(lblUsingTerraSync);
	lblTerraSyncPath = new QLabel();
	grpTerrasync->addWidget(lblTerraSyncPath);




}

