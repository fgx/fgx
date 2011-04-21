/*
 *  fgx.cpp
 *  part of the fgx project
 *
 *  Created by Yves Sablonier, Zurich
 *  Adapted by pedro in Wales - pete at freflightsim dot org
 *  Copyright 2011 --- GPL2
 *
 */

#include <QApplication>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QTextStream>
#include <QtCore/QTimer>
#include <QtCore/QUrl>


#include <QtGui/QStyleFactory>
#include <QtGui/QFileDialog>
#include <QtGui/QDesktopServices>


#include "fgx.h"
#include "settings/settingsdialog.h"


fgx::fgx(QMainWindow *parent) : QMainWindow(parent){

	setupUi(this);

	setProperty("settings_namespace", QVariant("launcher_window"));
	settings.restoreWindow(this);

	setWindowTitle(QCoreApplication::applicationName().append(" - ").append(QCoreApplication::applicationVersion()));
	fgx_logo->setText(QCoreApplication::applicationVersion());

	//===================================================================
	//** Setup Styles - TODO make defult style from ptatform.. in settings. (pedro)
	//* This adds a menu Style items and attempts to set default or setting
	QApplication::setStyle( QStyleFactory::create(settings.value("gui_style","Macintosh (aqua)").toString()) );
	actionGroupStyle = new QActionGroup(this);
	actionGroupStyle->setExclusive(true);
	QStringList styles =  QStyleFactory::keys();
	for(int idx=0; idx < styles.count(); idx++){
		QString sty = QString(styles.at(idx));
		QAction *act = menuStyle->addAction( sty );
		actionGroupStyle->addAction( act );
		act->setCheckable(true);
		if(QApplication::style()->objectName() == sty.toLower()){
			act->setChecked(true);
		}
	}


	//========================================================================================
	// These are "hardcoded" widgets inserted into the tabWidget in "ui" MainWindow
	//========================================================================================

	//** Core Settings
	//coreSettingsWidget = new CoreSettingsWidget(this);
	//tabs->addTab(coreSettingsWidget, tr("Core Settings"));
	//connect(coreSettingsWidget->groupBoxTerraSync, SIGNAL(clicked()), this, SLOT(on_group_box_terrasync_clicked()));

	//** Time / Weather Widget
	timeWeatherWidget = new TimeWeatherWidget(this);
	tabs->addTab(timeWeatherWidget, tr("Time and Weather"));

	//** Aircraft tab
	//aircraftWidget = new AircraftWidget(mainObject);
	//tabs->addTab( aircraftWidget, tr("Aircraft"));

	//** Airports Tab
	airportsWidget = new AirportsWidget(this);
	tabs->addTab(  airportsWidget, tr("Airports"));

	//** Network Tab
	networkWidget = new NetworkWidget(this);
	tabs->addTab( networkWidget, tr("Network"));

	//** Advanced Options
	advancedOptionsWidget = new AdvancedOptionsWidget(this);
	tabs->addTab( advancedOptionsWidget, tr("Advanced Options"));

	//** Output + Preview
	outputPreviewWidget = new OutputPreviewWidget(this);
	tabs->addTab( outputPreviewWidget, tr("Output / Preview"));
	connect(outputPreviewWidget->buttonCommandPreview, SIGNAL(clicked()), this, SLOT(on_buttonCommandPreview_clicked()));
	connect(outputPreviewWidget->buttonCommandHelp, SIGNAL(clicked()), this, SLOT(on_buttonCommandHelp_clicked()));



	//========================================================================================
	//** Insert the "Controls" into the bottom Bar
	//========================================================================================
	exeFgCom = new ExeControls("FgCom", "fgcom");
	bottonActionLayout->addWidget(exeFgCom);
	connect(	exeFgCom->buttonStart, SIGNAL(clicked()),
				this, SLOT(on_start_fgcom_clicked())
	);

	exeTerraSync = new ExeControls("TerraSync", "terrasync");
	bottonActionLayout->addWidget(exeTerraSync);
	connect(	exeTerraSync->buttonStart, SIGNAL(clicked()),
				this, SLOT(on_start_terrasync_clicked())
	);

	exeFgfs = new ExeControls("FlightGear", "fgfs");
	bottonActionLayout->addWidget(exeFgfs);
	connect(	exeFgfs->buttonStart, SIGNAL(clicked()),
				this, SLOT(on_start_fgfs_clicked())
	);


	//=================================================
	//** Restore Last tab
	tabs->setCurrentIndex( settings.value("last_tab").toInt() );

	buttonSaveSettings->setVisible(settings._dev_mode());
	buttonLoadSettings->setVisible(settings._dev_mode());

	//====================================================================================
	//* Problem:  Qt Has no "Show event" for a "widget", so we need to present Widgets first
	//** and then initialise. This is achieved with a timer that triggers in a moment

	// TODO  - disable widget till sane in initialize()
	//centralWidget()->setDisabled(true);
	QTimer::singleShot(500, this, SLOT(initialize()));



}/* end constructor */

fgx::~fgx(){
}


//=======================================================================================================================
// initialize and  Setup
//=======================================================================================================================
void fgx::initialize(){

	//** First load the settings, and check the "paths" to fg_root and fg are sane
	load_settings();
	if(!settings.paths_sane()){
		coreSettingsWidget->show_settings_dialog();
	}

	//** Paths are sane so we can initialize;
	//TODO setup wizard to import data first time
	aircraftWidget->initialize();
	airportsWidget->initialize();
	coreSettingsWidget->initialize();
	update_pids();

	centralWidget()->setDisabled(false);
}

//=======================================================================================================================
// Updates as the external processes in the "command buttons"
//=======================================================================================================================
void fgx::update_pids(){
	if (exeFgCom->isEnabled()){
		exeFgCom->update_pid();
	}
	if (exeTerraSync->isEnabled()){
		exeTerraSync->update_pid();
	}
	if (exeFgfs->isEnabled()){
		exeFgfs->update_pid();
	}
}


//=======================================================================================================================
// Start FlightGear
//=======================================================================================================================
void fgx::on_start_fgfs_clicked() {

	QString command = settings.fgfs_path();
	//##QStringList arguments = fg_args();

	QString command_line = QString(command).append(" ").append(fg_args());
	outputPreviewWidget->txtPreviewOutput->setPlainText(command_line);

	if(coreSettingsWidget->checkBoxShowMpMap->isChecked()){
		QUrl mapUrl(coreSettingsWidget->comboMpMapServer->itemData(coreSettingsWidget->comboMpMapServer->currentIndex()).toString());
		mapUrl.addQueryItem("follow", networkWidget->txtCallSign->text());
		QDesktopServices::openUrl(mapUrl);
	}
	// qDebug() << command_line;
	exeFgfs->start(command_line);
}

//=======================================================================================================================
// Start Terrasync
//=======================================================================================================================
void fgx::on_start_terrasync_clicked(){

	QStringList args;
	args << settings.terrasync_exe_path() << "-p" << "5500" << "-S" << "-d" << settings.terrasync_sync_path();
	QString command_line("nice");
	command_line.append(" ").append(args.join(" "));
	// qDebug() << command_line;
	exeTerraSync->start(command_line);
}
//=======================================================================================================================
// Start FGCom
//=======================================================================================================================
void fgx::on_start_fgcom_clicked(){
	QString command_line = settings.fgcom_exe_path();
	command_line.append(" ");
	QStringList args;
	args << settings.value("fgcom_no").toString() << settings.value("fgcom_port").toString();
	command_line.append( args.join(" ") );
	// qDebug() << command_line;
	exeFgCom->start(command_line);
}


//=======================================================================================================================
//* Validate
//=======================================================================================================================
bool fgx::validate(){
	int TIMEOUT = 5000;
	QString v;

	v = aircraftWidget->validate();
	if(v != ""){
		tabs->setCurrentIndex( tabs->indexOf(aircraftWidget));
		statusBar()->showMessage(v, TIMEOUT);
		return false;
	}

	v = airportsWidget->validate();
	if(v != ""){
		tabs->setCurrentIndex( tabs->indexOf(airportsWidget));
		statusBar()->showMessage(v, TIMEOUT);
		return false;
	}

	v = networkWidget->validate();
	if(v != ""){
		tabs->setCurrentIndex( tabs->indexOf(networkWidget));
		statusBar()->showMessage(v, TIMEOUT);
		return false;
	}

	if(coreSettingsWidget->groupBoxTerraSync->isChecked() and coreSettingsWidget->txtTerraSyncPath->text().length() == 0){
		tabs->setCurrentIndex( tabs->indexOf(coreSettingsWidget) );
		coreSettingsWidget->txtTerraSyncPath->setFocus();
		statusBar()->showMessage("Need a Terrasync directory", TIMEOUT);
		return false;
	}
	return true;
}

//=======================================================================================================================
// FlightGear Command Args
//=======================================================================================================================
/* This function return the "command-line=args --as-a=string near end
   However all the args are added as a string list until join() near end
   +++ WARNING >>>
   * the QStringLIst args = Please make sure there are no spaces in the args.
   * any args with spaces are quoted eg " --foo=bar" instead of --foo=bar
   -- end Warning <<<
*/
QString fgx::fg_args(){

	//** This is unused atmo ??
	/*
	QString argtime;
	argtime.append(year->text());
	argtime.append(":");
	argtime.append(month->text());
	argtime.append(":");
	argtime.append(day->text());
	argtime.append(":");
	argtime.append(hour->text());
	argtime.append(":");
	argtime.append(minute->text());
	argtime.append(":");
	argtime.append(second->text());
	*/


	QStringList args;

	//** fg_root
	args << QString("--fg-root=").append(settings.fg_root());





	//** Scenery Path
	args << QString("--fg-scenery=%1").arg(settings.scenery_path());



	//** Time Of Day
	// TODO
	//if (groupBoxSetTime->isChecked()) {
		//content.append("--start-date-lat=");
		//content.append(argtime);
	//} else {
		//* replaces "Dawn" with "dawn", and "Real Time" with "realtime" as a hack
		//args << QString("--timeofday=").append( timeWeatherWidget->buttonGroupTime->checkedButton()->property("value").toString() );
	//}

	//** Core Settings
	args << coreSettingsWidget->get_args();


	//* Weather/Metar fetch
	QString metar = timeWeatherWidget->buttonGroupMetar->checkedButton()->property("value").toString();
	if(metar == "live") {
		//* Enable real weather
		args << QString("--enable-real-weather-fetch");

	}else if(metar == "custom"){
		//* Use metar string ? do we need to parse ?
		args << QString("--metar=").append("\"").append(timeWeatherWidget->txtMetar->toPlainText()).append("\"");

	}else{
		args << QString("--disable-real-weather-fetch");
	}


	//** Aircraft
	args << aircraftWidget->get_args();

	//** Airport, Runway Start pos
	args << airportsWidget->get_args();

	//** Network
	args << networkWidget->get_args();

	//** Advanced Options
	args << advancedOptionsWidget->get_args();

	//* Ai Traffic TODO
	/*
	if (enableAITraffic->isChecked()) {
		args << QString("--enable-ai-traffic");
	}else{
		args << QString("--disable-ai-traffic");
	}
	*/
	//** Enable AI models ???
	args << QString("--enable-ai-models");

	args.sort();

	//*  Additonal args in text box..
	/*
	QString extra = lineEditExtraArgs->toPlainText().trimmed();
	if (extra.length() > 0) {
		QStringList parts = extra.split("\n");
		if(parts.count() > 0){
			for(int i=0; i < parts.count(); i++){
				QString part = parts.at(i).trimmed();
				if(part.length() > 0){
					args << part;
				}
			}
		}
	}
	*/

	//** Create the return string
	QString args_string = args.join(" ");


	return args_string;
}






//================================================================================
// Save Settings
//================================================================================
void fgx::on_buttonSaveSettings_clicked(){
	save_settings();
}

void fgx::save_settings()
{
	//## NB: fgfs path and FG_ROOT are saves in SettingsDialog ##
	coreSettingsWidget->save_settings();
	aircraftWidget->save_settings();
	airportsWidget->save_settings();
	networkWidget->save_settings();
	advancedOptionsWidget->save_settings();




	/* TODO
	settings.setValue("year", year->text());
	settings.setValue("month", month->text());
	settings.setValue("day", day->text());
	settings.setValue("hour", hour->text());
	settings.setValue("minute", minute->text());
	settings.setValue("second", second->text());
	*/

	//* Time
	settings.setValue("timeofday", timeWeatherWidget->buttonGroupTime->checkedButton()->property("value").toString());
	//settings.setValue("set_time", groupBoxSetTime->isChecked());

	
	//* Weather
	settings.setValue("weather", timeWeatherWidget->buttonGroupMetar->checkedId());
	settings.setValue("metar", timeWeatherWidget->txtMetar->toPlainText());
	
	//* Advanced

	settings.sync();
}

//================================================================================
// Load Settings
//================================================================================
void fgx::on_buttonLoadSettings_clicked(){
	load_settings();
}

void fgx::load_settings()
{



	coreSettingsWidget->load_settings();
	aircraftWidget->load_settings();
	airportsWidget->load_settings();
	networkWidget->load_settings();
	advancedOptionsWidget->load_settings();
	

	exeTerraSync->setEnabled( settings.value("use_terrasync").toBool() );



	
	//** Time Of Day - TODO
	/*
	bool setTime = settings.value("set_time").toBool();
	groupBoxSetTime->setChecked(setTime);

	QString yearSet = settings.value("year").toString();
	year->setText(yearSet);
	QString monthSet = settings.value("month").toString();
	month->setText(monthSet);
	QString daySet = settings.value("day").toString();
	day->setText(daySet);
	QString hourSet = settings.value("hour").toString();
	hour->setText(hourSet);
	QString minuteSet = settings.value("minute").toString();
	minute->setText(minuteSet);
	QString secondSet = settings.value("second").toString();
	second->setText(secondSet);
	*/

	QString tod = settings.value("timeofday", "real").toString();
	QList<QAbstractButton *> todButtons = timeWeatherWidget->buttonGroupTime->buttons();
	for (int i = 0; i < todButtons.size(); ++i) {
		if(todButtons.at(i)->property("value").toString() == tod){
			todButtons.at(i)->setChecked(true);
		}
		//todButtons.at(i)->setEnabled(!setTime);
	 }




	//** Weather
	int weather = settings.value("weather").toInt();
	timeWeatherWidget->buttonGroupMetar->button(weather)->setChecked(true);

	timeWeatherWidget->txtMetar->setPlainText(settings.value("metar").toString());
	timeWeatherWidget->txtMetar->setEnabled(weather == 2);
	



}

//===============================================================
// Terrasync
//===============================================================

//** Group Box Checked
void fgx::on_group_box_terrasync_clicked(){
	settings.setValue("use_terrasync", coreSettingsWidget->groupBoxTerraSync->isChecked());
	settings.sync();
	exeTerraSync->setEnabled(coreSettingsWidget->groupBoxTerraSync->isChecked());
}

//===============================================================
// Misc Events
//===============================================================
void fgx::on_menuStyle_triggered(QAction *action){
	settings.setValue("gui_style", action->text());
	QApplication::setStyle(QStyleFactory::create(action->text()));
}

void fgx::on_actionAboutFGX_triggered(){
	QString txt;
	txt.append("<html><body><p>FGX FlightGear Launcher</b></p>");
	txt.append("<p>&copy; 2011 Gral aka Yves Sablonier and Pete Morgan</p>");
	txt.append("<p><a href='http://www.gnu.org/licenses/gpl-2.0.txt'>GPL2</a></p>");
	txt.append("<p><a href='http://wiki.flightgear.org'>FlightGear</a></p>");
	txt.append("</body></html>");
	QMessageBox::about(this, "About FGX", txt);
}

void fgx::on_actionAboutQT_triggered(){
	QMessageBox::aboutQt(this, "About Qt");
}

void fgx::on_actionQuit_triggered(){
	QApplication::quit();
}



//===============================================================
// Write settings on close
void fgx::closeEvent(QCloseEvent *event)
{
	save_settings();
	settings.saveWindow(this);
	settings.sync();
	event->accept();
}




void fgx::on_tabs_currentChanged(int index){
	settings.setValue("last_tab", index); //** Save this tab as the last one.. on next startup
	//if(index == 6){
	//	on_buttonViewCommand_clicked();
	//}
}

//==============================================
//** View Buttons
void fgx::on_buttonCommandPreview_clicked(){
	if(!validate()){
		return;
	}
	QString str = QString(settings.fgfs_path()).append(" ").append( fg_args());
	outputPreviewWidget->txtPreviewOutput->setPlainText(str);
}

void fgx::on_buttonCommandHelp_clicked(){
	QProcess process;
	QStringList args;
	args << "-h" << "-v" << QString("--fg-root=").append(settings.fg_root());
	process.start(settings.fgfs_path(), args, QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished();
		QString ok_result = process.readAllStandardOutput();
		QString error_result = process.readAllStandardError();
		outputPreviewWidget->txtPreviewOutput->setPlainText(ok_result);
	}
}



