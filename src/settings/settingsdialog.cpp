
#include <QDebug>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QByteArray>
#include <QtCore/QProcess>

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QFileDialog>
#include <QtGui/QTreeWidget>
#include <QtGui/QMessageBox>

#include "settingsdialog.h"

/* Presents the settings "dialog" for the fgfs paths

   TODO - scenery Paths and check paths, change color if path not found

*/

SettingsDialog::SettingsDialog(QWidget *parent) :
	QDialog(parent)
{

	setProperty("settings_namespace", QVariant("settings_window"));
	settings.restoreWindow(this);

	setWindowTitle(tr("Settings"));
	setWindowIcon(QIcon(":/icon/settings"));


    //* Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);
    mainLayout->setSpacing(20);
    int m = 20;
    mainLayout->setContentsMargins(m,m,m,m);


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
	buttExecutable->setIcon(QIcon(":/icons/black"));
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
	actionFgfsAutoSelect->setVisible( settings.runningOS() != XSettings::WINDOWS );




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


    //*** Action Scenery
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


    //*** Bottom Button Box
    QHBoxLayout *buttonBox = new QHBoxLayout();
    mainLayout->addLayout(buttonBox);
    buttonBox->addStretch(10);

	QPushButton *buttCancel = new QPushButton();
	buttonBox->addWidget(buttCancel);
	buttCancel->setText("Cancel");
	buttCancel->setIcon(QIcon(":/icon/cancel"));
	connect(buttCancel, SIGNAL(clicked()), this, SLOT(reject()));

    QPushButton *buttSave = new QPushButton();
    buttonBox->addWidget(buttSave);
    buttSave->setText("Save");
	buttSave->setIcon(QIcon(":/icon/save"));
	connect(buttSave, SIGNAL(clicked()), this, SLOT(on_save_clicked()));


	load_settings();
	validate_paths();
}

//=================================================================================
//** Settings
void SettingsDialog::load_settings(){
	txtFgfs->setText(settings.fgfs_path());
	txtFgRoot->setText(settings.fg_root());
}


//=================================================================================
//* TODO Disable Scenery Buttons
void SettingsDialog::disable_scenery_actions(bool state){
    buttSceneryDown->setDisabled(state);
    buttSceneryUp->setDisabled(state);
    buttSceneryRemove->setDisabled(state);
}

//=================================================================================
//* Autodetect fgfs - this wont work on windows proabably
void SettingsDialog::on_fgfs_autodetect(){

	//if(settings.runningOs() == )
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
void SettingsDialog::on_select_fgfs_path(){
	QString filePath = QFileDialog::getOpenFileName(this, tr("Select FGFS  binary"),
														 txtFgfs->text());
	if(filePath.length() > 0){
		txtFgfs->setText(filePath);
	}
	validate_paths();
}


//=================================================================================
// Select FG Root Dialog
void SettingsDialog::on_select_fg_root_path(){
	QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select FG_ROOT directory"),
														 txtFgRoot->text(), QFileDialog::ShowDirsOnly);
	if(dirPath.length() > 0){
		txtFgRoot->setText(dirPath);
	}
	validate_paths();
}


//=================================================================================
// closeEvent
void SettingsDialog::closeEvent(QCloseEvent *event){
	Q_UNUSED(event);
	settings.saveWindow(this);
}


//=================================================================================
// Validate
bool SettingsDialog::validate_paths(){

	bool fgfs_ok = QFile::exists( txtFgfs->text() );
	grpFgfs->setStyleSheet( get_frame_style(fgfs_ok) );

	bool fg_root_ok = QFile::exists( txtFgRoot->text() );
	grpFgRoot->setStyleSheet( get_frame_style(fg_root_ok) );

	return fgfs_ok && fg_root_ok;
}


//=================================================================================
// Frame Style
QString SettingsDialog::get_frame_style(bool is_ok){
	return QString("QGroupBox{border:2px solid %1;border-radius:5px;  margin-top: 1ex;} QGroupBox::title{subcontrol-origin: margin;subcontrol-position:top center;padding:0 3px;}").arg(is_ok ? "green" : "#990000");

}


//=================================================================================
// Save Clicked
void SettingsDialog::on_save_clicked(){
	if( !validate_paths() ){
		if(QMessageBox::question(this, "Path Error",
								"There are errors in path, Save Anyway?",
								QMessageBox::No | QMessageBox::Yes) == QMessageBox::No){
			return;
		}
	}

	settings.setValue("FGFS", txtFgfs->text());
	settings.setValue("FG_ROOT", txtFgRoot->text());
	settings.sync();
	accept();
}
