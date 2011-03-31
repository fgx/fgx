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

//#include <QFileDialog>
//#include <QProgressDialog>


#include "fgx.h"
#include "settings/settingsdialog.h"


fgx::fgx(QMainWindow *parent) : QMainWindow(parent){

	setupUi(this);

	setProperty("settings_namespace", QVariant("launcher_window"));
	settings.restoreWindow(this);

	setWindowTitle(QCoreApplication::applicationName().append(" - ").append(QCoreApplication::applicationVersion()));
	fgx_logo->setText(QCoreApplication::applicationName());

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
	tabs->insertTab(3, aircraftWidget, "Aircraft");

	airportsWidget = new AirportsWidget(this);
	tabs->insertTab(4, airportsWidget, "Airports");

	networkWidget = new NetworkWidget(this);
	tabs->insertTab(5, networkWidget, "Network");



	//** Restore Settings
	tabs->setCurrentIndex( settings.value("last_tab").toInt() );


	initialize();

}/* end constructor */

fgx::~fgx(){
}


//=======================================================================================================================
// Initial Setup
//=======================================================================================================================
void fgx::initialize(){


	load_settings();
	//on_enableMultiplayer_clicked();
	//on_useMetar_clicked();
	//on_groupBoxSetTime_clicked();
	//checkCoords();
		
	
	
	
	// startup checks
	checkAirportlist();
	
	listchecked = true;
	
	checkScenery();
	
	
	load_settings();

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
	if(!networkWidget->validate()){
		tabs->setCurrentIndex( tabs->indexOf(networkWidget));
		return;
	}
	if(!aircraftWidget->validate()){
		tabs->setCurrentIndex( tabs->indexOf(aircraftWidget));
		return;
	}

	checkScenery();
	
	txtStartupCommand->setPlainText(fg_args().join("\n"));

	//** This process will always start on the shell as fgfs returns an error help if incorrect args
	bool start = QProcess::startDetached( settings.fgfs_path(), fg_args(), QString(), &pid_fg);
	qDebug() << "PID=" << pid_fg << "=" << start;

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
void fgx::start_terrasync(){

	QString command("nice");

	QStringList args;
	args << "terrasync" << "-p" << "5505" << "-S" << "-d" << "settings";

	QProcess::startDetached(command, args, QString(), &pid_terra);
}

//=======================================================================================================================
// Stop FGCom
//=======================================================================================================================
void fgx::stop_terrasync() {
	kill_process(QString::number(pid_terra));
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
	if (useTerraSync->isChecked()) {
		args << QString("--atlas=socket,out,5,localhost,5505,udp");
	}
		
		
	//** Network
	args << networkWidget->get_args();


	//* Airport
	//args << QString("--airport=").append(locationIcao->currentText());

	//* Runway
	if (runWay->isEnabled()) {
		//args << QString("--runway=").append(runWay->currentText());
	}

	// Park Position
	if (parkPosition->isEnabled() == true) {
		args << QString("--parkpos=").append(parkPosition->currentText());
	}

	//* Coordinates
	if (checkBoxUseCoordinates->isChecked()) {
		if( Longitude->text().length() > 0 and Latitude->text().length() > 0){
			args << QString("--lon=").append(Longitude->text());
			args << QString("--lat=").append(Latitude->text());
		}
	}

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
	networkWidget->save_settings();

	if (useTerraSync->isChecked() == true) {
		settings.setValue("useTerraSync", "true");
	}	else {
		settings.setValue("useTerraSync", "false");
	}
	
	


	settings.setValue("screen_size", comboScreenSize->currentText());
	settings.setValue("screen_full", checkboxFullScreen->isChecked());
	settings.setValue("screen_splash", checkboxDisableSplash->isChecked());

	

	settings.setValue("locationIcao", locationIcao->currentText());
	
	if (usecustomScenery->isChecked() == true) {
		settings.setValue("usecustomScenery", "true");
	}	else {
		settings.setValue("usecustomScenery", "false");
	}
	
	

	settings.setValue("coordinates", checkBoxUseCoordinates->isChecked());
	settings.setValue("lon", Longitude->text());
	settings.setValue("lat", Latitude->text());


	settings.setValue("runWay", runWay->currentText());
	settings.setValue("parkPosition", parkPosition->currentText());
	
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
	networkWidget->load_settings();
	
	
	QString useTerraSyncSet = settings.value("useTerraSync").toString();
	if (useTerraSyncSet == "true") {
		useTerraSync->setCheckState(Qt::Checked);
	} else {
		useTerraSync->setCheckState(Qt::Unchecked);
	}
	

	//** Sartup sxreens
	comboScreenSize->setCurrentIndex( comboScreenSize->findText(settings.value("screen_size").toString()) );
	checkboxFullScreen->setChecked(settings.value("screen_full").toBool());
	checkboxDisableSplash->setChecked(settings.value("screen_splash").toBool());


	if (settings.value("locationIcao").toString() != "") {
		QString locationICAOSet = settings.value("locationIcao").toString();
		locationIcao->insertItem(0, locationICAOSet);
		locationIcao->insertItem(1, "----");
	} else {
		locationIcao->insertItem(0, "KSFO");
		locationIcao->insertItem(1, "----");
		//timeoftheDay->setCurrentIndex(0);
	}

	QString usecustomScenerySet = settings.value("usecustomScenery").toString();
	if (usecustomScenerySet == "true") {
		usecustomScenery->setCheckState(Qt::Checked);
	} else {
		usecustomScenery->setCheckState(Qt::Unchecked);
	}

	checkBoxUseCoordinates->setChecked(settings.value("coordinates").toBool());
	Latitude->setText(settings.value("lat").toString());
	Longitude->setText(settings.value("lng").toString());


	
	QString runWaySet = settings.value("runWay").toString();
	runWay->insertItem(0, runWaySet);
	runWay->insertItem(1, "----");
	
	QString parkPositionSet = settings.value("parkPosition").toString();
	parkPosition->insertItem(0, parkPositionSet);
	parkPosition->insertItem(1, "----");
	
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


// Check location edit change

void fgx::on_locationIcao_activated() {
	checkScenery();
}





void fgx::on_tabs_currentChanged() {
	checkScenery();
}




// Use coordinates checked

void fgx::on_checkBoxUseCoordinates_clicked() {
	
	bool checked = checkBoxUseCoordinates->isChecked();
	locationIcao->setEnabled(!checked);
	runWay->setEnabled(!checked);
	parkPosition->setEnabled(!checked);
	//Latitude->setEnabled(checked);
	//Longitude->setEnabled(true);
}

// Multiplayer checked

//void fgx::on_enableMultiplayer_clicked() {
	/*
	Qt::CheckState state;

	state = enableMultiplayer->checkState();
	
	if (state == Qt::Unchecked) {
		pathMultiplayerOut->setEnabled(false);
		callSign->setEnabled(false);
		multiplayerPort->setEnabled(false);
		enableAITraffic->setEnabled(true);
	} else {
		pathMultiplayerOut->setEnabled(true);
		callSign->setEnabled(true);
		multiplayerPort->setEnabled(true);
		enableAITraffic->setEnabled(false);
		enableAITraffic->setCheckState(Qt::Checked);
	}
	*/
//}

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



// Check for using park position (needs runways disabled)
void fgx::on_useParkPosition_clicked() {
	
	Qt::CheckState ppstate;
	
	ppstate = useParkPosition->checkState();
	if (ppstate == Qt::Checked) {
		runWay->setEnabled(false);
		if (parkPosition->currentText() != "") {
			parkPosition->setEnabled(true);
		}
		
	} else {
		runWay->setEnabled(true);
		parkPosition->setEnabled(false);
	}
}


// Check for scenery when "use custom scenery" is clicked

void fgx::on_usecustomScenery_clicked() {
	locationIcao->clear();
	runWay->clear();
	parkPosition->clear();
	checkAirportlist();
	checkScenery();

}

void fgx::checkAirportlist() {
	return;
	QString directory = settings.airports_path();
	/*
	if (usecustomScenery->isChecked() == true) {
		directory = QDir::homePath();
		directory.append("/Documents/TerrasyncScenery/Airports");
	}	else {
		directory = fgdataPath->text();
		directory.append("/Scenery/Airports");
	}
	*/

	QStringList files;
	QString entry;

	QDirIterator it(directory, QDirIterator::Subdirectories);
	while (it.hasNext()) {
		//files << it.next();
		entry = it.next();
		if(entry.endsWith(".threshold.xml") ){
			files << entry;
		}
	}
	qDebug() << files;
	return;
	// filters Stringlist for Airport entries containing "threshold"
		
	QStringList filtered;
	filtered = files.filter(QRegExp("threshold.xml"));
		
	// filter path Strings in Stringlist to get a valid airport name
		
	filtered.replaceInStrings(".threshold.xml", "");
	filtered.replaceInStrings(directory, "");
	filtered.replaceInStrings(QRegExp("......."), "");
		
	//removing duplicates
	filtered.removeDuplicates();
		
	// add to list
	locationIcao->addItems(filtered);

	listchecked = true;
}

// Check for scenery, runways and parkposition

void fgx::checkScenery() {
	
	
	// Check for installed airports
	
	QString directory;
	
	if (usecustomScenery->isChecked() == true) {
		directory = QDir::homePath();
		directory.append("/Documents/TerrasyncScenery/Airports");
	}	else {
		directory = fgdataPath->text();
		directory.append("/Scenery/Airports");
	}
	
	QStringList files;
	
	
	if (listchecked != true) {
		
		checkAirportlist();
		}
	
	
	QString letters = locationIcao->currentText();
	directory.append("/");
	directory.append(letters[0]);
	directory.append("/");
	directory.append(letters[1]);
	directory.append("/");
	directory.append(letters[2]);
	directory.append("/");
	directory.append(letters);
	directory.append(".threshold.xml");
	
	QFile rwyfile(directory);
	
	if (rwyfile.exists() == true) {
		
		if (rwyfile.open(QIODevice::ReadOnly)) {
			
			QXmlStreamReader rwyreader(&rwyfile);
			QXmlStreamReader::TokenType tokenType;
			
			QStringList runwayList;
			QString rwy;
			while ((tokenType = rwyreader.readNext()) != QXmlStreamReader::EndDocument) {
				if (rwyreader.name() == "rwy") {
					rwy = rwyreader.readElementText();
					runwayList.append(rwy);
				}
			}
			
			rwyfile.close();
			
			runWay->clear();
			runWay->addItems(runwayList);
			runWay->setEnabled(true);
			
		} else {
			runWay->setEnabled(false);
		}

		
		QString ppfilepath;
		
		if (usecustomScenery->isChecked() == true) {
			ppfilepath = directory.replace("threshold", "groundnet");
		} else {
		ppfilepath = directory.replace("threshold", "parking");
		}
		
		QFile ppfile(ppfilepath);
		
		if (ppfile.exists() == false) {
			if (usecustomScenery->isChecked() == true) {
				sceneryCheck->setText("<font color=#ff0000 size=11px>Using Custom Scenery data, currently no park positions avaiable.");
				useParkPosition->setEnabled(false);
				useParkPosition->setCheckState(Qt::Unchecked);
			} else {
				sceneryCheck->setText("<font color=#ff0000 size=11px>Using built-in Scenery, no park positions avaiable.");
				useParkPosition->setEnabled(false);
				useParkPosition->setCheckState(Qt::Unchecked);
			}

			
			parkPosition->setEnabled(false);
		} else {
			
			if (ppfile.open(QIODevice::ReadOnly)) {
				
				QXmlStreamReader ppreader(&ppfile);
				QXmlStreamReader::TokenType tokenType;
				
				QStringList ppList;
				
				while ((tokenType = ppreader.readNext()) != QXmlStreamReader::EndDocument) {
					if (ppreader.name() == "Parking") {
						QXmlStreamAttributes attributes = ppreader.attributes();
						
						if (attributes.value("type").toString() == "gate" && attributes.value("name").toString() != "Startup Location") {
							QString ppname = attributes.value("name").toString();
							QString ppnumber = attributes.value("number").toString();
							QString ppall;
							ppall.append(ppname); ppall.append(ppnumber);
							ppList.append(ppall);
						}
						
					}
				}
				
				ppfile.close();
				
				parkPosition->clear();
				parkPosition->addItems(ppList);
				parkPosition->setEnabled(true);
				useParkPosition->setEnabled(true);
				
				
			}
			
			else {
				parkPosition->setEnabled(false);
			}
			
			if (usecustomScenery->isChecked() == true) {
				sceneryCheck->setText("<font color=#ff0000 size=11px>Using custom scenery data</font>");
			} else
			{sceneryCheck->setText("<font color=#ff0000 size=11px>Using built-in Scenery</font>");}
		}

		
	} else {
		sceneryCheck->setText("<font color=#ff0000 size=11px>No runway/park position avaiable. Using defaults.");
		runWay->setEnabled(false);
		parkPosition->setEnabled(false);
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
	txtStartupCommand->setPlainText(fg_args().join("\n"));
	//qDebug() << start_fg_args();

}



