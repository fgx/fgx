


#include <QGridLayout>
#include <QFileDialog>

#include "setupwizard/terrasyncpage.h"
#include "xobjects/xopt.h"

TerraSyncPage::TerraSyncPage(MainObject *mob, QWidget *parent) :
    QWizardPage(parent)
{

	mainObject = mob;

	setTitle("Setup TerraSync and custom scenery");
	setSubTitle("Setup paths to data directory for TerraSync Scenery");

	QGridLayout *gridLayout = new QGridLayout();
	setLayout(gridLayout);

	//= Terrasync enabled
	int row = 0;
	checkBoxUseTerrasync = new QCheckBox();
	checkBoxUseTerrasync->setText("Download scenery via terrasync");
	gridLayout->addWidget(checkBoxUseTerrasync, row, 0, 1, 3);
	connect(checkBoxUseTerrasync, SIGNAL(clicked()), this, SLOT(on_checkbox_clicked()));


	//= Data Path
	row++;
	terraSyncPathLabel = new QLabel("Select terrasync scenery data path");
	gridLayout->addWidget(terraSyncPathLabel, row, 1, 1, 1);
	
	row++;
	txtTerraSyncPath = new QLineEdit("");
	gridLayout->addWidget(txtTerraSyncPath, row, 1, 1, 1);
	txtTerraSyncPath->setDisabled(true);
	connect(txtTerraSyncPath, SIGNAL(textChanged(QString)), this, SLOT(check_data_paths()));

	//= Select path button
	buttSelectDataPath = new QToolButton();
	gridLayout->addWidget(buttSelectDataPath, row, 2);
	buttSelectDataPath->setIcon(QIcon(":/icon/black"));
	buttSelectDataPath->setDisabled(true);
	connect(buttSelectDataPath, SIGNAL(clicked()), this, SLOT(on_select_data_path()));
	
	//= help data label
	row++;
	lblHelp = new QLabel("");
	gridLayout->addWidget(lblHelp, row, 1, 1, 2);
	
	//= Exe Path
	row++;
	terraSyncExePathLabel = new QLabel("Select terrasync executable path");
	gridLayout->addWidget(terraSyncExePathLabel, row, 1, 1, 1);
	
	row++;
	txtTerraSyncExePath = new QLineEdit("");
	gridLayout->addWidget(txtTerraSyncExePath, row, 1, 1, 1);
	txtTerraSyncExePath->setDisabled(true);
	connect(txtTerraSyncExePath, SIGNAL(textChanged(QString)), this, SLOT(check_exe_paths()));
	
	//= Select path button
	buttSelectExePath = new QToolButton();
	gridLayout->addWidget(buttSelectExePath, row, 2);
	buttSelectExePath->setIcon(QIcon(":/icon/black"));
	buttSelectExePath->setDisabled(true);
	connect(buttSelectExePath, SIGNAL(clicked()), this, SLOT(on_select_exe_path()));
	
	//= help data label
	row++;
	lblHelpExe = new QLabel("");
	gridLayout->addWidget(lblHelpExe, row, 1, 1, 2);
	
	//= Custom Scenery enabled
	row++;
	checkBoxUseCustomScenery = new QCheckBox();
	checkBoxUseCustomScenery->setText("Use custom scenery");
	gridLayout->addWidget(checkBoxUseCustomScenery, row, 0, 1, 3);
	connect(checkBoxUseCustomScenery, SIGNAL(clicked()), this, SLOT(on_checkbox_scenery_clicked()));
	
	//= Custom Scenery Data Path
	row++;
	customScenePathLabel = new QLabel("Select custom scenery data path");
	gridLayout->addWidget(customScenePathLabel, row, 1, 1, 1);
	
	row++;
	txtCustomScenePath = new QLineEdit("");
	gridLayout->addWidget(txtCustomScenePath, row, 1, 1, 1);
	txtCustomScenePath->setDisabled(true);
	connect(txtCustomScenePath, SIGNAL(textChanged(QString)), this, SLOT(check_custom_data_paths()));
	
	//= Select path button
	buttSelectCustomDataPath = new QToolButton();
	gridLayout->addWidget(buttSelectCustomDataPath, row, 2);
	buttSelectCustomDataPath->setIcon(QIcon(":/icon/black"));
	buttSelectCustomDataPath->setDisabled(true);
	connect(buttSelectCustomDataPath, SIGNAL(clicked()), this, SLOT(on_select_customscene_data_path()));
	
	//= help data label
	row++;
	lblHelpCust = new QLabel("");
	gridLayout->addWidget(lblHelpCust, row, 1, 1, 2);




	registerField("terrasync_enabled", checkBoxUseTerrasync);
	registerField("terrasync_path", txtTerraSyncPath);
	registerField("terrasync_exe_path", txtTerraSyncExePath);
	registerField("custom_scenery_path", txtCustomScenePath);

}

void TerraSyncPage::on_select_exe_path()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Select terrasync binary"),
													txtTerraSyncExePath->text());
	if(filePath.length() > 0){
		txtTerraSyncExePath->setText(filePath);
	}
	check_exe_paths();
}

void TerraSyncPage::on_select_customscene_data_path()
{
	QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select TerraSync directory"),
														txtTerraSyncPath->text(), QFileDialog::ShowDirsOnly);
	if(dirPath.length() > 0){
		txtCustomScenePath->setText(dirPath);
	}
	check_custom_data_paths();
}

void TerraSyncPage::on_select_data_path()
{
	QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select custom scenery directory"),
														txtTerraSyncPath->text(), QFileDialog::ShowDirsOnly);
	if(dirPath.length() > 0){
		txtTerraSyncPath->setText(dirPath);
	}
	check_data_paths();
}



void TerraSyncPage::on_checkbox_clicked(){
	bool ena = checkBoxUseTerrasync->isChecked();
	txtTerraSyncPath->setEnabled(ena);
	txtTerraSyncExePath->setEnabled(ena);
	buttSelectDataPath->setEnabled(ena);
	buttSelectExePath->setEnabled(ena);
	if(ena){
		txtTerraSyncPath->setFocus();
	}
	check_data_paths();
	check_exe_paths();
}

void TerraSyncPage::on_checkbox_scenery_clicked(){
	bool ena = checkBoxUseCustomScenery->isChecked();
	txtCustomScenePath->setEnabled(ena);
	buttSelectCustomDataPath->setEnabled(ena);
	if(ena){
		txtCustomScenePath->setFocus();
	}
	check_data_paths();
}

//===============================================================
//= check_paths() - does not return value but colours help labels
void TerraSyncPage::check_data_paths()
{
		if(QFile::exists(txtTerraSyncPath->text())){
			lblHelp->setText("Ok");
			lblHelp->setStyleSheet("color: green;");

		}else{
			lblHelp->setText("Not found");
			lblHelp->setStyleSheet("color:#990000;");
		}

}

void TerraSyncPage::check_exe_paths()
{
	QString exePath(txtTerraSyncExePath->text());
		bool custom_exists = QFile::exists(exePath);
		lblHelpExe->setText(custom_exists ? "Ok" : "Not found");
		lblHelpExe->setStyleSheet(custom_exists ?  "color: green;" : "color:#990000;");
		if(custom_exists){
			QFileInfo fInfo(exePath);
			if(fInfo.isDir()){
				lblHelpExe->setText("Need a file path, not a directory");
				lblHelpExe->setStyleSheet("color:#990000;");
			}else{
				if(!fInfo.isExecutable()){
					lblHelpExe->setText("Not executable");
					lblHelpExe->setStyleSheet("color:#990000;");
				}
			}
		}
}		

void TerraSyncPage::check_custom_data_paths()
{
	if(QFile::exists(txtCustomScenePath->text())){
		lblHelpCust->setText("Ok");
		lblHelpCust->setStyleSheet("color: green;");
		
	}else{
		lblHelpCust->setText("Not found");
		lblHelpCust->setStyleSheet("color:#990000;");
	}
	
}

//===================================================
//= InitializePage
void TerraSyncPage::initializePage()
{
	
	XOpt optData = mainObject->X->get_opt("terrasync_path");
	XOpt optExe = mainObject->X->get_opt("terrasync_exe_path");
	XOpt optCust = mainObject->X->get_opt("custom_scenery_path");
	checkBoxUseTerrasync->setChecked( optData.enabled );
	checkBoxUseTerrasync->setChecked( optExe.enabled );
	checkBoxUseCustomScenery->setChecked( optCust.enabled );
	txtTerraSyncPath->setText( optData.value );
	txtTerraSyncExePath->setText( mainObject->X->terrasync_default_path() );
	txtCustomScenePath->setText( mainObject->X->custom_scenery_path() );
	on_checkbox_clicked();
}

//====================================================
//= ValidatePage
bool TerraSyncPage::validatePage()
{
	check_data_paths();
	check_exe_paths();

	if(checkBoxUseTerrasync->isChecked()){
		if(QFile::exists(txtTerraSyncPath->text())){
			// TODO - check its writable
			return true;
		}else{
			txtTerraSyncPath->setFocus();
			return false;
		}
	}
	return true;
}
