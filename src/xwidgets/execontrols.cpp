
#include <QDebug>

#include <QtCore/QProcess>

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
	QGridLayout *layout = new QGridLayout();
	setLayout(layout);
	layout->setContentsMargins(5,5,5,5);
	layout->setSpacing(5);

	int row = 0;
	QString buttStyle("padding: 2px;"); //* make button smaller. this need to go into global style sheet said pedro


	//** Stop Button
	buttonStop = new QPushButton();
	buttonStop->setText(tr("Stop"));
	buttonStop->setIcon(QIcon(":/icon/stop_disabled"));
	buttonStop->setStyleSheet(buttStyle);
	layout->addWidget(buttonStop, row, 0, 1, 1);
	connect(buttonStop, SIGNAL(clicked()), this, SLOT(on_stop_clicked()));

	//** Start Button
	buttonStart = new QPushButton();
	buttonStart->setText(tr("Start"));
	buttonStart->setIcon(QIcon(":/icon/start_enabled"));
	buttonStart->setStyleSheet(buttStyle);
	layout->addWidget(buttonStart, row, 1, 1, 1);
	//* connection is done in fgx not here..

	//** Pid Label, this might end up being LCD maybe
	//labelPid = new QLabel("---");
	//labelPid->setStyleSheet("font-size: 8pt; font-family: monospaced;");
	//layout->addWidget(labelPid, row, 2, 1, 1);

	//** Refresh Button
	buttonRefresh = new QPushButton();
	buttonRefresh->setIcon(QIcon(":/icon/refresh"));
	buttonRefresh->setStyleSheet(buttStyle);
	layout->addWidget(buttonRefresh, row, 3, 1, 1);
	connect(buttonRefresh, SIGNAL(clicked()), this, SLOT(on_refresh_clicked()));

	row++;
	statusBar = new QStatusBar();
	layout->addWidget(statusBar, row, 0, 1, 4);

}

//==========================================================================
// GUI Events
//==========================================================================

//** Stop Button clicked so kill process
void ExeControls::on_stop_clicked(){
	qDebug() << "stop clicked";
	if(get_pid() >0){
		this->kill_pid();
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

