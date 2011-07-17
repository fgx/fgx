


#include <QGridLayout>
#include <QFileDialog>

#include "setupwizard/terrasyncpage.h"
#include "xobjects/xopt.h"

TerraSyncPage::TerraSyncPage(MainObject *mob, QWidget *parent) :
    QWizardPage(parent)
{

	mainObject = mob;

	setTitle("Setup TerraSync");
	setSubTitle("Select the directory for TerraSync");

	QGridLayout *gridLayout = new QGridLayout();
	setLayout(gridLayout);

	//= Terrasync enabled
	int row = 0;
	checkBoxUseTerrasync = new QCheckBox();
	checkBoxUseTerrasync->setText("Download Scenery via TerraSync");
	gridLayout->addWidget(checkBoxUseTerrasync, row, 0, 1, 3);
	connect(checkBoxUseTerrasync, SIGNAL(clicked()), this, SLOT(on_checkbox_clicked()));


	//= Path
	row++;
	txtTerraSyncPath = new QLineEdit("");
	gridLayout->addWidget(txtTerraSyncPath, row, 1, 1, 1);
	txtTerraSyncPath->setDisabled(true);
	connect(txtTerraSyncPath, SIGNAL(textChanged(QString)), this, SLOT(check_paths()));

	//= Select path button
	buttSelectPath = new QToolButton();
	gridLayout->addWidget(buttSelectPath, row, 2);
	buttSelectPath->setIcon(QIcon(":/icon/black"));
	buttSelectPath->setDisabled(true);
	connect(buttSelectPath, SIGNAL(clicked()), this, SLOT(on_select_path()));

	//= help label
	row++;
	lblHelp = new QLabel("");
	gridLayout->addWidget(lblHelp, row, 1, 1, 2);


	registerField("terrasync_enabled", checkBoxUseTerrasync);
	registerField("terrasync_path", txtTerraSyncPath);

}

void TerraSyncPage::on_select_path()
{
	QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select TerraSync directory"),
														 txtTerraSyncPath->text(), QFileDialog::ShowDirsOnly);
	if(dirPath.length() > 0){
		txtTerraSyncPath->setText(dirPath);
	}
	check_paths();
}



void TerraSyncPage::on_checkbox_clicked(){
	bool ena = checkBoxUseTerrasync->isChecked();
	txtTerraSyncPath->setEnabled(ena);
	buttSelectPath->setEnabled(ena);
	if(ena){
		txtTerraSyncPath->setFocus();
	}
	check_paths();
}

//===============================================================
//= check_paths() - does not return value but colours help labels
void TerraSyncPage::check_paths()
{

	if(checkBoxUseTerrasync->isChecked()){
		if(QFile::exists(txtTerraSyncPath->text())){
			lblHelp->setText("Ok");
			lblHelp->setStyleSheet("color: green;");

		}else{
			lblHelp->setText("Not found");
			lblHelp->setStyleSheet("color:#990000;");
		}
	}else{
		lblHelp->setText("");
		lblHelp->setStyleSheet("color: #000000;");
	}

}

//===================================================
//= InitializePage
void TerraSyncPage::initializePage()
{
	XOpt opt = mainObject->X->get_opt("terrasync_path");
	checkBoxUseTerrasync->setChecked( opt.enabled );
	txtTerraSyncPath->setText( opt.value );
	on_checkbox_clicked();
}

//====================================================
//= ValidatePage
bool TerraSyncPage::validatePage()
{
	check_paths();

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
