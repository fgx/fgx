// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // xdialog.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information


#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QTreeWidget>
#include <QMessageBox>

#include "settingsdialog.h"
#include "launcher/launcherwindow.h"


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

	accept();
}


