


#include <QGridLayout>
#include <QFileDialog>

#include "setupwizard/otherpage.h"
#include "xobjects/xopt.h"
#include "xwidgets/xgroupboxes.h"

OtherPage::OtherPage(MainObject *mob, QWidget *parent) :
	QWizardPage(parent)
{

	mainObject = mob;

	setTitle("Utility Paths");
	setSubTitle("Setup paths to all de otherz stuff");

	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	//==================================================
	// FgCom path
	XGroupGBox *grpFgcom = new XGroupGBox("FgCom executable");
	mainLayout->addWidget(grpFgcom);

	int row = 0;
	txtFgCom = new QLineEdit("");
	grpFgcom->addWidget(txtFgCom, row, 0, 1, 1);
	connect(txtFgCom, SIGNAL(textChanged(QString)), this, SLOT(check_paths()) );

	//= Dropdown button for path
	QToolButton *buttFgCom = new QToolButton();
	grpFgcom->addWidget(buttFgCom, row, 1, 1, 1);
	buttFgCom->setIcon(QIcon(":/icon/black"));
	buttFgCom->setPopupMode(QToolButton::InstantPopup);

	QMenu *menuFgCom = new QMenu();
	buttFgCom->setMenu(menuFgCom);

	QAction *actionFgComSelectPath = new QAction(menuFgCom);
	menuFgCom->addAction(actionFgComSelectPath);
	actionFgComSelectPath->setText(tr("Select path ..."));
	connect(actionFgComSelectPath, SIGNAL(triggered()), this, SLOT(on_select_fgcom_path()));

	// does not work for win/osx
	if(mainObject->runningOs() == XSettings::LINUX){
		QAction *actionFgComAutoSelect = new QAction(menuFgCom);
		menuFgCom->addAction(actionFgComAutoSelect);
		actionFgComAutoSelect->setText(tr("Autodetect"));
		connect(actionFgComAutoSelect, SIGNAL(triggered()), this, SLOT(on_fgcom_autodetect()));
		//actionFgfsAutoSelect->setVisible( mainObject->settings->runningOs() != XSettings::WINDOWS );
	}

	row++;
	lblFgCom = new QLabel("Fgcom help label");
	grpFgcom->addWidget(lblFgCom, row, 0, 1, 2);


	//==================================================
	// Joystick Demo path
	XGroupGBox *grpJoystick = new XGroupGBox("Joystick executable");
	mainLayout->addWidget(grpJoystick);

	row = 0;
	txtJoystickDemo = new QLineEdit("");
	grpJoystick->addWidget(txtJoystickDemo, row, 0, 1, 1);
	connect(txtJoystickDemo, SIGNAL(txtJoystickDemo(QString)), this, SLOT(check_paths()) );

	//= Dropdown button for path
	QToolButton *buttJoystick = new QToolButton();
	grpJoystick->addWidget(buttJoystick, row, 1, 1, 1);
	buttJoystick->setIcon(QIcon(":/icon/black"));
	buttJoystick->setPopupMode(QToolButton::InstantPopup);

	QMenu *menuJoystick = new QMenu();
	buttJoystick->setMenu(menuJoystick);

	QAction *actionJoystickSelectPath = new QAction(menuFgCom);
	menuJoystick->addAction(actionJoystickSelectPath);
	actionJoystickSelectPath->setText(tr("Select path ..."));
	connect(actionJoystickSelectPath, SIGNAL(triggered()), this, SLOT(on_select_joystick_path()));

	// does not work for win/osx
	if(mainObject->runningOs() == XSettings::LINUX){
		QAction *actionJoystickAutoSelect = new QAction(menuJoystick);
		menuJoystick->addAction(actionJoystickAutoSelect);
		actionJoystickAutoSelect->setText(tr("Autodetect"));
		connect(actionJoystickAutoSelect, SIGNAL(triggered()), this, SLOT(on_joystick_autodetect()));
		//actionFgfsAutoSelect->setVisible( mainObject->settings->runningOs() != XSettings::WINDOWS );
	}

	row++;
	lblJoystickDemo = new QLabel("joystick help label");
	grpJoystick->addWidget(lblJoystickDemo, row, 0, 1, 2);



	//registerField("terrasync_enabled", checkBoxUseTerrasync);
	//registerField("terrasync_path", txtTerraSyncPath);
	//registerField("terrasync_exe_path", txtTerraSyncExePath);

}



void OtherPage::on_select_fgcom_path()
{
	QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select TerraSync directory"),
														txtFgCom->text(), QFileDialog::ShowDirsOnly);
	if(dirPath.length() > 0){
		txtFgCom->setText(dirPath);
	}
	//check_data_paths();
}

void OtherPage::on_select_joystick_path()
{
	QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select TerraSync directory"),
														txtJoystickDemo->text(), QFileDialog::ShowDirsOnly);
	if(dirPath.length() > 0){
		txtJoystickDemo->setText(dirPath);
	}
	//check_data_paths();
}

void OtherPage::on_fgcom_autodetect(){

}

void OtherPage::on_joystick_autodetect(){

}

void OtherPage::check_paths()
{
	/*
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
		*/
}

//===================================================
//= InitializePage
void OtherPage::initializePage()
{
	//XOpt optData = mainObject->X->get_opt("terrasync_path");
	//XOpt optExe = mainObject->X->get_opt("terrasync_exe_path");
	//checkBoxUseTerrasync->setChecked( optData.enabled );
	//checkBoxUseTerrasync->setChecked( optExe.enabled );
	//txtTerraSyncPath->setText( optData.value );
	//txtTerraSyncExePath->setText( mainObject->X->terrasync_default_path() );
	//on_checkbox_clicked();
}

//====================================================
//= ValidatePage
bool OtherPage::validatePage()
{
	//check_data_paths();
	//check_exe_paths();

	//if(checkBoxUseTerrasync->isChecked()){
	//	if(QFile::exists(txtTerraSyncPath->text())){
	//		// TODO - check its writable
	//		return true;
	//	}else{
	//		txtTerraSyncPath->setFocus();
	//		return false;
	//	}
	//}
	return true;
}
