
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


	//** Buttons layout
	QHBoxLayout *buttlay = new QHBoxLayout();
	layout->addLayout(buttlay);

	//** Stop Button
	buttonStop = new QPushButton();
	buttonStop->setText(tr(" Stop "));
	buttlay->addWidget(buttonStop);
	buttonStop->setEnabled(false);
	connect(buttonStop, SIGNAL(clicked()), this, SLOT(on_stop_clicked()));

	//** Start Button
	buttonStart = new QPushButton();
	buttonStart->setText(tr(" Start "));
	buttlay->addWidget(buttonStart);
	//* connection is done in fgx not here..
	



	//** Bottom layout
	QHBoxLayout *bottlay = new QHBoxLayout();
	layout->addLayout(bottlay);
	
	//* Status Bar
	
	QString statusStyle("font-size: 9px; color: #666666");
	
	statusBar = new QStatusBar();
	statusBar->setSizeGripEnabled(false);
	statusBar->setMinimumWidth(200);
	statusBar->setMinimumHeight(30);
	statusBar->setContentsMargins(0,0,0,0);
	statusBar->setStyleSheet(statusStyle);
	bottlay->addWidget(statusBar);

}


// Read standard output
void ExeControls::readOutput()

{
	QString fgxoutput = P->readAllStandardOutput();
	
	// show some lines in statusbar as feedback
	statusBar->showMessage(fgxoutput, 1000);

	// write output to fgxlog
	QFile file("fgxlog.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
		return;
	
	// show exe name in log
	QTextStream out(&file);
	out << ExeControls::title() << ": " << fgxoutput;
}


// Read standard error
void ExeControls::readError()

{
	QString fgxerror = P->readAllStandardError();
	
	// show some lines in statusbar as feedback
	statusBar->showMessage(fgxerror, 1000);
	
	// write errors to fgxlog
	QFile file("fgxlog.txt");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
		return;
	
	// show exe name in log
	QTextStream out(&file);
	out << ExeControls::title() << ": " << fgxerror;
}


//==========================================================================
// Start Executable
// and connecting console output/errors for log
//==========================================================================
void ExeControls::start(QString command_line){
	
	P = new QProcess();
	
	connect( P, SIGNAL(readyReadStandardOutput()),this, SLOT(readOutput()));
	connect( P, SIGNAL(readyReadStandardError()),this, SLOT(readError()));
	
	//Geoff says we need a OSG or LD_LIBRARY_PATH option once!
	//QStringList env = QProcess::systemEnvironment();
	//env << "LD_LIBRARY_PATH=/home/geoff/fg/fg15/install/OSG283/lib";
	//P->setEnvironment(env);
 	
 	P->start( QString(command_line));
	
	int res = P->waitForStarted();
		if (res) {
			buttonStart->setEnabled(false);
			buttonStop->setEnabled(true);
		}
	
	
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


