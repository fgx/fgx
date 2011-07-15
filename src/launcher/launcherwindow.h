#ifndef LAUNCHERWINDOW_H
#define LAUNCHERWINDOW_H

#include <QtGui/QWidget>

#include <QtGui/QTabWidget>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QPushButton>


//++ Help
//= because of recursive inputs please pleace forwards here, if you are a sausage make or eat hot tiguuillea..

#include "panes/coresettingswidget.h"
class CoreSettingsWidget;

#include "panes/timeweatherwidget.h"
class TimeWeatherWidget;

#include "aircraft/aircraftwidget.h"
class AircraftWidget;

#include "airports/airportswidget.h"
class AirportsWidget;

#include "network/networkwidget.h"
class NetworkWidget;

#include "panes/expertoptionswidget.h"
class ExpertOptionsWidget;

#include "xwidgets/execontrols.h"
class ExeControls;

#include "launcher/headerwidget.h"
class HeaderWidget;

#include "utilities/statusbar.h"

#include "utilities/messagebox.h"

#include "xobjects/mainobject.h"
#include "xwidgets/xgroupboxes.h"




class LauncherWindow : public QWidget
{
    Q_OBJECT

public:

    LauncherWindow(MainObject *mainOb, QWidget *parent = 0);
	
    ~LauncherWindow();

    MainObject *mainObject;
	bool initializing;

	HeaderWidget *headerWidget;
	
	XGroupHBox *toolBox;
	
	QToolButton *buttonWhatsThis;

	QActionGroup *actionGroupStyle;
    QTabWidget *tabWidget;

	CoreSettingsWidget *coreSettingsWidget;
	TimeWeatherWidget *timeWeatherWidget;
	AircraftWidget *aircraftWidget;
	AirportsWidget *airportsWidget;
	NetworkWidget *networkWidget;
	ExpertOptionsWidget *expertOptionsWidget;

	QPushButton *buttonLoadSettings;
	QPushButton *buttonSaveSettings;

	ExeControls *exeAll;
	ExeControls *exeFgfs;
	ExeControls *exeTerraSync;
	ExeControls *exeFgCom;

	StatusBar *statusBar;
	MessageBox *messageBox;

    void closeEvent(QCloseEvent *event);

public slots:


	//= Start/Stop
	void on_start_all_clicked();
	void on_start_fgfs_clicked();
	void on_start_terrasync_clicked();
	void on_start_fgcom_clicked();


	//* Actions
	void on_about_fgx();
	void on_about_qt();
	void on_quit();
	//void on_action_style(QAction *act);
	void on_action_open_url(QAction *act);
	void on_whats_this();

	//* Functions
	void load_settings();
	void save_settings();
	void initialize();
	bool validate();


	//* Ouput Buttons
	void on_command_preview();


	//* Widget Events
	void on_tab_changed(int);


};

#endif // LAUNCHERWINDOW_H
