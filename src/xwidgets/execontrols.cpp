
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QTimer>

#include <QtGui/QHBoxLayout>

#include "execontrols.h"
#include "utilities/utilities.h"

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
	buttonStop->setMinimumSize(QSize( 100, 20));
	connect(buttonStop, SIGNAL(clicked()), this, SLOT(on_stop_clicked()));

	//** Start Button
	buttonStart = new QPushButton();
	buttonStart->setText(tr(" Start "));
	buttonStart->setMinimumSize(QSize( 100, 20)); 
	buttlay->addWidget(buttonStart);
	//* connection is done in fgx not here..

	//** Bottom layout
	QHBoxLayout *bottlay = new QHBoxLayout();
	layout->addLayout(bottlay);
	

}


// Read standard output
void ExeControls::readOutput()

{
	QString fgxoutput = P->readAllStandardOutput();
	
	// show some lines in statusbar as feedback
	//statusBar->showMessage(fgxoutput, 6000));

	// write output to fgxlog
	outLog(ExeControls::title()+": "+fgxoutput);
}


// Read standard error
void ExeControls::readError()

{
	QString fgxerror = P->readAllStandardError();
	
	// show some lines in statusbar as feedback
	//statusBar->showMessage(fgxerror, 6000);
	
	// write errors to fgxlog
	outLog(ExeControls::title()+": "+fgxerror);
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

	outLog("Starting: "+command_line);
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


