
//#include <QDebug>

#include <QCoreApplication>
#include <QApplication>
#include <QTimer>

#include <QStyleFactory>

#include <QtGui/QDesktopServices>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QCursor>


#include "xobjects/mainobject.h"
#include "launcher/launcherwindow.h"

#include "settings/settingsdialog.h"
#include "utilities/utilities.h"
#include "setupwizard/setupwizard.h"

MainObject::MainObject(QObject *parent) :
    QObject(parent)
{

	//= init the LOG file
	util_setStdLogFile();

	//= XSettings Object
    settings = new XSettings();

	//= Set GLobal style
	QApplication::setStyle( QStyleFactory::create(settings->style_current()) );

	launcher_flag = false;

	mpMapWidget = 0;
	viewLogWidget = 0;

	//============================
	//== Tray Icon
	trayIcon = new QSystemTrayIcon(QIcon(":/icon/favicon"), this);
    trayIcon->setToolTip("FlightGear Launcher");
    trayIcon->setVisible(true);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(on_tray_icon(QSystemTrayIcon::ActivationReason)));


	//== Tray Menu and actions
    popupMenu = new QMenu();
    trayIcon->setContextMenu(popupMenu);

	//== Callsign - label in a widget..
    actionCallsign = new QWidgetAction(this);
    lblCallsign = new QLabel();
    lblCallsign->setText("(your callsign)");
	lblCallsign->setStyleSheet("color: #0000ff; padding: 0px; font-weight: bold;");
    actionCallsign->setDefaultWidget(lblCallsign);
	popupMenu->addAction(actionCallsign);

	//= Launcher Action
	actionLauncher = popupMenu->addAction(tr("Launcher"));
	actionLauncher->setIconVisibleInMenu(true);
	connect(actionLauncher, SIGNAL(triggered()), this, SLOT(on_launcher()) );

	//= MpMap action
	actionMpMap = popupMenu->addAction(tr("Start Map Widget"));
	actionMpMap->setIconVisibleInMenu(true);
    connect(actionMpMap, SIGNAL(triggered()), this, SLOT(on_mpmap()));


	//= Settings Action
	QAction *actionSetupWizard= new QAction(this);
	actionSetupWizard->setIcon(QIcon(":/icon/wizard"));
	actionSetupWizard->setText(tr("Setup Wizard..."));
	actionSetupWizard->setIconVisibleInMenu(true);
	popupMenu->addAction(actionSetupWizard);
	connect(actionSetupWizard, SIGNAL(triggered()),
			this, SLOT(show_setup_wizard())
    );

	//== View Logs
	QAction *actionViewLog= new QAction(this);
	actionViewLog->setIcon(QIcon(":/icon/log"));
	actionViewLog->setText(tr("View Log..."));
	actionViewLog->setIconVisibleInMenu(true);
	popupMenu->addAction(actionViewLog);
	connect(actionViewLog, SIGNAL(triggered()),
			this, SLOT(on_view_log())
	);

    popupMenu->addSeparator();



	//== Quit
	actionQuit = popupMenu->addAction(QIcon(":/icon/quit"), tr("Quit"));
	actionQuit->setIconVisibleInMenu(true);
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(on_quit()));

	//==================
    trayIcon->show();

	connect(this, SIGNAL(show_settings(int)), this, SLOT(on_settings(int)));

	//== initialise after initial show so UI dont look frozen while cache loading etc
	QTimer::singleShot(300, this, SLOT(initialize()));

}

MainObject::~MainObject()
{
    outLog(util_getDateTimestg()+" - Application close");
}

//============================================================================
//** Shows the Launcher window
void MainObject::initialize(){

	on_launcher();
}

//============================================================================
//**  Launcher window
void MainObject::on_launcher(){
	//TODO there HAS to be a better was than using a flag
	if(launcher_flag == true){
		return;
	}
	LauncherWindow *launcherWindow = new LauncherWindow(this);
    launcherWindow->show();
	launcher_flag = true;
}


//============================================================================
//** Settings Dialog
void MainObject::on_settings(){
	on_settings(0);
}
void MainObject::on_settings(int idx){
	SettingsDialog *dialog = new SettingsDialog(this);
	dialog->tabWidget->setCurrentIndex(idx);
	dialog->exec();
}


//****************************************************************************
//** Map
void MainObject::on_map(){
	//GoogleMapWidget *gmapWidget = new GoogleMapWidget();
   // gmapWidget->show();
}

//============================================================================
//** MpMap
void MainObject::on_mpmap(){
	if(mpMapWidget == 0){
		//qDebug() << "NO mpmap";
		mpMapWidget = new MpMapWidget(this);

	}else{
		//qDebug() << "exists";
	}
	mpMapWidget->show();
	mpMapWidget->setFocus();
	//TODO need to set focus here.
}

//======================================
//** Propertes Browser
void MainObject::on_properties_browser(){
	//propertiesTree = new PropsTreeWidget(this);
	//propertiesTree->show()
	//mpMapWidget->show();
}



//****************************************************************************
//** Quit
void MainObject::on_quit(){
	
    QCoreApplication::instance()->quit();
}


//****************************************************************************
//** Tray Icon
void MainObject::on_tray_icon(QSystemTrayIcon::ActivationReason reason){   
    //* Right click will show the context Menu above system tray
    //* Following will popup menu with single click on Top LEFT ??
    if(reason == QSystemTrayIcon::Trigger){
        QPoint p = QCursor::pos();
        trayIcon->contextMenu()->popup(p);
    }
}



void MainObject::show_setup_wizard(){
	SetupWizard *setupWizard = new SetupWizard(this);
	if(setupWizard->exec()){
		qDebug() << "closed";
		emit(reload_paths());
	}
}

void MainObject::on_view_log(){
	viewLogWidget = new ViewLogWidget(this);
	viewLogWidget->show();
}
