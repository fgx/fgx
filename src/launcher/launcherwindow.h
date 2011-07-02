#ifndef LAUNCHERWINDOW_H
#define LAUNCHERWINDOW_H

#include <QtGui/QWidget>

#include <QtGui/QTabWidget>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QPushButton>

#include "panes/coresettingswidget.h"
#include "panes/timeweatherwidget.h"
#include "aircraft/aircraftwidget.h"
#include "airports/airportswidget.h"
#include "network/networkwidget.h"
#include "panes/advancedoptionswidget.h"
#include "panes/outputpreviewwidget.h"


#include "xwidgets/execontrols.h"
#include "utilities/statusbar.h"
#include "utilities/messagebox.h"

#include "xobjects/mainobject.h"



//= Foward Declaratins
class MainObject;
class NetworkWidget;
class CoreSettingsWidget;
class TimeWeatherWidget;
class AircraftWidget;
class AirportsWidget;
class NetworkWidget;
class AdvancedOptionsWidget;
class OutputPreviewWidget;




class LauncherWindow : public QWidget
{
    Q_OBJECT

public:

    LauncherWindow(MainObject *mainOb, QWidget *parent = 0);
    ~LauncherWindow();

    MainObject *mainObject;
	bool initializing;

	QActionGroup *actionGroupStyle;
    QLabel *headerLabel;
    QTabWidget *tabWidget;

	CoreSettingsWidget *coreSettingsWidget;
	TimeWeatherWidget *timeWeatherWidget;
	AircraftWidget *aircraftWidget;
	AirportsWidget *airportsWidget;
	NetworkWidget *networkWidget;
	AdvancedOptionsWidget *advancedOptionsWidget;
	OutputPreviewWidget *outputPreviewWidget;

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
	void on_action_style(QAction *act);
	void on_action_open_url(QAction *act);

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
