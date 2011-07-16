

//#include <QDebug>

#include <QtCore/QProcess>

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>

#include "xwidgets/xgroupboxes.h"
#include "panes/outputpreviewwidget.h"

OutputPreviewWidget::OutputPreviewWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{
	mainObject = mOb;

	QHBoxLayout *mainLayout = new QHBoxLayout();
	setLayout(mainLayout);


	XGroupHBox *groupBox = new XGroupHBox(tr("Debug Output"));
	mainLayout->addWidget(groupBox);


	txtPreviewOutput = new QPlainTextEdit();
	groupBox->addWidget(txtPreviewOutput);
	txtPreviewOutput->setReadOnly(true);


	QVBoxLayout *layoutButtons = new QVBoxLayout();
	groupBox->addLayout(layoutButtons);


	//=============================================================
	buttonGroup = new QButtonGroup(this);
	buttonGroup->setExclusive(true);
	connect(buttonGroup, SIGNAL(buttonClicked(int)) , this, SLOT(preview()));

	QRadioButton *radioString = new QRadioButton();
	radioString->setText("Single line");
	radioString->setProperty("value", "string");
	layoutButtons->addWidget(radioString);
	buttonGroup->addButton(radioString, 0);

	QRadioButton *radioLines = new QRadioButton();
	radioLines->setText("Separate lines");
	radioLines->setProperty("value", "lines");
	layoutButtons->addWidget(radioLines);
	buttonGroup->addButton(radioLines, 1);

	QRadioButton *radioShell = new QRadioButton();
	radioShell->setText("Shell lines");
	radioShell->setProperty("value", "shell");
	layoutButtons->addWidget(radioShell);
	buttonGroup->addButton(radioShell, 2);


	//=============================================================
	buttonCommandPreview = new QPushButton();
	buttonCommandPreview->setText(tr("Show Command"));
	layoutButtons->addWidget(buttonCommandPreview);
	connect(buttonCommandPreview, SIGNAL(clicked()), this, SLOT(preview()));

	buttonCommandHelp = new QPushButton();
	buttonCommandHelp->setText(tr("Options Help"));
	layoutButtons->addWidget(buttonCommandHelp);
	connect(buttonCommandHelp, SIGNAL(clicked()), this, SLOT(on_command_help()));

	layoutButtons->addStretch(20);

	buttonGroup->button(mainObject->settings->value("preview_type", "1").toInt())->setChecked(true);
}


