
#include <QProcess>
#include <QVBoxLayout>


#include "airports/metarwidget.h"

MetarWidget::MetarWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mob;

	setWindowIcon(QIcon(":/icon/metar"));

	setMinimumWidth(600);

	QVBoxLayout *mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	txtMetar = new QPlainTextEdit();
	txtMetar->setStyleSheet("font-size: 9pt;");
	mainLayout->addWidget(txtMetar);

}

//=================================================
//= load Metar
//===================================================
void MetarWidget::load_metar(QString apt)
{
	QProcess process;
	QStringList args;
	args << "-r" << "-v" << apt;

	txtMetar->setPlainText( QString("Loading..\n\nmetar %1").arg(apt) );
	process.start("metar", args, QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished(3000);
		QString ok_result = process.readAllStandardOutput();
		QString error_result = process.readAllStandardError();
		Q_UNUSED(error_result);
		//qDebug() <<  error_result;
		txtMetar->setPlainText(ok_result);
	}
}

void MetarWidget::clear_metar()
{
	txtMetar->setPlainText( "" );
}
