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
	//debug_mode = mainObject->X->get_ena("DEBUG_MODE");
	
	menuLayout = new QHBoxLayout(this);
	
	
	helpMenu = new QMenu("&Help");
	//applicationMenu = new QMenuBar(0);
	actionLauncher = new QAction(this);
	actionLauncher->setText(tr("&Show Launcher Window"));
	actionLauncher->setShortcut(tr("Ctrl+L"));
	//connect(actionLauncher, SIGNAL(triggered()), this, SLOT(on_launcher()) );
	
	
	actionDebugMode = new QAction(this);
	actionDebugMode->setText(tr("&Debug Mode"));
	actionDebugMode->setShortcut(tr("Ctrl+D"));
	actionDebugMode->setCheckable(true);
	//actionDebugMode->setChecked(debug_mode);
	//connect(actionDebugMode, SIGNAL(triggered()), this, SLOT(set_debug_mode()));
	
	
	loadProfileAction = new QAction("&Load Profile", this);
	loadProfileAction->setShortcut(tr("Ctrl+L"));
	//connect(loadProfileAction, SIGNAL(triggered()), this, SLOT(new_()));
	
	saveProfileAction = new QAction("&Save Profile", this);
	saveProfileAction->setShortcut(tr("Ctrl+S"));
	//connect(loadProfileAction, SIGNAL(triggered()), this, SLOT(new_()));
	
	windowsMenu = new QMenu ("&Windows");
	debugMenu = new QMenu ("&Debug");
	profileMenu = new QMenu ("&Profiles");
	
	windowsMenu->addAction(actionLauncher);
	
	profileMenu->addAction(loadProfileAction);
	profileMenu->addAction(saveProfileAction);
	
	debugMenu->addAction(actionDebugMode);
	
	
	// This is needed for OSX, see qt4 MenuBar doc
	applicationMenu = new QMenuBar(0);
	
	applicationMenu->addMenu(windowsMenu);
	applicationMenu->addMenu(debugMenu);
	applicationMenu->addMenu(profileMenu);
	
	//applicationMenu->addMenu(helpMenu);
	
	menuLayout->addWidget(applicationMenu, 0);
	

}