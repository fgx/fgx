#ifndef LAUNCHERWINDOW_H
#define LAUNCHERWINDOW_H

#include <QtGui/QMainWindow>



#include <QtGui/QLabel>
#include <QtGui/QTreeWidget>
#include <QtGui/QTabWidget>
#include <QtGui/QSplitter>

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

	enum COLS{
		C_ARG = 0,
		C_VAL = 1
	};

    LauncherWindow(MainObject *mainOb, QWidget *parent = 0);
    ~LauncherWindow();

    MainObject *mainObject;

    QLabel *headerLabel;
	QSplitter *splitter;
    QTabWidget *tabWidget;
	//QTreeWidget *tree;

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
	void setup_tree();

	QString fg_args();


public slots:
	void on_tab_changed(int);
	void set_arg(QString action, QString arg, QString val);
	void set_paths();



	//* FlightGear
	void on_start_fgfs_clicked();

	//** FgCom
	void on_start_fgcom_clicked();


	void on_start_terrasync_clicked();
};

#endif // LAUNCHERWINDOW_H
