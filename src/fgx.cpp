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

	checkFGFS();
	on_enableMultiplayer_clicked();
	on_useMetar_clicked();
	on_groupBoxSetTime_clicked();
	checkCoords();
	

	
	// Setting some defaults in case
	
	if (portFGCOM->text() == "") {
		portFGCOM->setText("16661");
	}
	
	if (multiplayerPort->text() == "") {
		multiplayerPort->setText("5000");
	}
	
	QDir fgdatadir(fgdataPath->text());
	
	if (fgdatadir.exists() == false || fgfsPath->text() == "" || useFGXfgfs->isChecked() == true) {
			QString fgdataPathGet = QDir::currentPath();
			fgdataPathGet.append("/fgx.app/Contents/Resources/fgx-fgdata");
			fgdataPath->setText(fgdataPathGet);
			QString fgfsPathGet = QDir::currentPath();
			fgfsPathGet.append("/fgx.app/Contents/MacOS");
			fgfsPath->setText(fgfsPathGet);
			useFGXfgfs->setCheckState(Qt::Checked);
	}
	
	// startup checks
	checkAirportlist();
	checkAircraftListStartup();
	
	listchecked = true;
	
	checkScenery();
	checkAircraftImage();
	
	ps.setProcessChannelMode(QProcess::MergedChannels);
	
	readSettings();


} /* end constructor */

fgx::~fgx(){

}



// Start FlightGear
void fgx::on_fgStart_clicked() {
	
	checkScenery();
	
	
	// Write commands and arguments to TerraSync.sh and run via Terminal.app	
	
	if (useTerraSync->isChecked() == true) {
		
	
	QString tsStartfilename = "TerraSync.sh";
	
    QFile tsfile(tsStartfilename);
	
	if(tsfile.open(QIODevice::WriteOnly))
		
	{
        QByteArray tscontent = "# Start TerraSync - auto generated by fgx. DO NOT MODIFY!\n";
		tscontent.append("\n");
		
		
		// Check for using built-in terrasync
		
		QString argterrasyncpath;
		
		if (useFGXfgfs->isChecked() == true) {
			argterrasyncpath = QDir::currentPath();
			argterrasyncpath.append("/fgx.app/Contents/MacOS/terrasync");
			tscontent.append(" nice ");
			tscontent.append(argterrasyncpath);
			tscontent.append(" -p 5505 -S -d \"$HOME/Documents/TerrasyncScenery\"\n");
		}	else {
			argterrasyncpath = fgfsPath->text();
			argterrasyncpath = fgdataPath->text();
			tscontent.append("nice terrasync -p 5505 -S -d \"$HOME/Documents/TerrasyncScenery\"\n");
		}
		
		tsfile.write(tscontent);
	}
	
	else
    {
        QMessageBox error;
        error.setText("Error: Can not open file TerraSync.sh to write");
        error.exec();
    }
	
	system("chmod 755 TerraSync.sh");
	system("open -a Terminal.app ./TerraSync.sh");
		
	}
	
	// Write commands and arguments to FGCOM.sh and run via Terminal.app
	
	
	if (enableFGCom->isChecked() == true) {
		
		QString comStartfilename = "FGCOM.sh";
		
		QFile comfile(comStartfilename);
		
		if(comfile.open(QIODevice::WriteOnly))
			
		{
			QByteArray comcontent = "# Start FGCOM - auto generated by fgx. DO NOT MODIFY!\n";
			comcontent.append("\n");
			comcontent.append("nice fgcom -Sfgcom.flightgear.org.uk -p");
			comcontent.append(portFGCOM->text());
			comcontent.append("\n");
			comfile.write(comcontent);
		}
		
		else
		{
			QMessageBox error;
			error.setText("Error: Can not open file FGCOM.sh to write");
			error.exec();
		}
		
		system("chmod 755 FGCOM.sh");
		system("open -a Terminal.app ./FGCOM.sh");
		
	}
	
	// Write commands and arguments to fgStart.sh and run via Terminal.app
	
	QString fgStartfilename = "fgStart.sh";
	
    QFile file(fgStartfilename);
	
	QString argfgfspath = fgfsPath->text();
	QString argfgdatapath = fgdataPath->text();
	
	QString argmultiplayerpathout = pathMultiplayerOut->currentText();
	QString	argmultiplayerport = multiplayerPort->text();
	
	QString argaircraft = airCraft->currentText();
	QString argairport = locationIcao->currentText();

	QString arggeometry = screenSize->currentText();
	
	QString argfgcomport = portFGCOM->text();
	
	QString argcallsign = callSign->text();
	
	QString arglon = Longitude->text();
	QString arglat = Latitude->text();
	
	QString argparkpos = parkPosition->currentText();
	QString	argrunway = runWay->currentText();
	
	QString argtimeofday = timeoftheDay->currentText();
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
	
	
	QString argweather = Weather->currentText(); 
	
	QString argcommandline = commandLine->toPlainText();
	
	QString argloglevel = logLevel->currentText();
	
	
	// Redirect stdout and stderr to logfile
	QString argwritelogdir = QDir::currentPath();
	QString argwritelog;
	argwritelog.append(argwritelogdir);
	argwritelog = " &> fgfslog.log";
	
	
    if(file.open(QIODevice::WriteOnly))
    {
        QByteArray content = "# Start fgfs - generated by fgx. DO NOT MODIFY!\n";
		content.append("\n");
		content.append("#!/bin/bash");
		content.append("\n");
		content.append(argfgfspath);
		content.append("/fgfs");
		content.append(" --fg-root=");
		content.append(argfgdatapath);
		
		if (usecustomScenery->isChecked() == true) {
			content.append(" --fg-scenery=$HOME/Documents/TerrasyncScenery:");
			content.append(argfgdatapath);
			content.append("/Scenery/");
		} else {
			content.append(" --fg-scenery=");
			content.append(argfgdatapath);
			content.append("/Scenery/");
		}
		
		if (useTerraSync->isChecked() == true) {
			QString argatlas = " --atlas=socket,out,5,localhost,5505,udp";
			content.append(argatlas);
		}
		
		if (enableFGCom->isChecked() == true) {
			content.append(" --generic=");
			content.append("socket,out,10,localhost,");
			content.append(argfgcomport);
			content.append(",udp,fgcom");
			
		}
		
		
		if (enableFGCom->isChecked() == true || enableMultiplayer->isChecked() == true) {
			content.append(" --multiplay=out,10,");
			content.append(argmultiplayerpathout);
			content.append(",");
			content.append(argmultiplayerport);
			
			content.append(" --multiplay=in,10,");
			
			system("ipconfig getifaddr en0 > ip.txt");
			
			QFile ipFile("");
			ipFile.setFileName("ip.txt");
			
			if (!ipFile.open(QIODevice::ReadOnly | QIODevice::Text))
				return;
			
			while (!ipFile.atEnd()) {
				
				QString line;
				line.append(ipFile.readAll());
				line.remove("\n");
				content.append(line);	
			}
			
			content.append(",5000");
		}
		
		if (argcallsign != "") {
		content.append(" --callsign=");
		content.append(argcallsign);
		}
		
		content.append(" --aircraft=");
		content.append(argaircraft);
		
		content.append(" --airport=");
		content.append(argairport);
		
		if (useCoordinates->isChecked() == true) {
		
		content.append(" --lon=");
		content.append(arglon);
			
		content.append(" --lat=");
		content.append(arglat);
		}
		
		
		if (runWay->isEnabled() == true) {
			content.append(" --runway=");
			content.append(argrunway);
		}
		
		
		if (parkPosition->isEnabled() == true) {
			content.append(" --parkpos=");
			content.append(argparkpos);
		}
		
	
		
		if (argweather == "real") {
			content.append(" --enable-real-weather-fetch");
		}
		else {
			content.append(" --disable-real-weather-fetch");
		}
		
		if (enableAITraffic->isChecked() == true) {
			content.append(" --enable-ai-traffic");
		}
		else {
			content.append(" --disable-ai-traffic");
		}
		
		content.append(" --enable-ai-models");
		
		if (useMetar->isChecked() == true) {
			content.append(" --metar=");
			QString metartoUse = metarText->toPlainText();
			content.append("\"");
			content.append(metartoUse);
			content.append("\"");
		}
		
		
		if (disableSplashScreen->isChecked() == true) {
			QString argsplashscreen = " --disable-splash-screen";
			content.append(argsplashscreen);
		}
		
		
		if (groupBoxSetTime->isChecked() == true) {
			content.append(" --start-date-lat=");
			content.append(argtime);
		} else {
			content.append(" --timeofday=");
			content.append(argtimeofday);
		}
		
		content.append(" --geometry=");
		content.append(arggeometry);
		
		if (useFullScreen->isChecked() == true) {
			QString argfullscreen = " --enable-fullscreen";
			content.append(argfullscreen);
		}
		
		
		// remove line endings in command line text field and add arguments
		if (commandLine->toPlainText() != "") {
			QString str = commandLine->toPlainText();
			str.replace(QString("\n"), QString(" "));
			content.append(" ");
			content.append(argcommandline);
		}
		
		content.append(" --log-level=");
		content.append(argloglevel);
		
		if (enableLog->isChecked() == true) {
			content.append(argwritelog);
		}
		
		content.append("\n");
		
		
		
        file.write(content);
    }
    else
    {
        QMessageBox error;
        error.setText("Error: Can not open file fgStart.sh to write");
        error.exec();
    } 
	
	system("chmod 755 fgStart.sh");
	system("open -a Terminal.app ./fgStart.sh");
	
}



// Write Settings

void fgx::writeSettings()
{
	//QSettings settings("fgx", "FlightGear Starter OSX");
	
	settings.setValue("fgdataPath", fgdataPath->text());
	
	if (useFGXfgfs->isChecked() == true) {
		settings.setValue("useFGXfgfs", "true");
	}	else {
		settings.setValue("useFGXfgfs", "false");
	}
	
	settings.setValue("fgfsPath", fgfsPath->text());
	settings.setValue("fgcomShell", fgcomShell->text());
	settings.setValue("pathMultiplayerOut", pathMultiplayerOut->currentText());
	settings.setValue("portFGCOM", portFGCOM->text());
	
	if (useTerraSync->isChecked() == true) {
		settings.setValue("useTerraSync", "true");
	}	else {
		settings.setValue("useTerraSync", "false");
	}
	
	if (enableFGCom->isChecked() == true) {
		settings.setValue("enableFGCom", "true");
	}	else {
		settings.setValue("enableFGCom", "false");
	}
	
	if (enableMultiplayer->isChecked() == true) {
		settings.setValue("enableMultiplayer", "true");
	}	else {
		settings.setValue("enableMultiplayer", "false");
	}
	
	if (enableAITraffic->isChecked() == true) {
		settings.setValue("enableAITraffic", "true");
	}	else {
		settings.setValue("enableAITraffic", "false");
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
	settings.setValue("multiplayerPort", multiplayerPort->text());
	settings.setValue("callSign", callSign->text());
	settings.setValue("locationIcao", locationIcao->currentText());
	
	if (usecustomScenery->isChecked() == true) {
		settings.setValue("usecustomScenery", "true");
	}	else {
		settings.setValue("usecustomScenery", "false");
	}
	
	settings.setValue("airCraft", airCraft->currentText());
	
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

// Read Settings

void fgx::readSettings()
{
	
	qDebug() << settings.fg_root() << "  " << settings.fgfs_path();
	fgdataPath->setText(settings.fg_root());
	
	fgfsPath->setText(settings.fgfs_path());
	
	QString useFGXfgfsSet = settings.value("useFGXfgfs").toString();
	if (useFGXfgfsSet == "true") {
		useFGXfgfs->setCheckState(Qt::Checked);
	} else {
		useFGXfgfs->setCheckState(Qt::Unchecked);
	}
	
	QString useTerraSyncSet = settings.value("useTerraSync").toString();
	if (useTerraSyncSet == "true") {
		useTerraSync->setCheckState(Qt::Checked);
	} else {
		useTerraSync->setCheckState(Qt::Unchecked);
	}
	
	QString enableFGComSet = settings.value("enableFGCom").toString();
	if (enableFGComSet == "true") {
		enableFGCom->setCheckState(Qt::Checked);
	} else {
		enableFGCom->setCheckState(Qt::Unchecked);
	}
	
	QString callSignSet = settings.value("callSign").toString();
	callSign->setText(callSignSet);
	
	QString portFGCOMSet = settings.value("portFGCOM").toString();
	portFGCOM->setText(portFGCOMSet);
	
	QString enableMultiplayerSet = settings.value("enableMultiplayer").toString();
	if (enableMultiplayerSet == "true") {
		enableMultiplayer->setCheckState(Qt::Checked);
	} else {
		enableMultiplayer->setCheckState(Qt::Unchecked);
	}
	
	QString enableAITrafficSet = settings.value("enableAITraffic").toString();
	if (enableAITrafficSet == "true") {
		enableAITraffic->setCheckState(Qt::Checked);
	} else {
		enableAITraffic->setCheckState(Qt::Unchecked);
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
	
	QString pathMultiplayerOutSet = settings.value("pathMultiplayerOut").toString();
	pathMultiplayerOut->insertItem(0, pathMultiplayerOutSet);
	pathMultiplayerOut->insertItem(1, "----");
	
	QString multiplayerPortSet = settings.value("multiplayerPort").toString();
	multiplayerPort->setText(multiplayerPortSet);
	

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

	if (settings.value("airCraft").toString() != "") {
		QString airCraftSet = settings.value("airCraft").toString();
		airCraft->insertItem(0, airCraftSet);
		airCraft->insertItem(1, "----");
	} else {
		airCraft->insertItem(0, "c172p");
		airCraft->insertItem(1, "----");
		timeoftheDay->setCurrentIndex(0);
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

// Check built-in FlightGear state
void fgx::on_useFGXfgfs_clicked() {
	checkFGFS();
	checkScenery();
	
	if (useFGXfgfs->isChecked() == true) {
		QString fgdataPathGet = QDir::currentPath();
		fgdataPathGet.append("/fgx.app/Contents/Resources/fgx-fgdata");
		fgdataPath->setText(fgdataPathGet);
		QString fgfsPathGet = QDir::currentPath();
		fgfsPathGet.append("/fgx.app/Contents/MacOS");
		fgfsPath->setText(fgfsPathGet);
	}
	
	checkAircraftList();
	checkAircraftImage();

}

// refresh Aircraft List after setting fgdata path

void fgx::on_set_fgdata_path_Button_clicked() {
	checkAircraftList();
}

void fgx::checkAircraftListStartup() {
	
	QStringList str_list;
	
	QString aircraftPath = fgdataPath->text();
	aircraftPath.append("/Aircraft");
	QDir aircraftDir(aircraftPath);
	aircraftDir.setFilter( QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	
	QStringList entries = aircraftDir.entryList();
	
	for( QStringList::ConstIterator entry=entries.begin(); entry!=entries.end(); ++entry )
		
	{
		// Filter out default dir names, should be a QDir name filter?
		if (*entry != "Instruments" &&  *entry != "Instruments-3d" && *entry != "Generic") {
			str_list << *entry;
		}
	}
	
	//airCraft->clear();
	airCraft->addItems(str_list);
	
}

void fgx::checkAircraftList() {
	
	QStringList str_list;
	
	QString aircraftPath = fgdataPath->text();
	aircraftPath.append("/Aircraft");
	QDir aircraftDir(aircraftPath);
	aircraftDir.setFilter( QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	
	QStringList entries = aircraftDir.entryList();
	
	for( QStringList::ConstIterator entry=entries.begin(); entry!=entries.end(); ++entry )
		
	{
		// Filter out default dir names, should be a QDir name filter?
		if (*entry != "Instruments" &&  *entry != "Instruments-3d" && *entry != "Generic") {
			str_list << *entry;
		}
	}
	
	airCraft->clear();
	airCraft->addItems(str_list);
	
}

void fgx::on_tabs_currentChanged() {
	checkScenery();
}


// Check aircraft image
void fgx::checkAircraftImage() {
	
	if (useFGXfgfs->isChecked() == true) {
	
	QString fileName = QDir::currentPath();
		fileName.append("/fgx.app/Contents/Resources/fgx-fgdata/Aircraft/");
		fileName.append(airCraft->currentText());
		fileName.append("/thumbnail.jpg");
	
	if (!fileName.isEmpty()) {
		QImage image(fileName);
		if (image.isNull()) {
			// TODO - make sure this shows a null image or alike and solve problem another way
			/*
			QMessageBox::information(this, tr("Image Viewer"),
									 tr("Please click on refresh on tab \"path/programs\". Cannot load %1.").arg(fileName));
			*/
			return;
		}
			aircraftImage->setPixmap(QPixmap::fromImage(image));
		
			}
	}
	
	else {
		QString fileName = fgdataPath->text();
		fileName.append("/Aircraft/");
		fileName.append(airCraft->currentText());
		fileName.append("/thumbnail.jpg");
		
		if (!fileName.isEmpty()) {
			QImage image(fileName);
			if (image.isNull()) {
				QMessageBox::information(this, tr("Image Viewer"),
										 tr("Please check path to your FlightGear data folder and click on refresh. Cannot load %1.").arg(fileName));
				return;
			}
			aircraftImage->setPixmap(QPixmap::fromImage(image));
			
		}
	}
			
}

void fgx::on_airCraft_activated() {
	checkAircraftImage();
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

// built-in check, disable path line edits

void fgx::checkFGFS() {
	
	Qt::CheckState defaultstate;
	
	defaultstate = useFGXfgfs->checkState();
	if (defaultstate == Qt::Checked) {
		fgfsPath->setEnabled(false);
		fgdataPath->setEnabled(false);
	} else {
		fgfsPath->setEnabled(true);
		fgdataPath->setEnabled(true);
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


// Write settings on close
void fgx::closeEvent(QCloseEvent *event)
{
	writeSettings();
	settings.saveWindow(this);
	event->accept();
}

void fgx::on_buttonTest_clicked(){
	qDebug() << "test";
	SettingsDialog *settingsDialog = new SettingsDialog();
	settingsDialog->exec();
}

