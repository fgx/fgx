


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



//===================================================
//= initializePage
void FinishPage::initializePage()
{
	//radioDefault->setChecked( mainObject->settings->value("USE_DEFAULT_FG_ROOT", "1").toBool() );
	//lblDefault->setText( QString("Default: ").append(mainObject->settings->default_fg_root()) );
	qDebug() << field("fgfs_use_default");
	qDebug() << field("fgfs_path");
	qDebug() << field("fgroot_use_default");
	qDebug() << field("fgroot_path");

	if(field("fgfs_use_default").toBool()){
		lblFgExeUsingDefault->setText("Using Default Path");
		lblFgExePath->setText(mainObject->settings->default_fgfs_path());
	}else{
		lblFgExeUsingDefault->setText("Using Custom Path");
		lblFgExePath->setText(field("fgfs_path").toString());
	}

	if(field("fgroot_use_default").toBool()){
		lblFgRootUsingDefault->setText("Using Default Data Path");
		lblFgRootPath->setText(mainObject->settings->default_fg_root());
	}else{
		lblFgRootUsingDefault->setText("Using Custom Data Path");
		lblFgRootPath->setText(field("fgroot_path").toString());
	}

	if(field("use_terrasync").toBool()){
		lblUsingTerraSync->setText("Using Terrasync");
		lblTerraSyncPath->setText(field("terrasync_path").toString());
	}else{
		lblUsingTerraSync->setText("No Terrasync");
		lblTerraSyncPath->setText("");
	}


}


