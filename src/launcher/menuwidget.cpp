#include "app_config.h"
#include <QtDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>

#include "utilities/utilities.h"
#include "launcher/menuwidget.h"
#include "xobjects/mainobject.h"


MenuWidget::MenuWidget(MainObject *mob, QWidget *parent) :
	QWidget(parent)
{

	mainObject = mob;
	
	settings = new XSettings();
	debug_mode = settings->value("DEBUG_MODE","0").toBool();
	
	menuLayout = new QHBoxLayout;
	
	// Macro needed to make the widget "placeholder" disappearing on OSX.
	// Not nice at all, but working.
	if(MainObject::runningOs() != MainObject::MAC){
		setLayout(menuLayout);
	}
	
	// Menu "FGx" (OSX = "Windows") actions
	quitAction = new QAction(tr("&Quit"), this);
	quitAction->setShortcut(QString("Ctrl+q"));
	quitAction->setStatusTip(tr("Exit the application"));
	connect(quitAction, SIGNAL(triggered()), this, SLOT(on_menu_quit()));
	
	logWindowAction = new QAction(tr("Show &logs"), this);
	logWindowAction->setShortcut(QString("Ctrl+Shift+l"));
	logWindowAction->setStatusTip(tr("Shows all logs in a separate window"));
	connect(logWindowAction, SIGNAL(triggered()), this, SLOT(on_show_log_window()));
	
	debugWindowAction = new QAction(tr("&Debug settings"), this);
	debugWindowAction->setShortcut(QString("Ctrl+d"));
	debugWindowAction->setStatusTip(tr("Shows the settings debug window"));
	connect(debugWindowAction, SIGNAL(triggered()), this, SLOT(on_show_debug_window()));
	
	propsWindowAction = new QAction(tr("&Properties browser"), this);
	propsWindowAction->setShortcut(QString("Ctrl+Shift+p"));
	propsWindowAction->setStatusTip(tr("Shows properties browser"));
	connect(propsWindowAction, SIGNAL(triggered()), this, SLOT(on_show_props_window()));
	
	// Menu "Mode" actions
	debugmodeAction = new QAction(tr("&Debug mode"), this);
	debugmodeAction->setShortcut(QString("Ctrl+Shift+d"));
	debugmodeAction->setStatusTip(tr("Switching debug mode"));
	debugmodeAction->setCheckable(true);
	debugmodeAction->setChecked(debug_mode);
	connect(debugmodeAction, SIGNAL(triggered()), this, SLOT(on_menu_debug_mode()));
	
	// Switching menu name, maybe should better go to two menus later
	if(MainObject::runningOs() == MainObject::MAC){
		applicationMenu = new QMenu(tr("&Windows"));
	}else {
		applicationMenu = new QMenu(tr("&FGx"));
	}
	
	modeMenu = new QMenu(tr("&Mode"));

	// Add actions to menu "FGx"/"Windows" (OSX = "Windows")
	applicationMenu->addAction(quitAction);
	applicationMenu->addAction(logWindowAction);
	applicationMenu->addAction(debugWindowAction);
	applicationMenu->addAction(propsWindowAction);
	
	// Add actions to menu "Mode"
	modeMenu->addAction(debugmodeAction);
	
	// Create menubar, parentless 0 is needed for OSX using the wrapper for
	// getting native OSX menus, see qt4 MenuBar doc
	mainMenu = new QMenuBar(0);
	
	// Adding the menus to the MenuBar
	mainMenu->addMenu(applicationMenu);
	mainMenu->addMenu(modeMenu);
	
	// No action for OSX, but gives the menu for x/win
	menuLayout->addWidget(mainMenu, 0);

}

// Quit application
void MenuWidget::on_menu_quit(){
	mainObject->stop_all();
	QApplication::quit();
}

// Show log window, focus
void MenuWidget::on_show_log_window(){
	mainObject->viewLogsWidget->show();
	mainObject->viewLogsWidget->raise();
	mainObject->viewLogsWidget->activateWindow();
	
}

// Show debug window, focus
void MenuWidget::on_show_debug_window(){
	mainObject->fgxDebugWidget->show();
	mainObject->fgxDebugWidget->raise();
	mainObject->fgxDebugWidget->activateWindow();
	
}

// Show properties window, focus
void MenuWidget::on_show_props_window(){
	mainObject->propertiesBrowser->show();
	mainObject->propertiesBrowser->raise();
	mainObject->propertiesBrowser->activateWindow();
	
}

// Set application to debug mode (show all exe buttons)
void MenuWidget::on_menu_debug_mode(){
	debug_mode = debugmodeAction->isChecked();
	mainObject->set_debug_mode(debug_mode);

}