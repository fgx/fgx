
#include <QDebug>

#include <QtCore/QProcess>
#include <QtCore/QTimer>

#include <QtGui/QHBoxLayout>

#include "execontrols.h"

/* The idea of this is to have a widget that both launches a "service"..
   But also can check and kill it, on linux or mac.. Windows got no idea
   WANTED: expert advice
*/

ExeControls::ExeControls(QString title, QString exeCmd, QWidget *parent) :
    QGroupBox(parent)
{
	exe_name = exeCmd;
	setTitle(title);

	//** Grid layout just in case..
	QVBoxLayout *layout = new QVBoxLayout();
	setLayout(layout);
	layout->setContentsMargins(5,5,5,5);
	layout->setSpacing(10);

	QString buttStyle("padding: 8px;"); //* make button smaller. this need to go into global style sheet said pedro

	//** Buttons layout
	QHBoxLayout *buttlay = new QHBoxLayout();
	layout->addLayout(buttlay);

	//** Stop Button
	buttonStop = new QPushButton();
	buttonStop->setText(tr(" Stop "));
	buttonStop->setIcon(QIcon(":/icon/stop_disabled"));
	buttonStop->setStyleSheet(buttStyle);
	buttlay->addWidget(buttonStop);
	connect(buttonStop, SIGNAL(clicked()), this, SLOT(on_stop_clicked()));

	//** Start Button
	buttonStart = new QPushButton();
	buttonStart->setText(tr(" Start "));
	buttonStart->setIcon(QIcon(":/icon/start_enabled"));
	buttonStart->setStyleSheet(buttStyle);
	buttlay->addWidget(buttonStart);
	//* connection is done in fgx not here..



	//** Bottom layout
	QHBoxLayout *bottlay = new QHBoxLayout();
	layout->addLayout(bottlay);

	//* Status Bar
	statusBar = new QStatusBar();
	statusBar->setSizeGripEnabled(false);
	statusBar->setContentsMargins(5,5,5,5);
	bottlay->addWidget(statusBar);

	//** Refresh Button
	buttonRefresh = new QPushButton();
	buttonRefresh->setIcon(QIcon(":/icon/refresh"));
	buttonRefresh->setFlat(true);
	buttonRefresh->setStyleSheet("padding: 0px;");
	bottlay->addWidget(buttonRefresh);
	connect(buttonRefresh, SIGNAL(clicked()), this, SLOT(on_refresh_clicked()));

}


//==========================================================================
// Start Executable
// - not interested in Pid as anything could have happened elsewhere
//==========================================================================
void ExeControls::start(QString command_line){
	int start = QProcess::startDetached(command_line);
	Q_UNUSED(start);
	if(start){
		statusBar->showMessage("Starting", 2000);
		QTimer::singleShot(2000,this, SLOT(on_refresh_clicked()));
	}else{
		statusBar->showMessage("Failed", 4000);
	}
}



//==========================================================================
// GUI Events
//==========================================================================

//** Stop Button clicked so kill process
void ExeControls::on_stop_clicked(){
	//qDebug() << "stop clicked";
	if(get_pid() > 0){
		this->kill_pid();
		statusBar->showMessage("Killed Process", 2000);
	}else{
		statusBar->showMessage("Not found", 2000);
	}
}

//** Refresh Button clicked
void ExeControls::on_refresh_clicked(){
	update_pid();
}


//==========================================================================
// Return process ID of executable on mac/linux - TODO windows ?
//==========================================================================
int ExeControls::get_pid(){
	QStringList args;
	// i.e. how to get pid of fgfs OSX: % ps axc|awk "{if (\$5==\"fgfs\") print \$1}" 
	args << exe_name;
	QProcess process;
	process.start("pidof", args, QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished();
		QString ok_result = process.readAllStandardOutput();
		//QString error_result = process.readAllStandardError(); Unused atmo
		QString pid = ok_result.trimmed();
		if(pid.length() > 0){
			return pid.toInt();
		}
	}
	return 0;
}

//==========================================================================
// Check if app is running and update label
//==========================================================================
void ExeControls::update_pid(){
	int pid = get_pid();
	if(pid == 0){
		statusBar->showMessage("--");
	}else{
		statusBar->showMessage(QString::number(pid));
	}
}

//==========================================================================
// Kills process if running
//==========================================================================
void ExeControls::kill_pid(){
	int pid = get_pid();
	if(pid == 0){
		return;
	}
	QStringList killargs;
	killargs << "-9" << QString::number(pid);
	QProcess::startDetached("kill", killargs);
}

