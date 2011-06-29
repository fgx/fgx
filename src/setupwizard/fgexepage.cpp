
#include <QtCore/QProcess>

#include <QtGui/QToolButton>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>

#include <QtGui/QLabel>
#include <QtGui/QMenu>

#include <QtGui/QFileDialog>

#include "fgexepage.h"

FgExePage::FgExePage(MainObject *mob, QWidget *parent) :
    QWizardPage(parent)
{
	mainObject = mob;

	setTitle("Select FlightGear exectable");
	setSubTitle("Select the default or a custom path");


	QGridLayout *layoutExe = new QGridLayout();
	setLayout(layoutExe);

	//= Default Path
	int row = 0;
	radioDefault = new QRadioButton();
	radioDefault->setText("Use default path");
	layoutExe->addWidget(radioDefault, row, 0, 1, 3);
	connect(radioDefault, SIGNAL(toggled(bool)), this, SLOT(on_default_toggled(bool)));

	//= default help label
	row++;
	lblDefault = new QLabel("--");
	layoutExe->addWidget(lblDefault, row, 1, 1, 2);

	//= Custom path
	row++;
	radioCustom = new QRadioButton();
	radioCustom->setText("Select custom path");
	radioCustom->setChecked(true);
	layoutExe->addWidget(radioCustom, row, 0, 1, 3);

	//= select custom path
	row++;
	txtFgfs = new QLineEdit("");
	layoutExe->addWidget(txtFgfs, row, 1, 1, 1);

	//= Dropdown button for path
	buttExecutable = new QToolButton();
	layoutExe->addWidget(buttExecutable, row, 2);
	buttExecutable->setIcon(QIcon(":/icon/black"));
	buttExecutable->setPopupMode(QToolButton::InstantPopup);

	QMenu *menuFgfs = new QMenu();
	buttExecutable->setMenu(menuFgfs);

	QAction *actionFgfsSelectPath = new QAction(menuFgfs);
	menuFgfs->addAction(actionFgfsSelectPath);
	actionFgfsSelectPath->setText(tr("Select path ..."));
	connect(actionFgfsSelectPath, SIGNAL(triggered()), this, SLOT(on_select_fgfs_path()));

	// does not work for win/osx
	if(mainObject->settings->runningOS() == XSettings::LINUX){
		QAction *actionFgfsAutoSelect = new QAction(menuFgfs);
		menuFgfs->addAction(actionFgfsAutoSelect);
		actionFgfsAutoSelect->setText(tr("Autodetect"));
		connect(actionFgfsAutoSelect, SIGNAL(triggered()), this, SLOT(on_fgfs_autodetect()));
		actionFgfsAutoSelect->setVisible( mainObject->settings->runningOS() != XSettings::WINDOWS );
	}


	//= custom help label
	row++;
	lblCustom = new QLabel("--");
	layoutExe->addWidget(lblCustom, row, 1, 1, 2);

	layoutExe->setColumnStretch(0,1);
	layoutExe->setColumnStretch(1,6);
	layoutExe->setColumnStretch(2,0);

	registerField("use_default_fgfs", radioDefault);
	registerField("fgfs_custom_path", txtFgfs);

}

//=================================================================================
//* Autodetect fgfs - this wont work on windows probably
void FgExePage::on_fgfs_autodetect(){

	// if(mainLayout->settings->runningOs() == )
	// TODO implem,ent Geoffs patch
	QString command = "which fgfs";

	QProcess *process = new QProcess(this);
	process->start(command);

	if(process->waitForStarted()){

		process->waitForFinished();
		QByteArray result =  process->readAllStandardOutput();
		//QByteArray errorResult = process->readAllStandardError();
		QString exe = QString(result).trimmed();

		if(exe.length() == 0){
			// none found
		}else{
			txtFgfs->setText(exe);
		}
	}
}


//=================================================================================
//* Select FGFS  Dialog
void FgExePage::on_select_fgfs_path(){
	QString filePath = QFileDialog::getOpenFileName(this, tr("Select FGFS binary"),
														 txtFgfs->text());
	if(filePath.length() > 0){
		txtFgfs->setText(filePath);
	}
	check_paths();
}

void FgExePage::check_paths()
{
	QString default_path = mainObject->settings->default_fgfs_path();
	QString style("");
	QString lbl_text(default_path);
	if(QFile::exists(default_path)){
		if(radioDefault->isChecked()){
			lbl_text.append(" - Ok exists ");
			style.append("color:green;");
		}
	}else{
		lbl_text.append("Not found ");
		style.append("color:#990000;");
	}
	lblDefault->setText(lbl_text);
	lblDefault->setStyleSheet(style);



	QString style2("");
	QString lbl_text2("");
	if(txtFgfs->text().length() == 0){
		style2.append("color: #000000;");
		lbl_text2.append("Select path to executable");

	}else{
		if(QFile::exists(txtFgfs->text())){
			style2.append("color: green;");
			lbl_text2.append("Ok");
		}else{
			lbl_text2.append("Not found ");
			style2.append("color:#990000;");
		}
	}
	lblCustom->setText(lbl_text2);
	lblCustom->setStyleSheet(style2);
}



//===================================================
//= initializePage
void FgExePage::initializePage()
{
	radioDefault->setChecked( mainObject->settings->value("use_default_fgfs", "1").toBool() );
	lblDefault->setText( QString("Default: ").append(mainObject->settings->default_fgfs_path()) );
	txtFgfs->setText(mainObject->settings->value("fgfs_custom_path").toString());
	check_paths();
}



//====================================================
//= ValidatePage
bool FgExePage::validatePage()
{
	check_paths();
	if(radioDefault->isChecked()){
		//TODO vaidate default path
		QString exFile = mainObject->settings->default_fgfs_path();
		if(QFile::exists(exFile)){
			//TODO check its executable
			return true;
		}else{
			return false;
		}

	}

	if(QFile::exists(txtFgfs->text())){
		// TODO - check its executable
		//perms = QFile::permissions(txtFgfs->text())
		return true;
	}else{
		txtFgfs->setFocus();
		//lblCustom->setText("File does not exist");
	}

	return false;
}


void FgExePage::on_default_toggled(bool state){
	Q_UNUSED(state);
	bool def = radioDefault->isChecked();

	txtFgfs->setDisabled(def);
	buttExecutable->setDisabled( def);
	if(def == false){
		if(txtFgfs->text().length() == 0){
			lblCustom->setText("Select path to exacutable");
		}
		txtFgfs->setFocus();
	}
}
