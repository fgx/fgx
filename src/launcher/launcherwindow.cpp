
#include <QDebug>
#include <QtCore/QUrl>

#include <QtGui/QVBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QDesktopServices>

#include "launcher/launcherwindow.h"

LauncherWindow::LauncherWindow(MainObject *mainOb, QWidget *parent)
    : QMainWindow(parent)
{

    mainObject = mainOb;
	setProperty("settings_namespace", QVariant("launcher_window"));
	mainObject->settings->restoreWindow(this);

	setWindowTitle("fgX Launcher");
	setWindowIcon(QIcon(":/icon/favicon"));
    //setWindowFlags(  Qt::WindowStaysOnTopHint);


    //* MainWidget and MainLayout
    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);


    QVBoxLayout *mainVBox = new QVBoxLayout();
    mainVBox->setContentsMargins(0,0,0,0);
    mainVBox->setSpacing(0);
    mainWidget->setLayout(mainVBox);

	//** Header Banner across the top =========================
	QString header_style("padding: 10px; font-size: 11pt; font-weight: bold; background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 white, stop: 1 #F0DD17);");
    headerLabel = new QLabel(this);
	headerLabel->setText("fgX Launcher");
	headerLabel->setStyleSheet(header_style);
	mainVBox->addWidget(headerLabel, 0);

	splitter = new QSplitter();
	mainVBox->addWidget(splitter, 20);

	//** Main Tab =========================
    tabWidget = new QTabWidget(this);
	splitter->addWidget(tabWidget);
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tab_changed(int)));

	//==================================================
	// Widgets

	//* Core Settings
	coreSettingsWidget = new CoreSettingsWidget(mainObject);
	tabWidget->addTab(coreSettingsWidget, tr("Core Settings"));

	//* Aircraft Widget
	aircraftWidget = new AircraftWidget(mainObject);
	tabWidget->addTab(aircraftWidget, tr("Aircraft"));
	//#connect(aircraftWidget, SIGNAL(set_arg(QString,QString,QString)), this, SLOT(set_arg(QString,QString,QString)));


	//* Options
	//mainOptionsWidget = new MainOptionsWidget(this);
	//tabWidget->addTab(mainOptionsWidget, tr("Main Options"));
	//connect(mainOptionsWidget, SIGNAL(set_arg(QString,QString,QString)), this, SLOT(set_arg(QString,QString,QString)));

	//* MpServers
	//mpServersWidget = new MpServersWidget(mainObject);
	//tabWidget->addTab(mpServersWidget, tr("Network"));
	//connect(mpServersWidget, SIGNAL(set_arg(QString,QString,QString)), this, SLOT(set_arg(QString,QString,QString)));









	//* Airports Widget
	//airportsWidget = new AirportsWidget(this);
   // tabWidget->addTab(airportsWidget, tr("Airports"));
	//#connect(airportsWidget, SIGNAL(set_arg(QString,QString,QString)), this, SLOT(set_arg(QString,QString,QString)));



	//========================================================================================
	//** Insert the "Controls" into the bottom Bar
	//========================================================================================
	QHBoxLayout *bottomActionLayout = new QHBoxLayout();
	mainVBox->addLayout(bottomActionLayout);

	exeFgCom = new ExeControls("FgCom", "fgcom");
	bottomActionLayout->addWidget(exeFgCom);
	connect(	exeFgCom->buttonStart, SIGNAL(clicked()),
				this, SLOT(on_start_fgcom_clicked())
	);

	exeTerraSync = new ExeControls("TerraSync", "terrasync");
	bottomActionLayout->addWidget(exeTerraSync);
	connect(	exeTerraSync->buttonStart, SIGNAL(clicked()),
				this, SLOT(on_start_terrasync_clicked())
	);

	exeFgfs = new ExeControls("FlightGear", "fgfs");
	bottomActionLayout->addWidget(exeFgfs);
	connect(	exeFgfs->buttonStart, SIGNAL(clicked()),
				this, SLOT(on_start_fgfs_clicked())
	);

	//set_paths();

}

LauncherWindow::~LauncherWindow()
{
}



//void LauncherWindow::closeEvent( event ){
//   //     self.main.mainObject->settings->save_window( "account_dialog", self )
//    qDebug() << "close";
//}

void LauncherWindow::closeEvent(QCloseEvent *event){
	Q_UNUSED(event);
    mainObject->settings->saveWindow(this);
}



void LauncherWindow::on_tab_changed(int tab_index){
	Q_UNUSED(tab_index);
	//TODO maybe we dont need this..
	// pusedo code
	// if isistance(widget, FooClass) : load()
}


void LauncherWindow::setup_tree(){
	return;
	/*
	QStringList args;
	args << "fgfs" << "--fg-root" << "--aircraft" << "--airport" << "--runway" << "--callsign";

	for (int i = 0; i < args.size(); ++i){
		QTreeWidgetItem *item = new QTreeWidgetItem();
		item->setText(0, args.at(i));
		tree->addTopLevelItem(item);
	}
	*/
}


void LauncherWindow::set_arg(QString action, QString arg, QString val){
	qDebug() << "set_arg" << action << " " << arg << " " << val;
	/*
	QList<QTreeWidgetItem *> items = tree->findItems(arg, Qt::MatchExactly, 0);

	if(action == "remove"){
		if(items.count() == 0){ //* item not there
			return;
		}
		QTreeWidgetItem *removeItem = items.first();
		tree->invisibleRootItem()->removeChild(removeItem);
		return;
	}

	if(action == "set"){
		if(items.count() == 0){
			QTreeWidgetItem *newItem = new QTreeWidgetItem(); //* add Item if not exist
			newItem->setText(C_ARG, arg);
			newItem->setTextAlignment(C_ARG, Qt::AlignRight);
			newItem->setText(C_VAL, val);
			tree->addTopLevelItem(newItem);
			return;
		}else{
			QTreeWidgetItem *item =  items.first();  //* update existing
			item->setText(C_VAL, val);
			return;
		}

	}
	qDebug() << "UNHANDLED";
	*/
}

void LauncherWindow::set_paths(){
	set_arg("set", "fgfs", mainObject->settings->fgfs_path());
	set_arg("set", "--fg-root=", mainObject->settings->fg_root());
}




//=======================================================================================================================
// Start FlightGear
//=======================================================================================================================
void LauncherWindow::on_start_fgfs_clicked() {

	QString command = mainObject->settings->fgfs_path();
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
void LauncherWindow::on_start_terrasync_clicked(){

	QStringList args;
	args << mainObject->settings->terrasync_exe_path() << "-p" << "5500" << "-S" << "-d" << mainObject->settings->terrasync_sync_path();
	QString command_line("nice");
	command_line.append(" ").append(args.join(" "));
	// qDebug() << command_line;
	exeTerraSync->start(command_line);
}
//=======================================================================================================================
// Start FGCom
//=======================================================================================================================
void LauncherWindow::on_start_fgcom_clicked(){
	QString command_line = mainObject->settings->fgcom_exe_path();
	command_line.append(" ");
	QStringList args;
	args << mainObject->settings->value("fgcom_no").toString() << mainObject->settings->value("fgcom_port").toString();
	command_line.append( args.join(" ") );
	// qDebug() << command_line;
	exeFgCom->start(command_line);
}



QString LauncherWindow::fg_args(){

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
	args << QString("--fg-root=").append(mainObject->settings->fg_root());





	//** Scenery Path
	args << QString("--fg-scenery=%1").arg(mainObject->settings->scenery_path());



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


