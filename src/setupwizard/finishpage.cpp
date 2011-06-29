


#include <QGridLayout>
#include <QFileDialog>
#include <QVBoxLayout>

#include "setupwizard/finishpage.h"
#include "xwidgets/xgroupboxes.h"


#include "airports/airportsdata.h"
#include "aircraft/aircraftdata.h"



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


	//= Terrasync
	XGroupVBox *grpTerrasync = new XGroupVBox("TerraSync");
	mainLayout->addWidget(grpTerrasync);
	lblUsingTerraSync = new QLabel();
	grpTerrasync->addWidget(lblUsingTerraSync);
	lblTerraSyncPath = new QLabel();
	grpTerrasync->addWidget(lblTerraSyncPath);

	//= Import Cache Data
	XGroupVBox *grpImports = new XGroupVBox("Import Cache Data");
	mainLayout->addWidget(grpImports);

	checkBoxImportAirports = new QCheckBox("Import Airports");
	grpImports->addWidget(checkBoxImportAirports);
	checkBoxImportAirports->setChecked(true);

	checkBoxImportAicraft = new QCheckBox("Import Aircraft");
	grpImports->addWidget(checkBoxImportAicraft);
	checkBoxImportAicraft->setChecked(true);

}



//===================================================
//= initializePage
void FinishPage::initializePage()
{

	if(field("use_default_fgfs").toBool()){
		lblFgExeUsingDefault->setText("Using Default Path");
		lblFgExePath->setText(mainObject->settings->default_fgfs_path());
	}else{
		lblFgExeUsingDefault->setText("Using Custom Path");
		lblFgExePath->setText(field("fgfs_custom_path").toString());
	}

	if(field("use_default_fgroot").toBool()){
		lblFgRootUsingDefault->setText("Using Default Data Path");
		lblFgRootPath->setText(mainObject->settings->default_fg_root());
	}else{
		lblFgRootUsingDefault->setText("Using Custom Data Path");
		lblFgRootPath->setText(field("fgroot_custom_path").toString());
	}

	if(field("use_terrasync").toBool()){
		lblUsingTerraSync->setText("Using Terrasync");
		lblTerraSyncPath->setText(field("terrasync_path").toString());
	}else{
		lblUsingTerraSync->setText("No Terrasync");
		lblTerraSyncPath->setText("");
	}
	return;
}


//===================================================
//= initializePage
bool FinishPage::validatePage()
{


	mainObject->settings->setValue("use_default_fgfs", field("use_default_fgfs"));
	mainObject->settings->setValue("fgfs_custom_path", field("fgfs_custom_path"));

	mainObject->settings->setValue("use_default_fgroot", field("use_default_fgroot"));
	mainObject->settings->setValue("fgroot_custom_path", field("fgroot_custom_path"));


	mainObject->settings->setValue("use_terrasync", field("use_terrasync"));
	mainObject->settings->setValue("terrasync_path", field("terrasync_path"));

	mainObject->settings->sync();

	if(checkBoxImportAirports->isChecked()){
		AirportsData::import(this, mainObject, true);
	}
	if(checkBoxImportAicraft->isChecked()){
		AircraftData::import(this, mainObject);
	}
	return true;
}
