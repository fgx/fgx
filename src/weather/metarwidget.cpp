// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // metarwidget.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information


#include <QProcess>
#include <QVBoxLayout>

#include "weather/metarwidget.h"
#include "utilities/utilities.h"

MetarWidget::MetarWidget(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

    mainObject = mob;
    this->netMan = new NetManager();
    connect(this->netMan, SIGNAL(reply(XReply)),
            this, SLOT(on_reply(XReply)));

    setWindowIcon(QIcon(":/icon/metar"));
    setWindowTitle("Metar Fetch");

    setProperty("settings_namespace", "metar_widget");

    setMinimumWidth(800);
    setMinimumHeight(600);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    txtMetar = new QPlainTextEdit();
    txtMetar->setStyleSheet("font-size: 9pt;");
    txtMetar->setMaximumHeight(50);
    mainLayout->addWidget(txtMetar);

    txtJson = new QPlainTextEdit();
    txtJson->setStyleSheet("font-size: 9pt;");
    mainLayout->addWidget(txtJson);

    statusBar = new XStatusBar();
    mainLayout->addWidget(statusBar);
    connect(statusBar, SIGNAL(refresh()),
            this, SLOT(on_refresh()));

    this->mainObject->settings->restoreWindow(this);

}

void MetarWidget::show_metar(QString icao_code){
    this->apt_code = icao_code;
    this->setWindowTitle( QString("Metar: %1").arg(this->apt_code));
    this->show();
    this->fetch_metar();
}

void MetarWidget::fetch_metar(){
    QString u = QString("http://avwx.rest/api/metar/%1").arg(this->apt_code);
    this->statusBar->set_busy(true, QString("Request: %1").arg(u));
    this->netMan->get(this, u);

}

void MetarWidget::on_refresh(){
    this->txtMetar->clear();
    this->fetch_metar();
}

void MetarWidget::on_reply(XReply xreply){
    if(xreply.error){
        statusBar->set_busy(false, "Error");
        return;
    }
    statusBar->set_busy(false, "Success");
    //qDebug() << "YES =" << xreply.raw;
    //qDebug() << "YES =" << xreply.data.property("Raw-Report").toString();
    txtMetar->setPlainText( xreply.data.property("Raw-Report").toString() );
    txtJson->setPlainText( xreply.raw );
}

//=================================================
//= load Metar
//===================================================
void MetarWidget::load_metar(QString apt)
{
    this->setWindowTitle( QString("Metar: %1").arg(apt));
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




//= window close
void MetarWidget::closeEvent(QCloseEvent *ev){
    Q_UNUSED(ev);
    this->mainObject->settings->saveWindow(this);

}
