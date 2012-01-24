
#include "app_config.h"
#include <QtDebug>

#include <QVBoxLayout>

#include "launcher/headerwidget.h"
#include "utilities/utilities.h"


HeaderWidget::HeaderWidget(MainObject *mob, QWidget *parent) :
	QWidget(parent)
{

	mainObject = mob;


	QGridLayout *mainLayout = new QGridLayout();
	mainLayout->setSpacing(2);
	setLayout(mainLayout);

	//= Fgx Logo
	QLabel *iconLabel = new QLabel("");
	iconLabel->setFixedHeight(40);
	iconLabel->setFixedWidth(40);
	iconLabel->setStyleSheet("QLabel { margin: 0px; background-image: url(:artwork/fgx-logo-flyer);  background-repeat: none;}");
	mainLayout->addWidget(iconLabel, 0, 0, 2, 1);

	//= Main header
	headerLabel = new QLabel("");
	headerLabel->setStyleSheet("QLabel { margin: 0px; color: #666666; font-size: 16px;}");
	mainLayout->addWidget(headerLabel, 0, 1, 1, 1);

	//= Callsing Full words
	callsignLabel = new QLabel("");
	callsignLabel->setStyleSheet("QLabel { margin: 0px; color: #888888; font-size: 11px;}");
	mainLayout->addWidget(callsignLabel, 1, 1, 1, 1);


	//= Message Label
	messageLabel = new QLabel();
	messageLabel->setText("FGx started.");
	mainLayout->addWidget(messageLabel, 0, 2,2,1);

}

//========================================================================
//= Show Message
void HeaderWidget::showMessage(QString message){
    showMessage(message, DEF_TO_MS);
}

void HeaderWidget::showMessage(QString message, int timeout){

	messageLabel->setStyleSheet("QLabel { color: #666666; font-size: 16px; padding-left: 5px; background-color: yellow; }");
	messageLabel->setText(message);

	QTimer::singleShot(timeout, this, SLOT(cancel_message()) );
}

//== Cancel message
void HeaderWidget::cancel_message()
{
	messageLabel->setStyleSheet("");
	messageLabel->setText("");
}


//========================================================================
//= Set Header
void HeaderWidget::setHeader(QString text)
{
	headerLabel->setText(text);

}

//= Set Callsign Sentence
void HeaderWidget::setCallsign(QString text)
{
	text = text.toUpper();
	QStringList words;
	for (int i = 0; i < text.size(); ++i) {
		words << abc_to_telephony(text.at(i).toLatin1());
	}
	callsignLabel->setText(words.join(" "));

}
