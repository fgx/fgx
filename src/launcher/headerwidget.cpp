
#include <QtDebug>

#include <QVBoxLayout>


#include "launcher/headerwidget.h"

#include "data/radio.h"


HeaderWidget::HeaderWidget(MainObject *mob, QWidget *parent) :
	QWidget(parent)
{

	mainObject = mob;

	//= Init Label
	QGridLayout *mainLayout = new QGridLayout();
	mainLayout->setSpacing(2);
	setLayout(mainLayout);

	QLabel *iconLabel = new QLabel("");
	iconLabel->setFixedHeight(40);
	iconLabel->setFixedWidth(40);
	iconLabel->setStyleSheet("QLabel { margin: 0px; background-image: url(:artwork/fgx-logo-flyer);  background-repeat: none;}");
	mainLayout->addWidget(iconLabel, 0, 0, 2, 1);

	headerLabel = new QLabel("");
	headerLabel->setStyleSheet("QLabel { margin: 0px; color: #666666; font-size: 15px;}");
	mainLayout->addWidget(headerLabel, 0, 1, 1, 1);

	callsignLabel = new QLabel("");
	callsignLabel->setStyleSheet("QLabel { margin: 0px; color: #888888; font-size: 10px;}");
	mainLayout->addWidget(callsignLabel, 1, 1, 1, 1);


	//===========================================
	//= Popup Message

	/*
	//= Popup Widget/Window
	popWidget = new QWidget(this);
	popWidget->hide();
	popWidget->setFixedHeight(50);
	popWidget->setFixedWidth(500);
	popWidget->show();

	//= Popup Layout
	QVBoxLayout *popLay = new QVBoxLayout();
	popWidget->setLayout(popLay);

	//= Popup Label with content and style
	popLabel = new QLabel();
	popLabel->setText("FGx started.");
	popLay->addWidget(popLabel);
	popWidget->show();

	popWidget->setStyleSheet("QLabel { background-image: url(:artwork/fgx-logo-flyer); background-repeat: none; color: #666666; font-size: 15px; padding-left: 40px; padding-top: 5px; background-color: yellow; }");

	QTimer::singleShot(3000, popWidget, SLOT(hide()) );
	*/

}

void HeaderWidget::showMessage(QString message){
	showMessage(message, 3000);
}

void HeaderWidget::showMessage(QString message, int timeout){
	return;

	popLabel->setText(message);
	popWidget->show();
	return;

	QTimer::singleShot(timeout, popWidget, SLOT(hide()) );
}


void HeaderWidget::setHeader(QString text)
{
	headerLabel->setText(text);

}
void HeaderWidget::setCallsign(QString text)
{
	text = text.toUpper();
	QStringList words;
	for (int i = 0; i < text.size(); ++i) {
		words << abc_to_telephony(text.at(i).toLatin1());
	}
	callsignLabel->setText(words.join(" "));

}
