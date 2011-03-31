/*
 *  fgx.cpp
 *  fgx
 *
 *  Created by Yves Sablonier, Zurich
 *  Copyright 2011 --- GPL2
 *
 */

#include "fgx.h"
#include <QFile>
#include <QDir>
#include <iostream>
#include <QTextStream>
#include <QSettings>
#include <QtGui/QCloseEvent>
#include <QAbstractSocket>
#include <QHostInfo>
#include <QFileDialog>
#include <QDirIterator>
#include <QXmlStreamReader>
#include <QProgressDialog>
#include <QTimer>

#include "settings/settingsdialog.h"


fgx::fgx(QMainWindow *parent) : QMainWindow(parent){

	setupUi(this);

	setProperty("settings_namespace", QVariant("launcher_window"));
	settings.restoreWindow(this);

	setWindowTitle(QCoreApplication::applicationName().append(" - ").append(QCoreApplication::applicationVersion()));
	fgx_logo->setText(QCoreApplication::applicationName());

	//** Initialse Extra Widgets
	aircraftWidget = new AircraftWidget(this);
	tabs->addTab(aircraftWidget, "Aircraft");

	networkWidget = new NetworkWidget(this);
	tabs->addTab(networkWidget, "Network");



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
// Start FlightGear
//=======================================================================================================================
void fgx::on_fgStart_clicked() {
	qDebug() << "validation";
	if(!networkWidget->validate()){
		tabs->setCurrentIndex( tabs->indexOf(networkWidget));
		return;
	}
	if(!aircraftWidget->validate()){
		tabs->setCurrentIndex( tabs->indexOf(aircraftWidget));
		return;
	}

	checkScenery();
	

	//** Start FlightGear
	qDebug() << "COMMAND=" << start_fg_args();
	//QProcess fgProcess;

	bool start = QProcess::startDetached( settings.fgfs_path(), start_fg_args(), QString(), &pid_fg);
	qDebug() << "PID=" << pid_fg << "=" << start;
	/*
	fgProcess.start(start_fg_command());
	if(fgProcess.waitForStarted()){
		fgProcess.waitForFinished();
		QString ok_result = fgProcess.readAllStandardOutput();
		QString error_result = fgProcess.readAllStandardError();
		qDebug() << "OK=" << ok_result;
		qDebug() << "ERROR=" << error_result;
	}
	*/
}


//=======================================================================================================================
// Start FGCom
//=======================================================================================================================
void fgx::start_fg_com(){

	QString command("nice");

	QStringList args;
	args << "fgcom" << networkWidget->txtFgComNo->text() << "-p" << networkWidget->txtFgComPort->text();

	QProcess::startDetached(command, args, QString(), &pid_fgcom);
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
// FlightGear Command Args
//=======================================================================================================================
QStringList fgx::start_fg_args(){

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
	
	QStringList args;
	QString command;

	//** WARNING - make sure there are no spaces in the args. Any args with spaces are quoted.

	//* fg_root
	args << QString("--fg-root=").append(settings.fg_root());

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
	if (useCoordinates->isChecked()) {
		args << QString("--lon=").append(Longitude->text());
		args << QString("--lat=").append(Latitude->text());
	}

	//* Weather fetch
	if (Weather->currentText() == "real") {
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
	if (useMetar->isChecked()) {
		command.append("--metar=");
		QString metartoUse = metarText->toPlainText();
		command.append("\"");
		command.append(metartoUse);
		command.append("\"");
	}



	/*
	if (groupBoxSetTime->isChecked() == true) {
		content.append(" --start-date-lat=");
		content.append(argtime);
	} else {
		content.append(" --timeofday=");
		content.append(argtimeofday);
	}
	*/

	//* Startup , Spalsh, Geometry
	args << QString("--geometry=").append(screenSize->currentText());
	if (disableSplashScreen->isChecked()) {
		args << QString("--disable-splash-screen");
	}
	if (useFullScreen->isChecked()) {
		args << QString("--enable-fullscreen");
	}


	//*  Additonal **args - remove line endings in command line text field and add arguments
	if (commandLine->toPlainText().trimmed().length() > 0) {
		args << QString(" ").append( commandLine->toPlainText().replace(QString("\n"), QString(" ")) );
	}

	//* Log Level
	args << QString("--log-level=").append(logLevel->currentText());


	if (enableLog->isChecked() == true) {
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

	aircraftWidget->save_settings();
	networkWidget->save_settings();

	if (useTerraSync->isChecked() == true) {
		settings.setValue("useTerraSync", "true");
	}	else {
		settings.setValue("useTerraSync", "false");
	}
	
	
	if (disableSplashScreen->isChecked() == true) {
		settings.setValue("disableSplashScreen", "true");
	}	else {
		settings.setValue("disableSplashScreen", "false");
	}
	
	settings.setValue("screenSize", screenSize->currentText());
	
	if (useFullScreen->isChecked() == true) {
		settings.setValue("useFullScreens", "true");
	}	else {
		settings.setValue("useFullScreen", "false");
	}
	
	settings.setValue("timeoftheDay", timeoftheDay->currentText());
	settings.setValue("locationIcao", locationIcao->currentText());
	
	if (usecustomScenery->isChecked() == true) {
		settings.setValue("usecustomScenery", "true");
	}	else {
		settings.setValue("usecustomScenery", "false");
	}
	
	
	if (useCoordinates->isChecked() == true) {
		settings.setValue("useCoordinates", "true");
	}	else {
		settings.setValue("useCoordinates", "false");
	}
	
	settings.setValue("Longitude", Longitude->text());
	settings.setValue("Latidude", Latitude->text());
	settings.setValue("runWay", runWay->currentText());
	settings.setValue("parkPosition", parkPosition->currentText());
	
	settings.setValue("year", year->text());
	settings.setValue("month", month->text());
	settings.setValue("day", day->text());
	settings.setValue("hour", hour->text());
	settings.setValue("minute", minute->text());
	settings.setValue("second", second->text());
	
	if (groupBoxSetTime->isChecked() == true) {
		settings.setValue("setTime", "true");
	}	else {
		settings.setValue("setTime", "false");
	}
	
	settings.setValue("Weather", Weather->currentText());
	
	if (useMetar->isChecked() == true) {
		settings.setValue("useMetar", "true");
	}	else {
		settings.setValue("useMetar", "false");
	}
	
	settings.setValue("metarText", metarText->toPlainText());
	
	settings.setValue("commandLine", commandLine->toPlainText());
	settings.setValue("logLevel", logLevel->currentText());
	
	if (enableLog->isChecked() == true) {
		settings.setValue("enableLog", "true");
	}	else {
		settings.setValue("enableLog", "false");
	}
	
	
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
	

	
	
	QString disableSplashScreenSet = settings.value("disableSplashScreen").toString();
	if (disableSplashScreenSet == "true") {
		disableSplashScreen->setCheckState(Qt::Checked);
	} else {
		disableSplashScreen->setCheckState(Qt::Unchecked);
	}
	
	QString useFullScreenSet = settings.value("useFullScreen").toString();
	if (useFullScreenSet == "true") {
		useFullScreen->setCheckState(Qt::Checked);
	} else {
		useFullScreen->setCheckState(Qt::Unchecked);
	}
	/*
	QString pathMultiplayerOutSet = settings.value("pathMultiplayerOut").toString();
	pathMultiplayerOut->insertItem(0, pathMultiplayerOutSet);
	pathMultiplayerOut->insertItem(1, "----");
	
	QString multiplayerPortSet = settings.value("multiplayerPort").toString();
	multiplayerPort->setText(multiplayerPortSet);
	*/

	if (settings.value("locationIcao").toString() != "") {
		QString locationICAOSet = settings.value("locationIcao").toString();
		locationIcao->insertItem(0, locationICAOSet);
		locationIcao->insertItem(1, "----");
	} else {
		locationIcao->insertItem(0, "KSFO");
		locationIcao->insertItem(1, "----");
		timeoftheDay->setCurrentIndex(0);
	}

	QString usecustomScenerySet = settings.value("usecustomScenery").toString();
	if (usecustomScenerySet == "true") {
		usecustomScenery->setCheckState(Qt::Checked);
	} else {
		usecustomScenery->setCheckState(Qt::Unchecked);
	}


	QString useCoordinatesSet = settings.value("useCoordinates").toString();
	useCoordinates->setChecked(useCoordinatesSet == "true");

	
	QString lonSet = settings.value("Longitude").toString();
	Longitude->setText(lonSet);
	
	QString latSet = settings.value("Latidude").toString();
	Latitude->setText(latSet);
	
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
	
	QString WeatherSet = settings.value("Weather").toString();
	if (WeatherSet == "real") {
		Weather->setCurrentIndex(0);
	} else {
		Weather->setCurrentIndex(1);
	}
	
	QString useMetarSet = settings.value("useMetar").toString();
	if (useMetarSet == "true") {
		useMetar->setCheckState(Qt::Checked);
	} else {
		useMetar->setCheckState(Qt::Unchecked);
	}

	QString metarTextSet = settings.value("metarText").toString();
	metarText->setPlainText(metarTextSet);
	
	if (settings.value("screenSize").toString() != "") {
		QString screenSizeSet = settings.value("screenSize").toString();
		screenSize->insertItem(0, screenSizeSet);
		screenSize->insertItem(1, "----");
		screenSize->setCurrentIndex(0);
	} else {
		screenSize->insertItem(0, "1024x768");
		screenSize->insertItem(1, "----");
		screenSize->setCurrentIndex(0);
	}
	
	if ( settings.value("timeoftheDay").toString() != "") {
		QString timeoftheDaySet = settings.value("timeoftheDay").toString();
		timeoftheDay->insertItem(0, timeoftheDaySet);
		timeoftheDay->insertItem(1, "----");
		timeoftheDay->setCurrentIndex(0);
	} else {
		timeoftheDay->insertItem(0, "real");
		timeoftheDay->insertItem(1, "----");
		timeoftheDay->setCurrentIndex(0);
	}
		
	
	QString commandLineSet = settings.value("commandLine").toString();
	commandLine->setPlainText(commandLineSet);
	
	if (settings.value("logLevel").toString() != "") {
		QString logLevelSet = settings.value("logLevel").toString();
		logLevel->insertItem(0, logLevelSet);
		logLevel->insertItem(1, "----");
		logLevel->setCurrentIndex(0);
	} else {
		logLevel->insertItem(0, "warn");
		logLevel->insertItem(1, "----");
		timeoftheDay->setCurrentIndex(0);
		}
	
	QString enableLogSet = settings.value("enableLog").toString();
	if (enableLogSet == "true") {
		enableLog->setCheckState(Qt::Checked);
	} else {
		enableLog->setCheckState(Qt::Unchecked);
	}
	
}


// Check location edit change

void fgx::on_locationIcao_activated() {
	checkScenery();
}





void fgx::on_tabs_currentChanged() {
	checkScenery();
}




// Use coordinates checked

void fgx::on_useCoordinates_clicked() {
	
	bool checked = useCoordinates->isChecked();
	locationIcao->setEnabled(!checked);
	runWay->setEnabled(!checked);
	parkPosition->setEnabled(!checked);
	//Latitude->setEnabled(checked);
	//Longitude->setEnabled(true);
}

// Multiplayer checked

void fgx::on_enableMultiplayer_clicked() {
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
}

// Set Time checked
void fgx::on_groupBoxSetTime_clicked() {
	
	bool enabled  = groupBoxSetTime->isChecked();
	timeoftheDay->setEnabled(!enabled);
	year->setEnabled(enabled);
	month->setEnabled(enabled);
	day->setEnabled(enabled);
	hour->setEnabled(enabled);
	minute->setEnabled(enabled);
	second->setEnabled(enabled);
}

// Metar checked

void fgx::on_useMetar_clicked() {
	
	Qt::CheckState metarstate;
	
	metarstate = useMetar->checkState();
	if (metarstate == Qt::Checked) {
		Weather->setEnabled(false);
		Weather->setCurrentIndex(1);
	} else {
		Weather->setEnabled(true);
	}
}





// Check for coordinates enabled, disable ICAO, runway, park position
void fgx::checkCoords() {
	
	bool checked = useCoordinates->isChecked();
	locationIcao->setEnabled(!checked);
	runWay->setEnabled(!checked);
	parkPosition->setEnabled(!checked);

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
	
	QString directory;

	if (usecustomScenery->isChecked() == true) {
		directory = QDir::homePath();
		directory.append("/Documents/TerrasyncScenery/Airports");
	}	else {
		directory = fgdataPath->text();
		directory.append("/Scenery/Airports");
	}

	QStringList files;


	
	QDirIterator it(directory, QDirIterator::Subdirectories);
	while (it.hasNext()) {
		files << it.next();
		}
		
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
	lineEditDebug->setPlainText(start_fg_args().join("\n"));
	//qDebug() << start_fg_args();

}



