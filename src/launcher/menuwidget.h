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


#include "xobjects/mainobject.h"
class MainObject;


class MenuWidget : public QWidget
{
Q_OBJECT
public:
	explicit MenuWidget(MainObject *mob, QWidget *parent = 0);
	
	bool debug_mode;

	MainObject *mainObject;

	QHBoxLayout *menuLayout;
	QMenuBar *applicationMenu;
	QMenu *helpMenu;
	
	// Application menus
	
	QMenu *windowsMenu;
	QMenu *debugMenu;
	QMenu *profileMenu;
	
	QAction *actionLauncher;
	QAction *actionDebugMode;
	QAction *loadProfileAction;
	QAction *saveProfileAction;


signals:
	void on_debug_mode(bool enabled);

public slots:
	
	
	/*void set_debug_mode();
	void on_launcher();
	void on_browsermap();
	void on_properties_browser();*/


};

#endif // XMENUWIDGET_H
