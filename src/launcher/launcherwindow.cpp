
#include <QDebug>
#include <QtCore/QUrl>
#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtCore/QProcess>

#include <QtGui/QApplication>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QDesktopServices>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QStyleFactory>
#include <QtGui/QMessageBox>


#include "launcher/launcherwindow.h"

LauncherWindow::LauncherWindow(MainObject *mainOb, QWidget *parent)
    : QMainWindow(parent)
{

    mainObject = mainOb;
	setProperty("settings_namespace", QVariant("launcher_window"));
	mainObject->settings->restoreWindow(this);

	setWindowTitle(QCoreApplication::applicationName().append(" - ").append(QCoreApplication::applicationVersion()));
	setWindowIcon(QIcon(":/icon/favicon"));
    //setWindowFlags(  Qt::WindowStaysOnTopHint);

	//====================================================
	//** Setup Menus
	//====================================================
	QMenuBar *menuBar = new QMenuBar();
	setMenuBar(menuBar);

	//** File Menu
	QMenu *menuFile = new QMenu(tr("File"));
	menuBar->addMenu(menuFile);
	QAction *actionQuit = menuFile->addAction(QIcon(":/icon/quit"), tr("Quit"), this, SLOT(on_quit()));
	actionQuit->setIconVisibleInMenu(true);

	//** Style Menu
	QMenu *menuStyle = new QMenu(tr("Style"));
	menuBar->addMenu(menuStyle);
	// TODO make defult style from xplatform.. in mainObject->settings-> (pedro)
	QApplication::setStyle( QStyleFactory::create(mainObject->settings->value("gui_style","Macintosh (aqua)").toString()) );
	actionGroupStyle = new QActionGroup(this);
	actionGroupStyle->setExclusive(true);
	connect(actionGroupStyle, SIGNAL(triggered(QAction*)), this, SLOT(mainObject->settings->(QAction*)));
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

	//** File Menu
	QMenu *menuHelp = new QMenu(tr("Help"));
	menuBar->addMenu(menuHelp);
	menuHelp->addAction(tr("About FGX"), this, SLOT(on_about_fgx()));
	menuHelp->addAction(tr("About Qt"), this, SLOT(on_about_qt()));


	//====================================================
	//** Main Central Widget and Layout
	//====================================================
    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    QVBoxLayout *mainVBox = new QVBoxLayout();
    mainVBox->setContentsMargins(0,0,0,0);
    mainVBox->setSpacing(0);
    mainWidget->setLayout(mainVBox);

	//====================================================
	//** Header Banner
	//====================================================
	QString header_style("padding: 10px 0px 0px 370px; vertical-align: top;	 background-image: url(':/images/fgx-logo'); background-repeat: no-repeat; background-color: white;");
    headerLabel = new QLabel(this);
	headerLabel->setFixedHeight(100);
	headerLabel->setStyleSheet(header_style);
	mainVBox->addWidget(headerLabel, 0);

	splitter = new QSplitter();
	mainVBox->addWidget(splitter, 20);

	//====================================================
	//** Main TabWidget with Widgets
	//====================================================
    tabWidget = new QTabWidget(this);
	splitter->addWidget(tabWidget);
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(on_tab_changed(int)));


	//* Core Settings
	coreSettingsWidget = new CoreSettingsWidget(mainObject);
	tabWidget->addTab(coreSettingsWidget, tr("Core Settings"));
	connect(coreSettingsWidget->groupBoxTerraSync, SIGNAL(clicked()), this, SLOT(on_group_box_terrasync_clicked()));

	//* Time / Weather Widget
	timeWeatherWidget = new TimeWeatherWidget(mainObject);
	tabWidget->addTab(timeWeatherWidget, tr("Time and Weather"));


	//* Aircraft Widget
	aircraftWidget = new AircraftWidget(mainObject);
	tabWidget->addTab(aircraftWidget, tr("Aircraft"));
	//#connect(aircraftWidget, SIGNAL(set_arg(QString,QString,QString)), this, SLOT(set_arg(QString,QString,QString)));


	//** Airports Tab
	airportsWidget = new AirportsWidget(mainObject);
	tabWidget->addTab(  airportsWidget, tr("Airports"));


	//* Network Tab
	networkWidget = new NetworkWidget(mainObject);
	tabWidget->addTab( networkWidget, tr("Network"));


	//* Advanced Options
	advancedOptionsWidget = new AdvancedOptionsWidget(mainObject);
	tabWidget->addTab( advancedOptionsWidget, tr("Advanced Options"));


	//* Output + Preview
	outputPreviewWidget = new OutputPreviewWidget(mainObject);
	tabWidget->addTab( outputPreviewWidget, tr("Output / Preview"));
	connect(outputPreviewWidget->buttonCommandPreview, SIGNAL(clicked()), this, SLOT(on_command_preview()));
	connect(outputPreviewWidget->buttonCommandHelp, SIGNAL(clicked()), this, SLOT(on_command_help()));


	//========================================================================================
	//** EXE "Controls" into a bottom Bar
	//========================================================================================
	QHBoxLayout *bottomActionLayout = new QHBoxLayout();
	mainVBox->addLayout(bottomActionLayout);
	bottomActionLayout->addStretch(100); // force to right

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


	//=================================================
	//** Restore Last tab
	tabWidget->setCurrentIndex( mainOb->settings->value("launcher_last_tab", 0).toInt() );

	//====================================================================================
	//* Problem:  Qt Has no "Show event" for a "widget", so we need to present Widgets first
	//** and then initialise. This is achieved with a timer that triggers in a moment

	// TODO  - disable widget till sane in initialize()
	//centralWidget()->setDisabled(true);
	QTimer::singleShot(500, this, SLOT(initialize()));

}

LauncherWindow::~LauncherWindow()
{
}


void LauncherWindow::closeEvent(QCloseEvent *event){
	Q_UNUSED(event);
	save_settings();
	mainObject->settings->saveWindow(this);
	mainObject->settings->sync();
	event->accept();
}

void LauncherWindow::on_menu_style(QAction *action){
	mainObject->settings->setValue("gui_style", action->text());
	QApplication::setStyle(QStyleFactory::create(action->text()));
}

void LauncherWindow::on_tab_changed(int idx){
	mainObject->settings->setValue("launcher_last_tab", idx);
		//if(index == 6){
		//	on_buttonViewCommand_clicked();
		//}

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



//=====================================================================================================================
// Updates as the external processes in the "command buttons"
//=======================================================================================================================
void LauncherWindow::update_pids(){
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



void LauncherWindow::on_about_fgx(){
	QString txt;
	txt.append("<html><body><p>FGX FlightGear Launcher</b></p>");
	txt.append("<p>&copy; 2011 Gral aka Yves Sablonier and Pete Morgan</p>");
	txt.append("<p><a href='http://www.gnu.org/licenses/gpl-2.0.txt'>GPL2</a></p>");
	txt.append("<p><a href='http://wiki.flightgear.org'>FlightGear</a></p>");
	txt.append("</body></html>");
	QMessageBox::about(this, "About FGX", txt);
}

void LauncherWindow::on_about_qt(){
	QMessageBox::aboutQt(this, "About Qt");
}

void LauncherWindow::on_quit(){
	QApplication::quit();
}




//================================================================================
// Save Settings
//================================================================================

void LauncherWindow::save_settings()
{
	//## NB: fgfs path and FG_ROOT are saves in SettingsDialog ##
	coreSettingsWidget->save_settings();
	aircraftWidget->save_settings();
	airportsWidget->save_settings();
	networkWidget->save_settings();
	advancedOptionsWidget->save_settings();




	/* TODO
	mainObject->settings->setValue("year", year->text());
	mainObject->settings->setValue("month", month->text());
	mainObject->settings->setValue("day", day->text());
	mainObject->settings->setValue("hour", hour->text());
	mainObject->settings->setValue("minute", minute->text());
	mainObject->settings->setValue("second", second->text());
	*/

	//* Time
	mainObject->settings->setValue("timeofday", timeWeatherWidget->buttonGroupTime->checkedButton()->property("value").toString());
	//mainObject->settings->setValue("set_time", groupBoxSetTime->isChecked());


	//* Weather
	mainObject->settings->setValue("weather", timeWeatherWidget->buttonGroupMetar->checkedId());
	mainObject->settings->setValue("metar", timeWeatherWidget->txtMetar->toPlainText());

	//* Advanced

	mainObject->settings->sync();
}

//================================================================================
// Load Settings
//================================================================================

void LauncherWindow::load_settings()
{



	coreSettingsWidget->load_settings();
	aircraftWidget->load_settings();
	airportsWidget->load_settings();
	networkWidget->load_settings();
	advancedOptionsWidget->load_settings();


	exeTerraSync->setEnabled( mainObject->settings->value("use_terrasync").toBool() );




	//** Time Of Day - TODO
	/*
	bool setTime = mainObject->settings->value("set_time").toBool();
	groupBoxSetTime->setChecked(setTime);

	QString yearSet = mainObject->settings->value("year").toString();
	year->setText(yearSet);
	QString monthSet = mainObject->settings->value("month").toString();
	month->setText(monthSet);
	QString daySet = mainObject->settings->value("day").toString();
	day->setText(daySet);
	QString hourSet = mainObject->settings->value("hour").toString();
	hour->setText(hourSet);
	QString minuteSet = mainObject->settings->value("minute").toString();
	minute->setText(minuteSet);
	QString secondSet = mainObject->settings->value("second").toString();
	second->setText(secondSet);
	*/

	QString tod = mainObject->settings->value("timeofday", "real").toString();
	QList<QAbstractButton *> todButtons = timeWeatherWidget->buttonGroupTime->buttons();
	for (int i = 0; i < todButtons.size(); ++i) {
		if(todButtons.at(i)->property("value").toString() == tod){
			todButtons.at(i)->setChecked(true);
		}
		//todButtons.at(i)->setEnabled(!setTime);
	 }




	//** Weather
	int weather = mainObject->settings->value("weather").toInt();
	timeWeatherWidget->buttonGroupMetar->button(weather)->setChecked(true);

	timeWeatherWidget->txtMetar->setPlainText(mainObject->settings->value("metar").toString());
	timeWeatherWidget->txtMetar->setEnabled(weather == 2);




}


//=======================================================================================================================
// initialize and  Setup
//=======================================================================================================================
void LauncherWindow::initialize(){

	//** First load the settings, and check the "paths" to fg_root and fg are sane
	load_settings();
	if(!mainObject->settings->paths_sane()){
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


//==============================================
//** View Buttons
void LauncherWindow::on_command_preview(){
	if(!validate()){
		return;
	}
	QString str = QString(mainObject->settings->fgfs_path()).append(" ").append( fg_args());
	outputPreviewWidget->txtPreviewOutput->setPlainText(str);
}

void LauncherWindow::on_command_help(){
	QProcess process;
	QStringList args;
	args << "-h" << "-v" << QString("--fg-root=").append(mainObject->settings->fg_root());
	process.start(mainObject->settings->fgfs_path(), args, QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished();
		QString ok_result = process.readAllStandardOutput();
		QString error_result = process.readAllStandardError();
		outputPreviewWidget->txtPreviewOutput->setPlainText(ok_result);
	}
}


//=======================================================================================================================
//* Validate
//=======================================================================================================================
bool LauncherWindow::validate(){
	int TIMEOUT = 5000;
	QString v;

	v = aircraftWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(aircraftWidget));
		statusBar()->showMessage(v, TIMEOUT);
		return false;
	}

	v = airportsWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(airportsWidget));
		statusBar()->showMessage(v, TIMEOUT);
		return false;
	}

	v = networkWidget->validate();
	if(v != ""){
		tabWidget->setCurrentIndex( tabWidget->indexOf(networkWidget));
		statusBar()->showMessage(v, TIMEOUT);
		return false;
	}

	if(coreSettingsWidget->groupBoxTerraSync->isChecked() and coreSettingsWidget->txtTerraSyncPath->text().length() == 0){
		tabWidget->setCurrentIndex( tabWidget->indexOf(coreSettingsWidget) );
		coreSettingsWidget->txtTerraSyncPath->setFocus();
		statusBar()->showMessage("Need a Terrasync directory", TIMEOUT);
		return false;
	}
	return true;
}

void LauncherWindow::on_group_box_terrasync_clicked(){
	mainObject->settings->setValue("use_terrasync", coreSettingsWidget->groupBoxTerraSync->isChecked());
	mainObject->settings->sync();
	exeTerraSync->setEnabled(coreSettingsWidget->groupBoxTerraSync->isChecked());
}
