
//#include <QDebug>

#include <QFile>
#include <QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QTimer>

#include <QtGui/QHBoxLayout>

#include "execontrols.h"
#include "launcher/launcherwindow.h"
#include "utilities/utilities.h"

ExeControls::ExeControls(QString title, QWidget *parent) :
    QGroupBox(parent)
{
	//exe_name = exeCmd;
	setTitle(title);

	//** Grid layout just in case..
	QVBoxLayout *layout = new QVBoxLayout();
	setLayout(layout);
	layout->setContentsMargins(5,5,5,5);
	layout->setSpacing(10);


	//** Buttons layout
	QHBoxLayout *buttlay = new QHBoxLayout();
	layout->addLayout(buttlay);

	//** Stop Button
	buttonStop = new QToolButton();
	buttonStop->setText("Stop");
	buttonStop->setAutoRaise(true);
	buttonStop->setIcon(QIcon(":/icon/stop"));
	buttonStop->setStyleSheet("padding: 0px;");
	buttonStop->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttonStop->setEnabled(false);
	connect(buttonStop, SIGNAL(clicked()), this, SLOT(on_stop_clicked()));
	buttlay->addWidget(buttonStop);

	//** Start Button
	buttonStart = new QToolButton();
	buttonStart->setText("Start");
	buttonStart->setAutoRaise(true);
	buttonStart->setIcon(QIcon(":/icon/start"));
	buttonStart->setStyleSheet("padding: 0px;");
	buttonStart->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttlay->addWidget(buttonStart);
	connect(buttonStart, SIGNAL(clicked()), this, SLOT(on_start_clicked()));
	

}


//== Start
void ExeControls::on_start_clicked(){
	emit start();
}


//== Stop
void ExeControls::on_stop_clicked(){
	emit stop();
}


//== set_running
void ExeControls::set_running(bool running){
	buttonStart->setEnabled(running == false);
	buttonStop->setEnabled(running == true);
}
