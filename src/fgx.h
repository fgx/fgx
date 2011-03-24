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
#include <QSettings>

#include "ui_fgx.h"

class fgx : public QMainWindow, public Ui::mainWindow{
	Q_OBJECT
	
protected:
	void closeEvent(QCloseEvent *event);
	
public:
	fgx (QMainWindow *parent = 0);
	~fgx();
	bool listchecked;
	

	
private:
	Ui::mainWindow *ui;
	QProcess ps;
	QProcess *fgProcess;
	
	private slots:
	void on_fgStart_clicked();
	void on_set_fgdata_path_Button_clicked();
	void checkFGFS();
	void on_useFGXfgfs_clicked();
	void on_enableMultiplayer_clicked();
	void on_setTime_clicked();
	void on_useMetar_clicked();
	void on_useCoordinates_clicked();
	void on_useParkPosition_clicked();
	void on_locationIcao_activated();
	void checkCoords();	
	void checkScenery();
	void readSettings();
	void writeSettings();
	void on_tabs_currentChanged();
	void on_usecustomScenery_clicked();
	void on_airCraft_activated();
	void checkAirportlist();
	void checkAircraftImage();
	void checkAircraftListStartup();
	void checkAircraftList();
	
};



#endif