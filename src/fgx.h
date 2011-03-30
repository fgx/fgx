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

#include "xobjects/xsettings.h"

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
	XSettings settings;
	
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
	void on_tabs_currentChanged();

	void readSettings();
	void writeSettings();


	void on_usecustomScenery_clicked();
	void checkScenery();

	void on_airCraft_activated();
	void checkAircraftImage();
	void checkAircraftListStartup();
	void checkAircraftList();

	void checkAirportlist();

	void on_buttonTest_clicked();
};



#endif
