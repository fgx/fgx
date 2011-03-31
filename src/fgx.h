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
	
private:
	Ui::mainWindow *ui;
	XSettings settings;

	AircraftWidget *aircraftWidget;
	NetworkWidget *networkWidget;



	QStringList start_fg_args();
	void start_fg_com();
	void start_terrasync();

private slots:
	void on_fgStart_clicked();

	void on_enableMultiplayer_clicked();
	void on_groupBoxSetTime_clicked();
	void on_useMetar_clicked();
	void on_useCoordinates_clicked();
	void on_useParkPosition_clicked();
	void on_locationIcao_activated();
	void checkCoords();	
	void on_tabs_currentChanged();

	void load_settings();
	void save_settings();


	void on_usecustomScenery_clicked();
	void checkScenery();


	void checkAirportlist();

	void show_settings_dialog();
	void on_buttonFgFsPath_clicked();
	void on_buttonFgRootPath_clicked();

	void on_buttonTest_clicked();

	void on_tabs_currentChanged(int);

	void initialize();
};



#endif
