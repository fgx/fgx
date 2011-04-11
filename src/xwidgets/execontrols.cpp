
#include <QDebug>

#include <QtGui/QHBoxLayout>

#include "execontrols.h"

/* The idea of this is to have a widget that both launches a "service"..
   But also can check and kill it, on linux or mac.. Windows got no idea
   WANTED: expert advice
*/

ExeControls::ExeControls(QWidget *parent) :
    QGroupBox(parent)
{

	QGridLayout *layout = new QGridLayout();
	setLayout(layout);
	int row = 0;

	//** First Row is the label with the "status".. colors set in stylsheet
	labelPid = new QLabel("---");
	labelPid->setStyleSheet("font-size: 8pt; font-family: monospaced;");
	layout->addWidget(labelPid, row, 1, 1, 2);

	//* Second row is the Push buttons
	buttonStop = new QPushButton();
	buttonStop->setText(tr("Stop"));
	buttonStop->setIcon(QIcon(":/icon/stop_disabled"));
	layout->addWidget(buttonStop, row, 0, 1, 1);
	connect(buttonStop, SIGNAL(clicked()), this, SLOT(on_stop_clicked()));

	buttonStart = new QPushButton();
	buttonStart->setText(tr("Start"));
	buttonStart->setIcon(QIcon(":/icon/start_enabled"));
	layout->addWidget(buttonStart, row, 2, 1, 1);
	connect(buttonStart, SIGNAL(clicked()), this, SLOT(on_start_clicked()));

}


void ExeControls::on_stop_clicked(){
	qDebug() <<  "start";
}

void ExeControls::on_start_clicked(){
	qDebug() <<  "stop kill";
}

void ExeControls::update_pid(){
	qDebug() << "Is the PID exe runnign or not";
}

void ExeControls::kill_pid(){
	//QProcess pid of exe
}




