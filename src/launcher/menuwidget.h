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
	
	XSettings *settings;
	bool debug_mode;

	MainObject *mainObject;

	// Some kind of placeholder
	QHBoxLayout *menuLayout;
	
	// Menubar
	QMenuBar *mainMenu;
	
	// Menus
	QMenu *applicationMenu;
	QMenu *modeMenu;
	
	// Menu Actions
	QAction *quitAction;
	QAction *logWindowAction;
	QAction *debugWindowAction;
	QAction *propsWindowAction;
	
	QAction *debugmodeAction;

signals:
	void on_debug_mode(bool enabled);

public slots:
	void on_menu_quit();
	void on_show_log_window();
	void on_show_debug_window();
	void on_show_props_window();
	
	void on_menu_debug_mode();

};

#endif // XMENUWIDGET_H
