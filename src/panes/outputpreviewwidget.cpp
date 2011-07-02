

//#include <QDebug>


#include <QtCore/QProcess>

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>


#include "panes/outputpreviewwidget.h"

OutputPreviewWidget::OutputPreviewWidget(MainObject *mOb, QWidget *parent) :
    QWidget(parent)
{
	mainObject = mOb;

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
	connect(buttonCommandHelp, SIGNAL(clicked()), this, SLOT(on_command_help()));

	layoutButtons->addStretch(20);
}



void OutputPreviewWidget::on_command_help(){
	QProcess process;
	QStringList args;
	args << "-h" << "-v" << QString("--fg-root=").append(mainObject->settings->fgroot());
	qDebug() << mainObject->settings->fgfs_path() <<  args;
	process.start(mainObject->settings->fgfs_path(), args, QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished(10000);
		QString ok_result = process.readAllStandardOutput();
		txtPreviewOutput->setPlainText(ok_result);
	}
}

