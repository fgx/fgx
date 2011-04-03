/*
 *  fgx.h
 *  fgx
 *
 *  Created by Yves Sablonier, Zurich
 *  Copyright 2011 --- GPL2
 *
 */

#ifndef FGX_H
#define FGX_H


#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

#include <QtCore/QProcess>

#include <QtGui/QMessageBox>
#include <QtGui/QButtonGroup>
#include <QtGui/QActionGroup>

#include "ui_fgx.h"
#include "xobjects/xsettings.h"

#include "aircraft/aircraftwidget.h"
#include "airports/airportswidget.h"
#include "network/networkwidget.h"


class fgx : public QMainWindow, public Ui::mainWindow
{
	Q_OBJECT
	
protected:
	void closeEvent(QCloseEvent *event);
	
public:

	fgx (QMainWindow *parent = 0);
	~fgx();

	QSqlDatabase db;
	//bool listchecked;
	
	Q_PID pid_fg;
	Q_PID pid_terra;
	Q_PID pid_fgcom;
	void kill_process(QString pid);
	
private:
	Ui::mainWindow *ui;
	XSettings settings;

	AircraftWidget *aircraftWidget;
	AirportsWidget *airportsWidget;
	NetworkWidget *networkWidget;


	QStringList fg_args();
	void start_fgcom();
	void stop_fgcom();
	void start_terrasync();
	void stop_terrasync();
	bool validate();
	QActionGroup *actionGroupStyle;

	void load_joysticks();

private slots:

	//* paths
	void on_buttonFgFsPath_clicked();
	void on_buttonFgRootPath_clicked();

	void on_groupBoxTerraSync_clicked();
	void on_buttonTerraSyncPath_clicked();
	void on_buttonStartTerraSync_clicked();
	void on_buttonStopTerraSync_clicked();

	void on_buttonViewCommand_clicked();
	void on_buttonViewHelp_clicked();

	void on_buttonLoadSettings_clicked();
	void on_buttonSaveSettings_clicked();

	//* Processstart actions
	void on_buttonStartFg_clicked();
	void on_buttonStopFg_clicked();


	void on_groupBoxSetTime_clicked();
	void on_buttonGroupWeather_buttonClicked(int);
	void on_menuStyle_triggered(QAction *act);

	//void on_buttonGroupTime_buttonClicked(int);


	void on_tabs_currentChanged(int);
	void show_settings_dialog();
	void load_settings();
	void save_settings();
	void initialize();


	void on_actionAboutFGX_triggered();
	void on_actionAboutQT_triggered();
	void on_actionQuit_triggered();

};



#endif
