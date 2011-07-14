
#include <QtDebug>


#include "xmessagelabel.h"
#include "panes/coresettingswidget.h"

XMessageLabel::XMessageLabel(QWidget *parent) :
    QLabel(parent)
{

	//= Fade Timer
	fadeTimer = new QTimer(this);
	connect(fadeTimer, SIGNAL(timeout()), this, SLOT(on_fade_timer()) );
	fadeTimer->setInterval(60);

	//= Init Label
	setText("Hello Message Label");

	//setStyleSheet("font-size: 20pt; background-color: #ffffff;");
	setStyleSheet("  font-family: freeuniversal; color: #333333; font-size: 16px; background-image: url(:/artwork/fgx-logo-flyer-yellow); background-repeat: none; padding-left: 40px; padding-top: 3px; padding-bottom: 5px; padding-right: 5px; margin-top: 10px; margin-bottom: 20px; ");

	//===========================================
	//= Popup Message

	//= Popup Widget/Window
	popWidget = new QWidget();
	popWidget->hide();

	popWidget->setWindowFlags(Qt::FramelessWindowHint);
	popWidget->setFixedWidth(400);
	popWidget->setFixedHeight(40);

	//= Popup Layout
	QVBoxLayout *popLay = new QVBoxLayout();
	popWidget->setLayout(popLay);

	//= Popup Label with content and style
	popLabel = new QLabel("Yesssss");
	popLay->addWidget(popLabel);
	QString style_str("font-weight: italic; color: blue;  padding: 10px; border: 1px outset yellow;");
	style_str.append("background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #F8FAC3, stop: 1 #F0DD17);");
	popLabel->setStyleSheet(style_str);




}

void XMessageLabel::showMessage(QString message){
	showMessage(message, 3000);
}

void XMessageLabel::showMessage(QString message, int timeout){
	//return;
	//#if pos:
	//	offPos = QtCore.QPoint( pos.x() - self.width(), pos.y() )
		//self.move(offPos)

	popLabel->setText(message);
	popWidget->setWindowOpacity(1.0);
	popWidget->show();


	QTimer::singleShot(timeout, this, SLOT(start_fade()) );
}





void XMessageLabel::start_fade(){
	setStyleSheet("font-family: freeuniversal; color: #666666; font-size: 16px; background-image: url(:/artwork/fgx-logo-flyer); background-repeat: none; padding-left: 40px; padding-top: 3px; padding-bottom: 5px; padding-right: 5px; margin-top: 10px; margin-bottom: 20px;");
}


void XMessageLabel::on_fade_timer()
{
	if(popWidget->windowOpacity() == 0.0){
		fadeTimer->stop();
		popWidget->hide();
	}else{
		popWidget->setWindowOpacity( popWidget->windowOpacity() - 0.1 );
	}
}
