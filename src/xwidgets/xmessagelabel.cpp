
#include <QtDebug>


#include "xmessagelabel.h"

XMessageLabel::XMessageLabel(QWidget *parent) :
    QLabel(parent)
{

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(on_timer()));
	timer->setInterval(60);

	//setWindowOpacity(0.0);
	setStyleSheet("  color: #000099; font-size: 10pt;");
}

void XMessageLabel::showMessage(QString message){
	showMessage(message, 3000);
}

void XMessageLabel::showMessage(QString message, int timeout){

	setText(message);
	setWindowOpacity(1.0);
	QTimer::singleShot(timeout, this, SLOT(start_fade()) );
}

void XMessageLabel::start_fade(){
	//qDebug() << "start_dade";
	//timer->start();
	setText("");
}

void XMessageLabel::on_timer(){
	//setText("")
	/*
	qDebug() << "on_timer" << windowOpacity();
	if(windowOpacity() == 0.0){
		timer->stop();
	}else{
		setWindowOpacity( windowOpacity() - 0.1 );
	}
	*/
}
