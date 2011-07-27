
#include <QProcess>
#include <QVBoxLayout>


#include "airports/metarwidget.h"
#include "utilities/utilities.h"

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
    QString results;
    QString metarPath;
    QString tmp;
    QString error_result;
    QDir d;
    metarPath = "metar";
	args << "-r" << "-v" << apt;
    if ( ! mainObject->X->fgroot_use_default() ) {
        int ind, siz;
        tmp = mainObject->X->fgfs_path();
        ind = tmp.indexOf(QChar('/'));
        siz = 0;
        // march to last '/' in path
        while (ind >= 0) {
            tmp = tmp.mid(ind + 1);
            siz = tmp.size();
            ind = tmp.indexOf(QChar('/'));
        }
        if (siz > 0) {
            tmp = mainObject->X->fgfs_path();
            tmp.chop(siz);
            if (d.exists(tmp)) {
                metarPath = tmp + metarPath;
            }
        }
    }

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
