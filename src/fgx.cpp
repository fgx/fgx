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

	//** Time / Weather Widget
	timeWeatherWidget = new TimeWeatherWidget(this);
	tabs->insertTab(1, timeWeatherWidget, tr("Time and Weather"));

	//** Aircraft tab
	aircraftWidget = new AircraftWidget(this);
	tabs->insertTab(2, aircraftWidget, tr("Aircraft"));

	//** Airports Tab
	airportsWidget = new AirportsWidget(this);
	tabs->insertTab(3, airportsWidget, tr("Airports"));

	//** Network Tab
	networkWidget = new NetworkWidget(this);
	tabs->insertTab(4, networkWidget, tr("Network"));


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
		show_settings_dialog();
	}

	//** Paths are sane so we can initialize;
	//TODO setup wizard to import data first time
	aircraftWidget->initialize();
	airportsWidget->initialize();
	load_joysticks();
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
	txtPreview->setPlainText(command_line);

	if(checkBoxMpMap->isChecked()){
		QUrl mapUrl("http://mpmap02.flightgear.org/");
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

	if(groupBoxTerraSync->isChecked() and txtTerraSyncPath->text().length() == 0){
		tabs->setCurrentIndex(0);
		txtTerraSyncPath->setFocus();
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


	//** Startup , Splash, Geometry
	args << QString("--geometry=").append( comboScreenSize->currentText() );
	if (checkboxDisableSplash->isChecked()) {
		args << QString("--disable-splash-screen");
	}
	if (checkboxFullScreen->isChecked()) {
		args << QString("--enable-fullscreen");
	}

	//** Autocoordination with Moouse - is this the same for joysticks ??
	args << QString(checkBoxEnableAutoCordination->isChecked() ? "--enable-auto-coordination" : "--disable-auto-coordination");


	//** Terrasync - send on socket
	if (groupBoxTerraSync->isChecked()) {
		args << QString("--atlas=socket,out,5,localhost,5505,udp");
	}

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

	//** Create the return string
	QString args_string = args.join(" ");

	//* Log Level - Redirect stdout and stderr to logfile MUST be last argument
	if(checkBoxLogEnabled->isChecked()){
		args_string.append("--log-level=warn ");
		args_string.append("&>").append(QDir::currentPath()).append("/fgfslog.txt");
	}
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
	aircraftWidget->save_settings();
	airportsWidget->save_settings();
	networkWidget->save_settings();


	settings.setValue("use_terrasync", groupBoxTerraSync->isChecked());
	settings.setValue("terrasync_sync_path", txtTerraSyncPath->text());


	settings.setValue("screen_size", comboScreenSize->currentText());
	settings.setValue("screen_full", checkboxFullScreen->isChecked());
	settings.setValue("screen_splash", checkboxDisableSplash->isChecked());

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
	settings.setValue("extra_args", lineEditExtraArgs->toPlainText());
	settings.setValue("log_enabled", checkBoxLogEnabled->isChecked());
	settings.setValue("log_level", buttonGroupLog->checkedButton()->text());
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

	fgdataPath->setText(settings.fg_root());
	fgfsPath->setText(settings.fgfs_path());

	aircraftWidget->load_settings();
	airportsWidget->load_settings();
	networkWidget->load_settings();
	
	groupBoxTerraSync->setChecked(settings.value("use_terrasync").toBool());
	exeTerraSync->setEnabled( settings.value("use_terrasync").toBool() );
	txtTerraSyncPath->setText( settings.value("terrasync_sync_path").toString() );

	//** Sartup sxreens	
	int idx = comboScreenSize->findText(settings.value("screen_size").toString());
	comboScreenSize->setCurrentIndex( idx == -1 ? 0 : idx );
	checkboxFullScreen->setChecked(settings.value("screen_full").toBool());
	checkboxDisableSplash->setChecked(settings.value("screen_splash").toBool());

	
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
	

		
	lineEditExtraArgs->setPlainText(settings.value("extra_args").toString());

	checkBoxLogEnabled->setChecked(settings.value("log_enabled").toBool());
	radioButtonLogWarn->setChecked(true);
	QString log_level = settings.value("log_level").toString();
	QList<QAbstractButton *> logButtons = buttonGroupLog->buttons();
	for (int i = 0; i < logButtons.size(); ++i) {
		if(logButtons.at(i)->text() == log_level){
			logButtons.at(i)->setChecked(true);
			break;
		}
	 }

}

//===============================================================
// Terrasync
//===============================================================

//** Path buttons Clicked - Opens the path dialog
void fgx::on_buttonTerraSyncPath_clicked(){

	QString filePath = QFileDialog::getExistingDirectory(this,
														 tr("Select Terrasync Directory"),
														 txtTerraSyncPath->text(),
														 QFileDialog::ShowDirsOnly);
	if(filePath.length() > 0){
		txtTerraSyncPath->setText(filePath);
	}

	//* Need to write out the terrasync dir as its used other places ;-(
	settings.setValue("terrasync_path", txtTerraSyncPath->text());
	settings.sync();
}

//** Group Box Checked
void fgx::on_groupBoxTerraSync_clicked(){
	settings.setValue("use_terrasync", groupBoxTerraSync->isChecked());
	settings.sync();
	exeTerraSync->setEnabled(groupBoxTerraSync->isChecked());
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


//========================================================================
// Set Time checked
void fgx::on_groupBoxSetTime_clicked() {
	
	/* TODO
	bool enabled  = groupBoxSetTime->isChecked();
	year->setEnabled(enabled);
	month->setEnabled(enabled);
	day->setEnabled(enabled);
	hour->setEnabled(enabled);
	minute->setEnabled(enabled);
	second->setEnabled(enabled);
	*/
	QList<QAbstractButton *> buttons = timeWeatherWidget->buttonGroupTime->buttons();
	for(int i=0; i < buttons.count(); i++){
		QAbstractButton *butt = buttons.at(i);
		butt->setEnabled(true);
	}
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

//===============================================================
// Settings Dialog
void fgx::show_settings_dialog(){
	SettingsDialog *settingsDialog = new SettingsDialog();
	if(settingsDialog->exec()){
		fgfsPath->setText(settings.fgfs_path());
		fgdataPath->setText(settings.fg_root());
	}
}
void fgx::on_buttonFgFsPath_clicked(){
	show_settings_dialog();
}
void fgx::on_buttonFgRootPath_clicked(){
	show_settings_dialog();
}





void fgx::on_tabs_currentChanged(int index){
	settings.setValue("last_tab", index); //** Save this tab as the last one.. on next startup
	if(index == 6){
		on_buttonViewCommand_clicked();
	}
}

//==============================================
//** View Buttons
void fgx::on_buttonViewCommand_clicked(){
	if(!validate()){
		return;
	}
	QString str = QString(settings.fgfs_path()).append(" ").append( fg_args());
	txtPreview->setPlainText(str);
}

void fgx::on_buttonViewHelp_clicked(){
	QProcess process;
	QStringList args;
	args << "-h" << "-v" << QString("--fg-root=").append(settings.fg_root());
	process.start(settings.fgfs_path(), args, QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished();
		QString ok_result = process.readAllStandardOutput();
		QString error_result = process.readAllStandardError();
		txtPreview->setPlainText(ok_result);
	}
}


//==============================================
//** Load Joysticks
void fgx::load_joysticks(){
	comboJoystick->clear();
	comboJoystick->addItem("-- None--");
	QProcess process;
	process.start("js_demo", QStringList(), QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished();
		QString ok_result = process.readAllStandardOutput();
		QString error_result = process.readAllStandardError();
		Q_UNUSED(error_result);
		//* take result and split into parts
		QStringList entries = ok_result.trimmed().split("\n");
		for(int i=2; i < entries.count(); i++){ //First 2 lines are crap
			comboJoystick->addItem(entries.at(i));
		}
	}
}



