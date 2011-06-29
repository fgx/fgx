


#include <QGridLayout>
#include <QFileDialog>

#include "terrasyncpage.h"

TerraSyncPage::TerraSyncPage(MainObject *mob, QWidget *parent) :
    QWizardPage(parent)
{

	mainObject = mob;

	setTitle("Setup terrasync");
	setSubTitle("Select the ??? for terrasync");

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




	registerField("use_terrasync", checkBoxUseTerrasync);
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
}

//===================================================
//= check_paths() - does not return value but colours help labels
void TerraSyncPage::check_paths()
{
	QString style("");
	QString lbl_text("");

	if(checkBoxUseTerrasync->isChecked()){
		if(QFile::exists(txtTerraSyncPath->text())){
			lbl_text.append("Ok");
			style.append("color: green;");

		}else{
			lbl_text.append("Not found");
			style.append("color:#990000;");
		}
	}else{
		lbl_text.append("-");
		style.append("color: #666666;");
	}
	lblHelp->setText(lbl_text);
	lblHelp->setStyleSheet(style);

}

//===================================================
//= InitializePage
void TerraSyncPage::initializePage()
{
	checkBoxUseTerrasync->setChecked( mainObject->settings->value("use_terrasync", "1").toBool() );
	txtTerraSyncPath->setText(mainObject->settings->value("terrasync_path", "").toString() );
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
