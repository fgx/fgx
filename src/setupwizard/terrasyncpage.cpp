


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

	//= Select path button
	buttSelectPath = new QToolButton();
	gridLayout->addWidget(buttSelectPath, row, 2);
	buttSelectPath->setIcon(QIcon(":/icon/black"));
	buttSelectPath->setPopupMode(QToolButton::InstantPopup);

	//= help label
	row++;
	lblHelp = new QLabel("--");
	gridLayout->addWidget(lblHelp, row, 1, 1, 2);






}

void TerraSyncPage::on_select_path()
{
	QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select TerraSync directory"),
														 txtTerraSyncPath->text(), QFileDialog::ShowDirsOnly);
	if(dirPath.length() > 0){
		txtTerraSyncPath->setText(dirPath);
	}
	//#chack_paths();
}



void TerraSyncPage::on_checkbox_clicked(){
	bool ena = checkBoxUseTerrasync->isChecked();
	txtTerraSyncPath->setEnabled(ena);
	buttSelectPath->setEnabled(ena);
	if(ena){
		txtTerraSyncPath->setFocus();
	}
}
