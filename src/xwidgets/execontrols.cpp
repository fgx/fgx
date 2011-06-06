
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QTimer>

#include <QtGui/QHBoxLayout>

#include "execontrols.h"

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
	buttonStop->setEnabled(false);
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
	
	QString statusStyle("font-size: 10px"); //* make font smaller. this need to go into global style sheet said yves
	statusBar = new QStatusBar();
	statusBar->setSizeGripEnabled(false);
	statusBar->setContentsMargins(5,5,5,5);
	statusBar->setStyleSheet(statusStyle);
	bottlay->addWidget(statusBar);

}


// Read standard output
void ExeControls::readOutput()

{
	QString output = P->readAllStandardOutput();
	//Should go into log file
	statusBar->showMessage(output, 1000);
}


// Read standard error
void ExeControls::readError()

{
	QString error = P->readAllStandardError();
	//Should go into log file
	statusBar->showMessage(error, 1000);
}


//==========================================================================
// Start Executable
// and connecting console output/errors for log
//==========================================================================
void ExeControls::start(QString command_line){
	
	P = new QProcess();
	
	connect( P, SIGNAL(readyReadStandardOutput()),this, SLOT(readOutput()));
	connect( P, SIGNAL(readyReadStandardError()),this, SLOT(readError()));
	
	P->start( QString(command_line));
	buttonStart->setEnabled(false);
	buttonStop->setEnabled(true);
	
	}



//==========================================================================
// Stop Executable
// 
//==========================================================================
void ExeControls::on_stop_clicked(){
	//danger!danger!
	P->kill();
	buttonStop->setEnabled(false);
	buttonStart->setEnabled(true);
}

//** Refresh Button clicked
void ExeControls::on_refresh_clicked(){
	//update_pid();
}


