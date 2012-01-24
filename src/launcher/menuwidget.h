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
	XSettings *settings;

	QHBoxLayout *menuLayout;
	QMenuBar *mainMenu;
	
	// Application menus
	
	QMenu *applicationMenu;
	QMenu *helpMenu;
	
	QAction *quitAction;
	QAction *helpAction;
	


signals:
	

public slots:
	void quit();


};

#endif // XMENUWIDGET_H
