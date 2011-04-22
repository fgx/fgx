

#include "pathssettingswidget.h"

#include <QDebug>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QByteArray>
#include <QtCore/QProcess>

#include <QtGui/QFileDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QFileDialog>


PathsSettingsWidget::PathsSettingsWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{
	mainObject = mOb;

	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	//================================================================
	//** Paths Widget
	checkBoxUseDefaults = new QCheckBox("Use default paths");
	mainLayout->addWidget(checkBoxUseDefaults);
	connect(checkBoxUseDefaults, SIGNAL(clicked()), this, SLOT(on_use_defaults()));



	//===============================================================
	//*** FGFS Group
	grpFgfs = new QGroupBox(tr("Path to FlightGear executable"));
	mainLayout->addWidget(grpFgfs);

	QHBoxLayout *layoutExe = new QHBoxLayout();
	grpFgfs->setLayout(layoutExe);

	txtFgfs = new QLineEdit("");
	layoutExe->addWidget(txtFgfs);

	QToolButton *buttExecutable = new QToolButton();
	layoutExe->addWidget(buttExecutable);
	buttExecutable->setIcon(QIcon(":/icon/black"));
	buttExecutable->setPopupMode(QToolButton::InstantPopup);

	QMenu *menuFgfs = new QMenu();
	buttExecutable->setMenu(menuFgfs);

	QAction *actionFgfsSelectPath = new QAction(menuFgfs);
	menuFgfs->addAction(actionFgfsSelectPath);
	actionFgfsSelectPath->setText(tr("Select path.."));
	connect(actionFgfsSelectPath, SIGNAL(triggered()), this, SLOT(on_select_fgfs_path()));

	QAction *actionFgfsAutoSelect = new QAction(menuFgfs);
	menuFgfs->addAction(actionFgfsAutoSelect);
	actionFgfsAutoSelect->setText(tr("Autodetect"));
	connect(actionFgfsAutoSelect, SIGNAL(triggered()), this, SLOT(on_fgfs_autodetect()));
	actionFgfsAutoSelect->setVisible( mainObject->settings->runningOS() != XSettings::WINDOWS );




	//===============================================================
	//*** FG_ROOT Group
	grpFgRoot = new QGroupBox(tr("FG_ROOT - Path to the data directory"));
	mainLayout->addWidget(grpFgRoot);

	QHBoxLayout *layoutFgRoot = new QHBoxLayout();
	grpFgRoot->setLayout(layoutFgRoot);

	txtFgRoot = new QLineEdit("");
	layoutFgRoot->addWidget(txtFgRoot);

	QToolButton *buttFgRoot = new QToolButton();
	layoutFgRoot->addWidget(buttFgRoot);
	buttFgRoot->setIcon(QIcon(":/icons/black"));
	buttFgRoot->setPopupMode(QToolButton::InstantPopup);

	QMenu *menuFgRoot = new QMenu();
	buttFgRoot->setMenu(menuFgRoot);

	QAction *actFgRootPath = new QAction(menuFgRoot);
	menuFgRoot->addAction(actFgRootPath);
	actFgRootPath->setText(tr("Select path.."));
	connect(actFgRootPath, SIGNAL(triggered()), this, SLOT(on_select_fg_root_path()));


	//===============================================================
	//*** FG_Scenery - UNUSED at the moment
	/*
	grpFgScenery = new QGroupBox(tr("FG_Scenery - Paths to the scenery directories."));
	grpFgScenery->setVisible(false);
	mainLayout->addWidget(grpFgScenery);

	QHBoxLayout *layoutFgScenery = new QHBoxLayout();
	grpFgScenery->setLayout(layoutFgScenery);

	treeFgScenery = new QTreeWidget();
	layoutFgScenery->addWidget(treeFgScenery);
	treeFgScenery->setRootIsDecorated(false);
	treeFgScenery->setAlternatingRowColors(true);
	QTreeWidgetItem *headerItem = treeFgScenery->headerItem();
	headerItem->setText(0, tr("Scenery Paths"));

	QVBoxLayout *layoutFgActionBox = new QVBoxLayout();
	layoutFgScenery->addLayout(layoutFgActionBox);


	// Action Scenery
	buttSceneryUp = new QPushButton();
	layoutFgActionBox->addWidget(buttSceneryUp);
	buttSceneryUp->setText(tr("Up"));
	buttSceneryUp->setIcon(QIcon(":/icon/up"));

	buttSceneryDown = new QPushButton();
	layoutFgActionBox->addWidget(buttSceneryDown);
	buttSceneryDown->setText(tr("Down"));
	buttSceneryDown->setIcon(QIcon(":/icon/down"));

//    QPushButton *buttSceneryAdd = new QPushButton();
//    layoutFgActionBox->addWidget(buttSceneryAdd);
//    buttSceneryAdd->setText(tr("Add Path"));
//    buttSceneryAdd->setIcon(QIcon(":/icons/add"));

	QToolButton *buttSceneryAdd = new QToolButton();
	layoutFgActionBox->addWidget(buttSceneryAdd);
	buttSceneryAdd->setText(tr("Add Path"));
	buttSceneryAdd->setIcon(QIcon(":/icon/add"));
	buttSceneryAdd->setPopupMode(QToolButton::InstantPopup);
	buttSceneryAdd->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttSceneryAdd->setPopupMode(QToolButton::MenuButtonPopup);
	QMenu *menu = new QMenu();
	buttSceneryAdd->setMenu(menu);

	buttSceneryRemove = new QPushButton();
	layoutFgActionBox->addWidget(buttSceneryRemove);
	buttSceneryRemove->setText(tr("Remove"));
	buttSceneryRemove->setIcon(QIcon(":/icon/delete_small"));

	layoutFgActionBox->addStretch(10);
	disable_scenery_actions(true);
	*/

}


//=================================================================================
//** Settings
void PathsSettingsWidget::load_settings(){
	bool use_defaults = mainObject->settings->value("USE_DEFAULT_PATHS", "1").toBool();
	checkBoxUseDefaults->setChecked(use_defaults);
	if(use_defaults){
		txtFgfs->setText(mainObject->settings->default_fgfs_path());
		txtFgRoot->setText(mainObject->settings->default_fg_root());

	}else{
		txtFgfs->setText(mainObject->settings->fgfs_path());
		txtFgRoot->setText(mainObject->settings->fg_root());
	}
	grpFgfs->setDisabled(use_defaults);
	grpFgRoot->setDisabled(use_defaults);
	grpFgfs->setStyleSheet( get_frame_style(true) );
	grpFgRoot->setStyleSheet( get_frame_style(true) );
}



//=================================================================================
//* TODO Disable Scenery Buttons
void PathsSettingsWidget::disable_scenery_actions(bool state){
	buttSceneryDown->setDisabled(state);
	buttSceneryUp->setDisabled(state);
	buttSceneryRemove->setDisabled(state);
}

//=================================================================================
//* Autodetect fgfs - this wont work on windows probably
void PathsSettingsWidget::on_fgfs_autodetect(){

	//if(mainLayout->settings->runningOs() == )
	QString command = "which fgfs";
	qDebug() << "command=" << command;
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
		validate_paths();
	}
}






//=================================================================================
//* Selct FGFS  Dialog
void PathsSettingsWidget::on_select_fgfs_path(){
	QString filePath = QFileDialog::getOpenFileName(this, tr("Select FGFS  binary"),
														 txtFgfs->text());
	if(filePath.length() > 0){
		txtFgfs->setText(filePath);
	}
	validate_paths();
}


//=================================================================================
// Select FG Root Dialog
void PathsSettingsWidget::on_select_fg_root_path(){
	QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select FG_ROOT directory"),
														 txtFgRoot->text(), QFileDialog::ShowDirsOnly);
	if(dirPath.length() > 0){
		txtFgRoot->setText(dirPath);
	}
	validate_paths();
}
//=================================================================================
// Validate
bool PathsSettingsWidget::validate_paths(){

	if(checkBoxUseDefaults->isChecked()){
		return true;
	}
	bool fgfs_ok = QFile::exists( txtFgfs->text() );
	grpFgfs->setStyleSheet( get_frame_style(fgfs_ok) );

	bool fg_root_ok = QFile::exists( txtFgRoot->text() );
	grpFgRoot->setStyleSheet( get_frame_style(fg_root_ok) );

	return fgfs_ok && fg_root_ok;
}


//=================================================================================
// Frame Style
QString PathsSettingsWidget::get_frame_style(bool is_ok){
	return QString("QGroupBox{border:2px solid %1;border-radius:5px;  margin-top: 1ex;} QGroupBox::title{subcontrol-origin: margin;subcontrol-position:top center;padding:0 3px;}").arg(is_ok ? "green" : "#990000");
}


void PathsSettingsWidget::on_use_defaults(){

	grpFgfs->setDisabled(checkBoxUseDefaults->isChecked());
	grpFgRoot->setDisabled(checkBoxUseDefaults->isChecked());
	if(checkBoxUseDefaults->isChecked()){
		mainObject->settings->setValue("custom_FGFS", txtFgfs->text());
		txtFgfs->setText(mainObject->settings->default_fgfs_path());
		mainObject->settings->setValue("custom_FG_ROOT", txtFgRoot->text());
		txtFgRoot->setText(mainObject->settings->default_fg_root());
		mainObject->settings->sync();

	}else{
		txtFgfs->setText(mainObject->settings->value("custom_FGFS").toString());
		mainObject->settings->setValue("custom_FG_ROOT", txtFgRoot->text());
		txtFgRoot->setText(mainObject->settings->value("custom_FG_ROOT").toString());
	}

}

void PathsSettingsWidget::save_settings(){
	mainObject->settings->setValue("FGFS", txtFgfs->text());
	mainObject->settings->setValue("FG_ROOT", txtFgRoot->text());
	mainObject->settings->setValue("USE_DEFAULT_PATHS", checkBoxUseDefaults->isChecked());
	mainObject->settings->sync();
}
