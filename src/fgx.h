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

#include "ui_fgx.h"

#include "xobjects/xsettings.h"
#include "network/networkwidget.h"
#include "aircraft/aircraftwidget.h"



class fgx : public QMainWindow, public Ui::mainWindow{
	Q_OBJECT
	
protected:
	void closeEvent(QCloseEvent *event);
	
public:
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
	NetworkWidget *networkWidget;



	QStringList fg_args();
	void start_fgcom();
	void stop_fgcom();
	void start_terrasync();
	void stop_terrasync();



private slots:

	void show_settings_dialog();
	void on_buttonFgFsPath_clicked();
	void on_buttonFgRootPath_clicked();


	void on_buttonStartFg_clicked();
	void on_buttonStopFg_clicked();

	void on_enableMultiplayer_clicked();
	void on_groupBoxSetTime_clicked();
	void on_useMetar_clicked();

	void on_useParkPosition_clicked();
	void on_locationIcao_activated();

	void on_checkBoxUseCoordinates_clicked();

	void on_tabs_currentChanged();

	void load_settings();
	void save_settings();


	void on_usecustomScenery_clicked();
	void checkScenery();


	void checkAirportlist();




	void on_buttonTest_clicked();

	void on_tabs_currentChanged(int);

	void initialize();
};



#endif
