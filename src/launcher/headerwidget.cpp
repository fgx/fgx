
#include <QtDebug>

#include <QVBoxLayout>


#include "launcher/headerwidget.h"

HeaderWidget::HeaderWidget(MainObject *mob, QWidget *parent) :
	QWidget(parent)
{

	mainObject = mob;

	//= Init Label
	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);
	
	headerLabel = new QLabel("");
	headerLabel->setFixedHeight(40);
	headerLabel->setFixedWidth(500);
	headerLabel->setStyleSheet("QLabel { margin: 0px; border: 0px; background-image: url(:artwork/fgx-logo-flyer); color: #666666; background-repeat: none; font-size: 15px; padding-left: 40px; }");
	mainLayout->addWidget(headerLabel);

	//===========================================
	//= Popup Message

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


void HeaderWidget::setText(QString text)
{
	headerLabel->setText(text);

}
