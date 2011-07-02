//
//#include <QDebug>


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


