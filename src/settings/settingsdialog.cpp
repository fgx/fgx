//
//#include <QDebug>
//
//#include <QtCore/QString>
//#include <QtCore/QStringList>
//#include <QtCore/QByteArray>
//#include <QtCore/QProcess>

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
#include "launcher/launcherwindow.h"

/* Presents the settings "dialog" for the fgfs paths

   TODO - scenery Paths and check paths, change color if path not found

*/

SettingsDialog::SettingsDialog(MainObject *mOb, QWidget *parent) :
	QDialog(parent)
{
	mainObject = mOb;

	setProperty("settings_namespace", QVariant("settings_dialog"));
	mainObject->settings->restoreWindow(this);

	setWindowTitle(tr("Settings"));
	setWindowIcon(QIcon(":/icon/settings"));

	setMinimumWidth(400);

    //* Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);
    mainLayout->setSpacing(20);
    int m = 20;
    mainLayout->setContentsMargins(m,m,m,m);

	//================================================================
	//** Tab Widget
	tabWidget = new QTabWidget();
	mainLayout->addWidget(tabWidget);

	pathsSettingsWidget = new PathsSettingsWidget(mainObject);
	tabWidget->addTab(pathsSettingsWidget, tr("Paths"));

	dbSettingsWidget = new DbSettingsWidget(mainObject);
	tabWidget->addTab(dbSettingsWidget, tr("Database"));


	//================================================================
	//** Tab Widget



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
}

//=================================================================================
//** Settings
void SettingsDialog::load_settings(){
	pathsSettingsWidget->load_settings();
	dbSettingsWidget->load_settings();
}



//=================================================================================
// closeEvent
void SettingsDialog::closeEvent(QCloseEvent *event){
	Q_UNUSED(event);
	mainObject->settings->saveWindow(this);
}




//=================================================================================
// Save Clicked
void SettingsDialog::on_save_clicked(){
	if( !pathsSettingsWidget->validate_paths() ){
		if(QMessageBox::question(this, "Path Error",
								"There are errors in path, Save Anyway?",
								QMessageBox::No | QMessageBox::Yes) == QMessageBox::No){
			return;
		}
	}
	pathsSettingsWidget->save_settings();
	dbSettingsWidget->save_settings();
	//mainObject->db_connect();
	accept();
}


