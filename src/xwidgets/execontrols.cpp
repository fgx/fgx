
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QTimer>

#include <QtGui/QHBoxLayout>

#include "execontrols.h"
//#include "panes/advancedoptionswidget.h"
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
	QVBoxLayout *buttlay = new QVBoxLayout();
	layout->addLayout(buttlay);

	//** Stop Button
	buttonStop = new QPushButton();
	buttonStop->setText(tr(" Stop "));
	buttlay->addWidget(buttonStop);
	buttonStop->setEnabled(false);
	buttonStop->setMinimumSize(QSize( 100, 20));
	connect(buttonStop, SIGNAL(clicked()), this, SLOT(on_stop_clicked()));

	//** Start Button
	buttonStart = new QPushButton();
	buttonStart->setText(tr(" Start "));
	buttonStart->setMinimumSize(QSize( 100, 20)); 
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
