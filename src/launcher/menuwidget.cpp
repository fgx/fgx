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
	
	// Macro needed to make the menubar disappear on osx
	// Not nice.
	if(!mainObject->runningOs() == MainObject::MAC){
		setLayout(menuLayout);
	}
	
	quitAction = new QAction(tr("&Quit"), this);
	quitAction->setShortcuts(QKeySequence::Quit);
	quitAction->setStatusTip(tr("Exit the application"));
	connect(quitAction, SIGNAL(triggered()), this, SLOT(on_menu_quit()));
	
	
	applicationMenu = new QMenu(tr("&Launcher")); // tr needed at least for osx!
	
	applicationMenu->addAction(quitAction);
	
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