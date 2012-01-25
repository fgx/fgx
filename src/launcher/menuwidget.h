#ifndef XMENUWIDGET_H
#define XMENUWIDGET_H

#include <QTimer>


#include <QWidget>
#include <QLabel>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QLabel>
#include <QtGui/QWidgetAction>

#include "xwidgets/fgxdebugwidget.h"

#include "xobjects/mainobject.h"
class MainObject;


class MenuWidget : public QWidget
{
Q_OBJECT
public:
	explicit MenuWidget(MainObject *mob, QWidget *parent = 0);
	
	bool debug_mode;

	MainObject *mainObject;

	// Some kind of placeholder
	QHBoxLayout *menuLayout;
	
	// Menubar
	QMenuBar *mainMenu;
	
	// Menus
	QMenu *applicationMenu;
	
	// Actions
	QAction *quitAction;
	


signals:
	

public slots:
	void on_menu_quit();


};

#endif // XMENUWIDGET_H
