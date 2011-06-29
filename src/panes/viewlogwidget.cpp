

#include <QHBoxLayout>

#include <QGroupBox>


#include "viewlogwidget.h"

ViewLogWidget::ViewLogWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mOb;



	QHBoxLayout *mainLayout = new QHBoxLayout();
	setLayout(mainLayout);


	QGroupBox *groupBox = new QGroupBox(tr("Log Output"));
	mainLayout->addWidget(groupBox);

	QHBoxLayout *layoutBox = new QHBoxLayout();
	groupBox->setLayout(layoutBox);


	txtLog = new QPlainTextEdit();
	layoutBox->addWidget(txtLog);


	QVBoxLayout *layoutButtons = new QVBoxLayout();
	layoutBox->addLayout(layoutButtons);

	buttonRefresh = new QPushButton();
	buttonRefresh->setText(tr("Refresh"));
	buttonRefresh->setIcon(QIcon(":/icon/refresh"));

	layoutButtons->addWidget(buttonRefresh);


	layoutButtons->addStretch(20);

}
