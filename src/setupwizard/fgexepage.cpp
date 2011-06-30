
#include <QtDebug>
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
	connect(txtFgfs, SIGNAL(textChanged(QString)), this, SLOT(check_paths()) );

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
	if(mainObject->settings->runningOs() == XSettings::LINUX){
		QAction *actionFgfsAutoSelect = new QAction(menuFgfs);
		menuFgfs->addAction(actionFgfsAutoSelect);
		actionFgfsAutoSelect->setText(tr("Autodetect"));
		connect(actionFgfsAutoSelect, SIGNAL(triggered()), this, SLOT(on_fgfs_autodetect()));
		//actionFgfsAutoSelect->setVisible( mainObject->settings->runningOs() != XSettings::WINDOWS );
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
//= Select FGFS  Dialog
void FgExePage::on_select_fgfs_path(){
	QString filePath = QFileDialog::getOpenFileName(this, tr("Select FGFS binary"),
														 txtFgfs->text());
	if(filePath.length() > 0){
		txtFgfs->setText(filePath);
	}
	check_paths();
}

//=================================================================================
//= Check paths and color labels
void FgExePage::check_paths()
{
	//= Check the default path
	QString default_path = mainObject->settings->fgfs_path_default();
	bool default_exists = QFile::exists(default_path);
	QString lbl_default(default_path);
	lbl_default.append( default_exists ? " - Ok" : "Not Found" );
	QString style_default("");

	if(radioDefault->isChecked()){
		style_default.append(default_exists ? "color:green;" : "color:#990000;");
	}else{
		style_default.append("color:#444444;");
	}
	lblDefault->setText(lbl_default);
	lblDefault->setStyleSheet(style_default);

	//= Check Custom Path
	if(radioCustom->isChecked()){
		QString custom_path = txtFgfs->text().trimmed();
		if(custom_path.size() == 0){
			lblCustom->setText("Select a path");
			lblCustom->setStyleSheet("color: #000099;");
		}else{
			bool custom_exists = QFile::exists(custom_path);
			lblCustom->setText(custom_exists ? "Ok" : "Not found");
			lblCustom->setStyleSheet(custom_exists ?  "color: green;" : "color:#990000;");
		}
	}else{
		lblCustom->setText("");
	}


}



//===================================================
//= initializePage
void FgExePage::initializePage()
{
	radioDefault->setChecked( mainObject->settings->value("use_default_fgfs", "1").toBool() );
	lblDefault->setText( QString("Default: ").append(mainObject->settings->fgfs_path_default()) );
	txtFgfs->setText(mainObject->settings->value("fgfs_custom_path").toString());
	check_paths();
}



//====================================================
//= ValidatePage
bool FgExePage::validatePage()
{
	check_paths();
	if(radioDefault->isChecked()){
		QString exFile = mainObject->settings->fgfs_path_default();
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
	}
	return false;
}


void FgExePage::on_default_toggled(bool state){
	//Q_UNUSED(state);
	qDebug() << state;
	bool def = radioDefault->isChecked();

	txtFgfs->setDisabled(def);
	buttExecutable->setDisabled( def);
	if(def == false){
		if(txtFgfs->text().length() == 0){
			lblCustom->setText("Select path to exacutable");
		}
		txtFgfs->setFocus();
	}

	check_paths();
}
