
#include <QProcess>
#include <QVBoxLayout>


#include "airports/metarwidget.h"
#include "utilities/utilities.h"

MetarWidget::MetarWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mob;

	setWindowIcon(QIcon(":/icon/metar"));
    setWindowTitle("Metar Fetch");

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
    QString results;
    QString metarPath;
    QString tmp;
    QString error_result;
    QDir d;
    metarPath = "metar";
	args << "-r" << "-v" << apt;
	
	// for environment output, deactivated
	/*QStringList environment = QProcess::systemEnvironment();
	for (int i=0; i < environment.count(); i++) {
		outLog("Env: "+environment[i]);
	}*/
	
#ifdef Q_OS_MAC
	metarPath = mainObject->X->fgfs_path();
	metarPath.chop(4);
	metarPath.append("metar");
#else
    // it is hoped this would suit ALL ports
    tmp = mainObject->X->fgfs_path();
    tmp = util_getBasePath(tmp);
    tmp.append("metar");
#ifdef Q_OS_WIN
    tmp.append(".exe"); // add windows thing
#endif
    if (QFile::exists(tmp))
        metarPath = tmp;
#endif
	txtMetar->setPlainText( QString("Loading..\n\nmetar %1").arg(apt) );
    outLog("Running: ["+metarPath+" "+args.join(" ")+"]");
    process.start(metarPath, args, QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished(3000);
        results = process.readAllStandardOutput();
        error_result = process.readAllStandardError();
        tmp = results.simplified();
        if (tmp.size() == 0) {
            if (error_result.size() > 0) {
                results = error_result;
            } else {
                results = "NO metar results found!";
            }
        }
        txtMetar->setPlainText(results);
    } else {
        results = "ERROR: Running of metar FAILED";
    }
    outLog("Results: "+results);
}

void MetarWidget::clear_metar()
{
	txtMetar->setPlainText( "" );
}
