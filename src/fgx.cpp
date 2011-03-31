/*
 *  fgx.cpp
 *  fgx
 *
 *  Created by Yves Sablonier, Zurich
 *  Copyright 2011 --- GPL2
 *
 */


#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <iostream>
#include <QTextStream>

#include <QXmlStreamReader>
#include <QSettings>
#include <QAbstractSocket>
#include <QHostInfo>
#include <QTimer>


#include <QtGui/QCloseEvent>
#include <QStyleFactory>

#include <QFileDialog>
//#include <QProgressDialog>


#include "fgx.h"
#include "settings/settingsdialog.h"


fgx::fgx(QMainWindow *parent) : QMainWindow(parent){

	setupUi(this);

	setProperty("settings_namespace", QVariant("launcher_window"));
	settings.restoreWindow(this);

	setWindowTitle(QCoreApplication::applicationName().append(" - ").append(QCoreApplication::applicationVersion()));
	fgx_logo->setText(QCoreApplication::applicationName());


	//** Setup Styles
	QApplication::setStyle( QStyleFactory::create(settings.value("gui_style","Cleanlooks").toString()) );
	actionGroupStyle = new QActionGroup(this);
	actionGroupStyle->setExclusive(true);
	connect(actionGroupStyle, SIGNAL(triggered(QAction*)), this, SLOT(on_style(QAction*)) );
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



	//*** Setup up Button Groups as the UI cant ;-(
	buttonGroupTime = new QButtonGroup(this);
	buttonGroupTime->setExclusive(true);
	buttonGroupTime->addButton(radioButtonTimeRealTime);
	buttonGroupTime->addButton(radioButtonTimeDawn);
	buttonGroupTime->addButton(radioButtonTimeMorning);
	buttonGroupTime->addButton(radioButtonTimeNoon);
	buttonGroupTime->addButton(radioButtonTimeAfterNoon);
	buttonGroupTime->addButton(radioButtonTimeEvening);
	buttonGroupTime->addButton(radioButtonTimeDusk);
	buttonGroupTime->addButton(radioButtonTimeMidnight);

	buttonGroupWeather = new QButtonGroup(this);
	buttonGroupWeather->setExclusive(true);
	buttonGroupWeather->addButton(radioButtonWeatherNone, METAR_NONE);
	buttonGroupWeather->addButton(radioButtonWeatherLive, METAR_LIVE);
	buttonGroupWeather->addButton(radioButtonWeatherMetar, METAR_EDIT);
	connect(buttonGroupWeather, SIGNAL(buttonClicked(int)), this, SLOT(on_weather_selected()));

	buttonGroupLog = new QButtonGroup(this);
	buttonGroupLog->setExclusive(true);
	buttonGroupLog->addButton(radioButtonLogWarn, LOG_WARN);
	buttonGroupLog->addButton(radioButtonLogInfo, LOG_INFO);
	buttonGroupLog->addButton(radioButtonLogDebug, LOG_DEBUG);
	buttonGroupLog->addButton(radioButtonLogBulk, LOG_BULK);
	buttonGroupLog->addButton(radioButtonLogAlert, LOG_ALERT);


	//** Initialse Extra Widgets
	aircraftWidget = new AircraftWidget(this);
	tabs->insertTab(2, aircraftWidget, "Aircraft");

	airportsWidget = new AirportsWidget(this);
	tabs->insertTab(3, airportsWidget, "Airports");

	networkWidget = new NetworkWidget(this);
	tabs->insertTab(4, networkWidget, "Network");



	//** Restore Settings
	tabs->setCurrentIndex( settings.value("last_tab").toInt() );


	//***** Qt Has no Show event for a form, so we need to present Widgets first
	//** and then initialise. THis is achieved with a timer that triggers in a moment
	QTimer::singleShot(500, this, SLOT(initialize()));

}/* end constructor */

fgx::~fgx(){
}


//=======================================================================================================================
// Initial Setup
//=======================================================================================================================
void fgx::initialize(){


	load_settings();
	aircraftWidget->initialize();
	airportsWidget->initialize();

	if(!settings.paths_sane()){
		show_settings_dialog();
	}



}
//=======================================================================================================================
// Process Related
//=======================================================================================================================
void fgx::kill_process(QString pid) {

	QString command("kill ");
	command.append(pid);
	QProcess::startDetached(command);
}

//=======================================================================================================================
// Start FlightGear
//=======================================================================================================================
void fgx::on_buttonStartFg_clicked() {

	//* Validate

	if(!aircraftWidget->validate()){
		tabs->setCurrentIndex( tabs->indexOf(aircraftWidget));
		return;
	}

	if(!airportsWidget->validate()){
		tabs->setCurrentIndex( tabs->indexOf(airportsWidget));
		return;
	}

	if(!networkWidget->validate()){
		tabs->setCurrentIndex( tabs->indexOf(networkWidget));
		return;
	}

	if(groupBoxTerraSync->isChecked() and txtTerraSyncPath->text().length() == 0){
		tabs->setCurrentIndex(0);
		txtTerraSyncPath->setFocus();
	}
	
	txtStartupCommand->setPlainText(fg_args().join("\n"));

	//** This process will always start on the shell as fgfs returns an error help if incorrect args
	bool start = QProcess::startDetached( settings.fgfs_path(), fg_args(), QString(), &pid_fg);
	Q_UNUSED(start);

}


//=======================================================================================================================
// Stop FlightGear
//=======================================================================================================================
void fgx::on_buttonStopFg_clicked() {
	kill_process(QString::number(pid_fg));
}


//=======================================================================================================================
// Start FGCom
//=======================================================================================================================
void fgx::start_fgcom(){

	QString command("nice");

	QStringList args;
	args << "fgcom" << networkWidget->txtFgComNo->text() << "-p" << networkWidget->txtFgComPort->text();

	QProcess::startDetached(command, args, QString(), &pid_fgcom);
}
//=======================================================================================================================
// Stop FGCom
//=======================================================================================================================
void fgx::stop_fgcom() {
	kill_process(QString::number(pid_fgcom));
}




//=======================================================================================================================
// Start Terrasync
//=======================================================================================================================
void fgx::on_buttonStartTerraSync_clicked(){
	start_terrasync();
}

void fgx::start_terrasync(){

	QString command("nice");

	QStringList args;
	args << "terrasync" << "-p" << "5505" << "-S" << "-d" << "settings";

	int start = QProcess::startDetached(command, args, QString(), &pid_terra);
	Q_UNUSED(start);

}


//=======================================================================================================================
// Stop TerraSync
//=======================================================================================================================
void fgx::on_buttonStopTerraSync_clicked(){
	stop_terrasync();
}

void fgx::stop_terrasync() {

	/* This is a really nasty hack cos I dont know what Im doing!! (said pete)
	   Gets a list of processes, and find terrasync arg
	   Then kills it - is there a better way ?
	*/
	//** Get a list of ALL process
	QStringList args;
	args << "-ef";
	QProcess process;
	process.start("ps", args, QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished();
		QString ok_result = process.readAllStandardOutput();
		QString error_result = process.readAllStandardError();

		//* take result and split into parts
		QStringList entries = ok_result.split("\n");
		for(int i=0; i < entries.count(); i++){
			QString entry(entries.at(i));

			if(entry.contains("terrasync -p 5505")){
				//* found a terrasync  so Kill it
				QStringList parts = entry.split(" ", QString::SkipEmptyParts);
				QStringList killargs;
				killargs << "-9" << parts.at(1);
				int start = QProcess::startDetached("kill", killargs);
				Q_UNUSED(start);
			}
		}
	}
}

//=======================================================================================================================
// FlightGear Command Args
//=======================================================================================================================
QStringList fgx::fg_args(){

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
	

	
	// Redirect stdout and stderr to logfile
	QString argwritelogdir = QDir::currentPath();
	QString argwritelog;
	argwritelog.append(argwritelogdir);
	argwritelog = " &> fgfslog.log";
	



	//++ WARNING >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//** Please make sure there are no spaces in the args.
	//** Any args with spaces are quoted eg ' --foo=bar'
	//-- end Warning <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	QStringList args;

	//= fg_root
	args << QString("--fg-root=").append(settings.fg_root());


	//= Startup , Spalsh, Geometry
	args << QString("--geometry=").append(comboScreenSize->currentText());
	if (checkboxDisableSplash->isChecked()) {
		args << QString("--disable-splash-screen");
	}
	if (checkboxFullScreen->isChecked()) {
		args << QString("--enable-fullscreen");
	}

	args << aircraftWidget->get_args();

	//* Custom Scenery
	/*
	if (usecustomScenery->isChecked()) {
		command.append(" --fg-scenery=$HOME/Documents/TerrasyncScenery:");
		command.append(settings.fg_root()).append("/Scenery/");
		command.append("/Scenery/");
	} else {
		command.append(" --fg-scenery=");
		command.append(settings.fg_root()).append("/Scenery/");
		command.append("/Scenery/");
	}
	*/
	//* terrasync enabled
	if (groupBoxTerraSync->isChecked()) {
		args << QString("--atlas=socket,out,5,localhost,5505,udp");
	}
		
		
	//** Network
	args << networkWidget->get_args();

	//** Airport, Runway Start pos
	args << airportsWidget->get_args();



	//* Weather fetch
	if (buttonGroupWeather->checkedId() == METAR_LIVE) {
		args << QString("--enable-real-weather-fetch");
	}else{
		args << QString("--disable-real-weather-fetch");
	}

	//* Ai Traffic
	/*
	if (enableAITraffic->isChecked()) {
		args << QString("--enable-ai-traffic");
	}else{
		args << QString("--disable-ai-traffic");
	}
	*/

	//* Enable AI models ???
	args << QString("--enable-ai-models");

	//* Metar
	/*
	if (useMetar->isChecked()) {
		command.append("--metar=");
		QString metartoUse = metarText->toPlainText();
		command.append("\"");
		command.append(metartoUse);
		command.append("\"");
	}
	*/


	/*
	if (groupBoxSetTime->isChecked() == true) {
		content.append(" --start-date-lat=");
		content.append(argtime);
	} else {
		content.append(" --timeofday=");
		content.append(argtimeofday);
	}
	*/




	//*  Additonal **args - remove line endings in command line text field and add arguments
	if (lineEditExtraArgs->toPlainText().trimmed().length() > 0) {
		args << QString(" ").append( lineEditExtraArgs->toPlainText().replace(QString("\n"), QString(" ")) );
	}

	//* Log Level
	args << QString("--log-level=").append( buttonGroupLog->button(buttonGroupLog->checkedId())->text().toLower() );


	if (checkBoxLogEnabled->isChecked()) {
		args << QString(argwritelog);
	}

	return args;
}


//================================================================================
// Save Settings
//================================================================================
void fgx::save_settings()
{
	//## NB: fgfs path and FG_ROOT are saves in SettingsDialog ##
	qDebug() << "saves settings";
	aircraftWidget->save_settings();
	airportsWidget->save_settings();
	networkWidget->save_settings();


	settings.setValue("use_terrasync", groupBoxTerraSync->isChecked());
	settings.setValue("terrasync_path", txtTerraSyncPath->text());


	settings.setValue("screen_size", comboScreenSize->currentText());
	settings.setValue("screen_full", checkboxFullScreen->isChecked());
	settings.setValue("screen_splash", checkboxDisableSplash->isChecked());

	


	
	//if (usecustomScenery->isChecked() == true) {
	//	settings.setValue("usecustomScenery", "true");
	//}	else {
	//	settings.setValue("usecustomScenery", "false");
	//}
	
	


	
	settings.setValue("year", year->text());
	settings.setValue("month", month->text());
	settings.setValue("day", day->text());
	settings.setValue("hour", hour->text());
	settings.setValue("minute", minute->text());
	settings.setValue("second", second->text());
	
	//* Time
	settings.setValue("timeofday", buttonGroupTime->checkedButton()->text());
	if (groupBoxSetTime->isChecked() == true) {
		settings.setValue("setTime", "true");
	}	else {
		settings.setValue("setTime", "false");
	}
	
	//* Weather
	settings.setValue("weather", buttonGroupWeather->checkedId());
	settings.setValue("metar", metarText->toPlainText());
	
	//* Advanced
	settings.setValue("extra_args", lineEditExtraArgs->toPlainText());
	settings.setValue("log_enabled", checkBoxLogEnabled->isChecked());
	settings.setValue("log_level", buttonGroupLog->checkedId());

	
	
}

//================================================================================
// Load Settings
//================================================================================
void fgx::load_settings()
{

	fgdataPath->setText(settings.fg_root());
	fgfsPath->setText(settings.fgfs_path());

	aircraftWidget->load_settings();
	airportsWidget->load_settings();
	networkWidget->load_settings();
	
	groupBoxTerraSync->setChecked(settings.value("use_terrasync").toBool());
	txtTerraSyncPath->setText( settings.value("terrasync_path").toString() );

	//** Sartup sxreens
	comboScreenSize->setCurrentIndex( comboScreenSize->findText(settings.value("screen_size").toString()) );
	checkboxFullScreen->setChecked(settings.value("screen_full").toBool());
	checkboxDisableSplash->setChecked(settings.value("screen_splash").toBool());



	
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
	
	QString setTimeSet = settings.value("setTime").toString();
	if (setTimeSet == "true") {
		groupBoxSetTime->setChecked(true);
	} else {
		groupBoxSetTime->setChecked(false);
	}

	//** Time Of Day
	QString tod = settings.value("timeofday", "Real Time").toString();
	qDebug() << "TOD=" << tod;
	QList<QAbstractButton *> todButtons = buttonGroupTime->buttons();
	for (int i = 0; i < todButtons.size(); ++i) {
		if(todButtons.at(i)->text() == tod){
			todButtons.at(i)->setChecked(true);
			break;
		}
	 }

	//** Weather
	int idx_weather = settings.value("weather").toInt();
	buttonGroupWeather->button(idx_weather)->setChecked(true);
	metarText->setPlainText(settings.value("metar").toString());
	
	int size_idx = comboScreenSize->findText(settings.value("screen_size").toString());
	comboScreenSize->setCurrentIndex( size_idx == -1 ? 0 : size_idx);
	

		
	
	lineEditExtraArgs->setPlainText(settings.value("extra_args").toString());
	checkBoxLogEnabled->setChecked(settings.value("log_enabled").toBool());
	int idxLog = settings.value("log_level", LOG_WARN).toInt();
	buttonGroupLog->button(idxLog)->setChecked(true);

}

//===============================================================
// Terrasync
//===============================================================
//** Path buttons Clicked
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
}

//===============================================================
// Misc Events
//===============================================================
void fgx::on_style(QAction *action){
	settings.setValue("gui_style",action->text());
	QApplication::setStyle(QStyleFactory::create(action->text()));
}

void fgx::on_actionAboutFGX_triggered(){
	QString txt;
	txt.append("<html><body><p><b>Launcher for OSX</b></p>");
	txt.append("<p>© 2011 Gral aka Yves Sablonier, Zurich</p>");
	txt.append("<p><a href='http://www.gnu.org/licenses/gpl-2.0.txt'>GPL2</a></p>");
	txt.append("<p><a href='http://wiki.flightgear.org'>FlightGear</a></p>");
	txt.append("</body></html>");
	QMessageBox::about(this, "About FGX", txt);
}

void fgx::on_actionAboutQT_triggered(){
	QMessageBox::aboutQt(this, "About Qt");
}

// Set Time checked
void fgx::on_groupBoxSetTime_clicked() {
	
	bool enabled  = groupBoxSetTime->isChecked();
	//timeoftheDay->setEnabled(!enabled);
	year->setEnabled(enabled);
	month->setEnabled(enabled);
	day->setEnabled(enabled);
	hour->setEnabled(enabled);
	minute->setEnabled(enabled);
	second->setEnabled(enabled);
}

// Metar checked
void fgx::on_weather_selected() {
	metarText->setEnabled(buttonGroupWeather->checkedId() == METAR_EDIT);
	if(buttonGroupWeather->checkedId() == METAR_EDIT){
		metarText->setFocus();
	}
}



//===============================================================
// Write settings on close
void fgx::closeEvent(QCloseEvent *event)
{
	save_settings();
	settings.saveWindow(this);
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
	settings.setValue("last_tab", index);
}



void fgx::on_buttonTest_clicked(){
	qDebug() << "YES" << pid_fg;
	//qDebug() << networkWidget->get_args();
	//networkWidget->save_settings();
	//qDebug() << aircraftWidget->aircraft();
	//aircraftWidget->save_settings();
	save_settings();
	qDebug() << airportsWidget->get_args();
	//txtStartupCommand->setPlainText(fg_args().join("\n"));
	//qDebug() << start_fg_args();

}



