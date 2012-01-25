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
	
	menuLayout = new QHBoxLayout;
	
	// Macro needed to make the widget "placeholder" disappearing on OSX.
	// Not nice at all, but working.
	if(MainObject::runningOs() != MainObject::MAC){
		setLayout(menuLayout);
	}
	
	quitAction = new QAction(tr("&Quit"), this);
	quitAction->setShortcuts(QKeySequence::Quit);
	quitAction->setStatusTip(tr("Exit the application"));
	connect(quitAction, SIGNAL(triggered()), this, SLOT(on_menu_quit()));
	
	logWindowAction = new QAction(tr("&Show logs"), this);
	logWindowAction->setShortcut(QString("Ctrl+L"));
	logWindowAction->setStatusTip(tr("Shows all logs in a separate window"));
	connect(logWindowAction, SIGNAL(triggered()), this, SLOT(on_show_log_window()));
	
	debugWindowAction = new QAction(tr("&Debug settings"), this);
	debugWindowAction->setShortcut(QString("Ctrl+D"));
	debugWindowAction->setStatusTip(tr("Shows the settings debug window"));
	connect(debugWindowAction, SIGNAL(triggered()), this, SLOT(on_show_debug_window()));
	
	propsWindowAction = new QAction(tr("&Properties browser"), this);
	propsWindowAction->setShortcut(QString("Ctrl+Shift+P"));
	propsWindowAction->setStatusTip(tr("Shows properties browser"));
	connect(propsWindowAction, SIGNAL(triggered()), this, SLOT(on_show_props_window()));
	
	
	if(MainObject::runningOs() == MainObject::MAC){
		applicationMenu = new QMenu(tr("&Debug"));
	}else {
		applicationMenu = new QMenu(tr("&FGx"));
	}

	
	applicationMenu->addAction(quitAction);
	applicationMenu->addAction(logWindowAction);
	applicationMenu->addAction(debugWindowAction);
	applicationMenu->addAction(propsWindowAction);
	
	// 0 is needed for OSX using the wrapper, see qt4 MenuBar doc
	mainMenu = new QMenuBar(0);
	
	// Adding the menus to the MenuBar
	mainMenu->addMenu(applicationMenu);
	
	// No action for OSX, but gives the menu for x/win
	menuLayout->addWidget(mainMenu, 0);
	

}

void MenuWidget::on_menu_quit(){
	// does this really stop all?
	mainObject->stop_all();
	
	// real quit, not window close only
	QApplication::quit();
}

void MenuWidget::on_show_log_window(){
	mainObject->viewLogsWidget->show();
	mainObject->viewLogsWidget->raise();
	mainObject->viewLogsWidget->activateWindow();
	
}

void MenuWidget::on_show_debug_window(){
	mainObject->fgxDebugWidget->show();
	mainObject->fgxDebugWidget->raise();
	mainObject->fgxDebugWidget->activateWindow();
	
}

void MenuWidget::on_show_props_window(){
	mainObject->propertiesBrowser->show();
	mainObject->propertiesBrowser->raise();
	mainObject->propertiesBrowser->activateWindow();
	
}