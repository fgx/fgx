
#include <QtDebug>


#include "xmessagelabel.h"
#include "panes/coresettingswidget.h"

XMessageLabel::XMessageLabel(QWidget *parent) :
    QLabel(parent)
{

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(on_timer()));
	timer->setInterval(60);

}

void XMessageLabel::showMessage(QString message){
	showMessage(message, 3000);
}

void XMessageLabel::showMessage(QString message, int timeout){

	setStyleSheet("  font-family: freeuniversal; color: #333333; font-size: 16px; background-image: url(:/artwork/fgx-logo-flyer-yellow); background-repeat: none; padding-left: 40px; padding-top: 3px; padding-bottom: 5px; padding-right: 5px; margin-top: 10px; margin-bottom: 20px; ");
	setText(message);
	QTimer::singleShot(timeout, this, SLOT(start_fade()) );
}

void XMessageLabel::start_fade(){
	setText("[Callsign], Cessna c172p, San Francisco (KSFO), Runway 28R");
	setStyleSheet("font-family: freeuniversal; color: #666666; font-size: 16px; background-image: url(:/artwork/fgx-logo-flyer); background-repeat: none; padding-left: 40px; padding-top: 3px; padding-bottom: 5px; padding-right: 5px; margin-top: 10px; margin-bottom: 20px;");
}


