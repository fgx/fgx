
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
	QGridLayout *layout = new QGridLayout();
	setLayout(layout);
	layout->setContentsMargins(5,5,5,5);
	layout->setSpacing(5);
	int row = 0;

	//** First Row is the label with the "status".. colors set in stylsheet

	//* Second row is the Push buttons
	//#row++;
	buttonStop = new QPushButton();
	buttonStop->setText(tr("Stop"));
	buttonStop->setIcon(QIcon(":/icon/stop_disabled"));
	//buttonStop->setStyleSheet("padding: 0px;");
	layout->addWidget(buttonStop, row, 0, 1, 1);
	connect(buttonStop, SIGNAL(clicked()), this, SLOT(on_stop_clicked()));

	buttonStart = new QPushButton();
	buttonStart->setText(tr("Start"));
	buttonStart->setIcon(QIcon(":/icon/start_enabled"));
	//buttonStart->setStyleSheet("padding: 0px;");
	layout->addWidget(buttonStart, row, 1, 1, 1);
	///connect(buttonStart, SIGNAL(clicked()), this, SLOT(on_start_clicked()));

	labelPid = new QLabel("---");
	labelPid->setStyleSheet("font-size: 8pt; font-family: monospaced;");
	layout->addWidget(labelPid, row, 2, 1, 1);

	buttonRefresh = new QPushButton();
	//buttonRefresh->setText(tr("Start"));
	buttonRefresh->setIcon(QIcon(":/icon/refresh"));
	//buttonStart->setStyleSheet("padding: 0px;");
	layout->addWidget(buttonRefresh, row, 3, 1, 1);


}


void ExeControls::on_stop_clicked(){
	qDebug() <<  "stop";
	kill_pid();
}
/*
void ExeControls::on_start_clicked(){
	qDebug() <<  "start kill";

}
*/



//==========================================================================
// Return process ID of executable on mac/linue - TODO windows ?
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
// Check if app is running and updates label
//==========================================================================
void ExeControls::update_pid(){
	int pid = get_pid();
	if(pid == 0){
		labelPid->setText("--");
	}else{
		labelPid->setText(QString::number(pid));
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
	QString command("kill ");
	command.append(pid);
	QProcess::startDetached(command);
}

