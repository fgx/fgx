#include <QDebug>


#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>


#include "panes/outputpreviewwidget.h"

OutputPreviewWidget::OutputPreviewWidget(QWidget *parent) :
    QWidget(parent)
{


	QHBoxLayout *mainLayout = new QHBoxLayout();
	setLayout(mainLayout);


	QGroupBox *groupBox = new QGroupBox(tr("Debug Output"));
	mainLayout->addWidget(groupBox);

	QHBoxLayout *layoutBox = new QHBoxLayout();
	groupBox->setLayout(layoutBox);


	txtPreviewOutput = new QPlainTextEdit();
	layoutBox->addWidget(txtPreviewOutput);


	QVBoxLayout *layoutButtons = new QVBoxLayout();
	layoutBox->addLayout(layoutButtons);

	buttonCommandPreview = new QPushButton();
	buttonCommandPreview->setText(tr("Preview Command"));
	layoutButtons->addWidget(buttonCommandPreview);

	buttonCommandHelp = new QPushButton();
	buttonCommandHelp->setText(tr("Options Help"));
	layoutButtons->addWidget(buttonCommandHelp);

	layoutButtons->addStretch(20);
}
