#include <QGridLayout>
#include <QFileDialog>

#include "setupwizard/otherpage.h"
#include "xobjects/xopt.h"

OtherPage::OtherPage(MainObject *mob, QWidget *parent) :
QWizardPage(parent)
{
	
	mainObject = mob;
	
	setTitle("Setup other paths");
	setSubTitle("Setup paths to optional utilities and add-ons");
	
	QGridLayout *gridLayout = new QGridLayout();
	setLayout(gridLayout);
	
	//= FGCom enabled
	int row = 0;
	checkBoxUseFGCom = new QCheckBox();
	checkBoxUseFGCom->setText("Use FGCom for voice communication");
	gridLayout->addWidget(checkBoxUseFGCom, row, 0, 1, 3);
	connect(checkBoxUseFGCom, SIGNAL(clicked()), this, SLOT(on_checkbox_fgcom_clicked()));
	
	//= FGCom Exe Path
	row++;
	FGComExePathLabel = new QLabel("Select FGCom executable path (no change needed by default)");
	gridLayout->addWidget(FGComExePathLabel, row, 1, 1, 1);
	
	row++;
	txtFGComExePath = new QLineEdit("");
	gridLayout->addWidget(txtFGComExePath, row, 1, 1, 1);
	txtFGComExePath->setDisabled(true);
	connect(txtFGComExePath, SIGNAL(textChanged(QString)), this, SLOT(check_fgcom_exe_paths()));
	
	//= Select path button
	buttSelectFGComExePath = new QToolButton();
	gridLayout->addWidget(buttSelectFGComExePath, row, 2);
	buttSelectFGComExePath->setIcon(QIcon(":/icon/black"));
	buttSelectFGComExePath->setDisabled(true);
	connect(buttSelectFGComExePath, SIGNAL(clicked()), this, SLOT(on_select_fgcom_exe_path()));
	
	//= help fgcom exe label
	row++;
	lblHelpFGComExe = new QLabel("");
	gridLayout->addWidget(lblHelpFGComExe, row, 1, 1, 2);
	
	
	//= JSDemo enabled
	row++;
	checkBoxUseJSDemo = new QCheckBox();
	checkBoxUseJSDemo->setText("Use JSDemo for FGx joystick detection");
	gridLayout->addWidget(checkBoxUseJSDemo, row, 0, 1, 3);
	connect(checkBoxUseJSDemo, SIGNAL(clicked()), this, SLOT(on_checkbox_jsdemo_clicked()));
	
	//= JSDemo Exe Path
	row++;
	JSDemoExePathLabel = new QLabel("Select JSDemo executable path (no change needed by default)");
	gridLayout->addWidget(JSDemoExePathLabel, row, 1, 1, 1);
	
	row++;
	txtJSDemoExePath = new QLineEdit("");
	gridLayout->addWidget(txtJSDemoExePath, row, 1, 1, 1);
	txtJSDemoExePath->setDisabled(true);
	connect(txtJSDemoExePath, SIGNAL(textChanged(QString)), this, SLOT(check_jsdemo_exe_paths()));
	
	//= Select path button
	buttSelectJSDemoExePath = new QToolButton();
	gridLayout->addWidget(buttSelectJSDemoExePath, row, 2);
	buttSelectJSDemoExePath->setIcon(QIcon(":/icon/black"));
	buttSelectJSDemoExePath->setDisabled(true);
	connect(buttSelectJSDemoExePath, SIGNAL(clicked()), this, SLOT(on_select_jsdemo_exe_path()));
	
	//= help fgcom exe label
	row++;
	lblHelpJSDemoExe = new QLabel("");
	gridLayout->addWidget(lblHelpJSDemoExe, row, 1, 1, 2);
	
	
	
	registerField("fgcom_enabled", checkBoxUseFGCom);
	registerField("fgcom_exe_path", txtFGComExePath);
	
	registerField("jsdemo_enabled", checkBoxUseJSDemo);
	registerField("jsdemo_exe_path", txtJSDemoExePath);
	
}

void OtherPage::on_select_fgcom_exe_path()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Select FGCom binary"),
													txtFGComExePath->text());
	if(filePath.length() > 0){
		txtFGComExePath->setText(filePath);
	}
	check_fgcom_exe_paths();
}

void OtherPage::on_select_jsdemo_exe_path()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Select js_demo binary"),
													txtJSDemoExePath->text());
	if(filePath.length() > 0){
		txtJSDemoExePath->setText(filePath);
	}
	check_jsdemo_exe_paths();
}


void OtherPage::on_checkbox_fgcom_clicked(){
	bool ena = checkBoxUseFGCom->isChecked();
	txtFGComExePath->setEnabled(ena);
	buttSelectFGComExePath->setEnabled(ena);
	if(ena){
		txtFGComExePath->setFocus();
	}
	check_fgcom_exe_paths();
}

void OtherPage::on_checkbox_jsdemo_clicked(){
	bool ena = checkBoxUseJSDemo->isChecked();
	txtJSDemoExePath->setEnabled(ena);
	buttSelectJSDemoExePath->setEnabled(ena);
	if(ena){
		txtJSDemoExePath->setFocus();
		
		// Aiiiiiiii, said Yves
		if (mainObject->X->getx("jsdemo_exe_path", true) != "") {
		txtJSDemoExePath->setText(mainObject->X->getx("jsdemo_exe_path", true));
		} else {
			if(mainObject->runningOs() == MainObject::MAC){
				
				QString jsDemoExePath;
				QString tmp;
				QDir d;
				jsDemoExePath = "js_demo";
				
				int ind, siz;
				tmp = mainObject->X->fgfs_path();
				ind = tmp.indexOf(QChar('/'));
				siz = 0;
				// march to last '/' in path
				while (ind >= 0) {
					tmp = tmp.mid(ind + 1);
					siz = tmp.size();
					ind = tmp.indexOf(QChar('/'));
				}
				if (siz > 0) {
					tmp = mainObject->X->fgfs_path();
					tmp.chop(siz);
					if (d.exists(tmp)) {
						jsDemoExePath = tmp + jsDemoExePath;
					}
				}
				
				txtJSDemoExePath->setText(jsDemoExePath);
				
				// PLEASE DO NOT TOUCH THIS PART --- END
				
				
			}else if(mainObject->runningOs() == MainObject::LINUX){
				txtJSDemoExePath->setText(QString("js_demo"));
				
			}else if(mainObject->runningOs() == MainObject::WINDOWS){
				txtJSDemoExePath->setText(QString("C:/Program Files/FlightGear/bin/win32/js_demo.exe"));
				
			}else{
				txtJSDemoExePath->setText(QString("UNKNOW OS in jsdemo_default_path()"));
			}
		}

			 
		}
	
	check_jsdemo_exe_paths();
}

//===============================================================
//= check_paths() - does not return value but colours help labels

void OtherPage::check_fgcom_exe_paths()
{
	QString exePath(txtFGComExePath->text());
	bool custom_exists = QFile::exists(exePath);
	lblHelpFGComExe->setText(custom_exists ? "Ok" : "Not found");
	lblHelpFGComExe->setStyleSheet(custom_exists ?  "color: green;" : "color:#990000;");
	if(custom_exists){
		QFileInfo fInfo(exePath);
		if(fInfo.isDir()){
			lblHelpFGComExe->setText("Need a file path, not a directory");
			lblHelpFGComExe->setStyleSheet("color:#990000;");
		}else{
			if(!fInfo.isExecutable()){
				lblHelpFGComExe->setText("Not executable");
				lblHelpFGComExe->setStyleSheet("color:#990000;");
			}
		}
	}
}		

void OtherPage::check_jsdemo_exe_paths()
{
	QString exePath(txtJSDemoExePath->text());
	bool custom_exists = QFile::exists(exePath);
	lblHelpJSDemoExe->setText(custom_exists ? "Ok" : "Not found");
	lblHelpJSDemoExe->setStyleSheet(custom_exists ?  "color: green;" : "color:#990000;");
	if(custom_exists){
		QFileInfo fInfo(exePath);
		if(fInfo.isDir()){
			lblHelpJSDemoExe->setText("Need a file path, not a directory");
			lblHelpJSDemoExe->setStyleSheet("color:#990000;");
		}else{
			if(!fInfo.isExecutable()){
				lblHelpJSDemoExe->setText("Not executable");
				lblHelpJSDemoExe->setStyleSheet("color:#990000;");
			}
		}
	}
}

//===================================================
//= InitializePage
void OtherPage::initializePage()
{
	
	XOpt optExeFGCom = mainObject->X->get_opt("fgcom_exe_path");
	XOpt optExeJSDemo = mainObject->X->get_opt("jsdemo_exe_path");
	checkBoxUseFGCom->setChecked( optExeFGCom.enabled );
	checkBoxUseJSDemo->setChecked( optExeJSDemo.enabled );
	txtFGComExePath->setText( mainObject->X->fgcom_exe_path() );
	txtJSDemoExePath->setText( mainObject->X->jsdemo_exe_path() );
	on_checkbox_fgcom_clicked();
	on_checkbox_jsdemo_clicked();
}

//====================================================
//= ValidatePage
bool OtherPage::validatePage()
{
	check_fgcom_exe_paths();
	
	if(checkBoxUseFGCom->isChecked()){
		if(QFile::exists(txtFGComExePath->text())){
			// TODO - check its writable
			return true;
		}else{
			txtFGComExePath->setFocus();
			return false;
		}
	}
	return true;
}
