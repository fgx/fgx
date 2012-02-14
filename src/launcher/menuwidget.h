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
	QMenu *profileMenu;
	
	QMenu *helpMenu;
	QMenu *helpFlightGearGroup;
	QMenu *helpFGxGroup;
	
	// Menu Actions
	QAction *quitAction;
	QAction *logWindowAction;
	QAction *debugWindowAction;
	QAction *propsWindowAction;
	
	QAction *debugmodeAction;
	
	QAction *loadProfileAction;
	QAction *saveProfileAction;
	QAction *resetProfileAction;
	QAction *defaultProfileAction;
	
	QAction *urlActionWiki;
	QAction *urlActionForums;
	QAction *urlActionFGxUserHelp;

signals:
	void on_debug_mode(bool enabled);

public slots:
	void on_menu_quit();
	void on_show_log_window();
	void on_show_debug_window();
	void on_show_props_window();
	
	void on_menu_debug_mode();
	
	void on_menu_load_profile();
	void on_menu_save_profile();
	void on_menu_reset_profile();
	void on_menu_default_profile();
	
	void on_menu_url_wiki();
	void on_menu_url_forums();
	void on_menu_fgx_user_forums();

};

#endif // XMENUWIDGET_H
