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

#include <QProcess>
#include <QMessageBox>
#include <QButtonGroup>

#include "ui_fgx.h"
#include "xobjects/xsettings.h"

#include "aircraft/aircraftwidget.h"
#include "airports/airportswidget.h"
#include "network/networkwidget.h"


class fgx : public QMainWindow, public Ui::mainWindow{
	Q_OBJECT
	
protected:
	void closeEvent(QCloseEvent *event);
	
public:
	enum METAR{
		METAR_NONE = 0,
		METAR_LIVE = 1,
		METAR_EDIT = 2
	};
	enum LOG{
		LOG_WARN = 0,
		LOG_INFO = 1,
		LOG_DEBUG = 2,
		LOG_BULK = 3,
		LOG_ALERT = 4,
	};

	fgx (QMainWindow *parent = 0);
	~fgx();
	bool listchecked;
	
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

	QButtonGroup *buttonGroupTime;
	QButtonGroup *buttonGroupWeather;
	QButtonGroup *buttonGroupLog;

	QStringList fg_args();
	void start_fgcom();
	void stop_fgcom();
	void start_terrasync();
	void stop_terrasync();



private slots:


	void on_buttonFgFsPath_clicked();
	void on_buttonFgRootPath_clicked();


	void on_buttonStartFg_clicked();
	void on_buttonStopFg_clicked();
	void on_buttonTest_clicked();

	void on_groupBoxSetTime_clicked();
	void on_weather_selected();


	void on_tabs_currentChanged(int);
	void show_settings_dialog();
	void load_settings();
	void save_settings();
	void initialize();




};



#endif
