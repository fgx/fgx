#ifndef LAUNCHERWINDOW_H
#define LAUNCHERWINDOW_H

#include <QtGui/QMainWindow>

#include <QtGui/QLabel>
#include <QtGui/QTreeWidget>
#include <QtGui/QTabWidget>
#include <QtGui/QSplitter>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>

#include "xobjects/mainobject.h"

#include "panes/coresettingswidget.h"
#include "panes/timeweatherwidget.h"
#include "aircraft/aircraftwidget.h"
#include "airports/airportswidget.h"
#include "network/networkwidget.h"
#include "panes/advancedoptionswidget.h"
#include "panes/outputpreviewwidget.h"

#include "xwidgets/execontrols.h"

class LauncherWindow : public QMainWindow
{
    Q_OBJECT

public:


    LauncherWindow(MainObject *mainOb, QWidget *parent = 0);
    ~LauncherWindow();

    MainObject *mainObject;

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

	ExeControls *exeFgfs;
	ExeControls *exeTerraSync;
	ExeControls *exeFgCom;

    void closeEvent(QCloseEvent *event);

	QString fg_args();


public slots:


	//* FlightGear
	void on_start_fgfs_clicked();

	//* FgCom
	void on_start_fgcom_clicked();

	//* Terrasync
	void on_start_terrasync_clicked();
	void update_pids();

	//* Actions
	void on_about_fgx();
	void on_about_qt();
	void on_quit();
	void on_action_style(QAction *act);

	//* Functions
	void load_settings();
	void save_settings();
	void initialize();
	bool validate();

	//* Ouput Buttons
	void on_command_preview();
	void on_command_help();


	//8 Widget Events
	void on_tab_changed(int);
	void on_group_box_terrasync_clicked();
};

#endif // LAUNCHERWINDOW_H
