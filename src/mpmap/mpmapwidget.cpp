
//#include <QtDebug>

#include <QDesktopServices>

#include <QtCore/QString>
#include <QtCore/QVariant>


#include <QtGui/QCloseEvent>
#include <QtGui/QVBoxLayout>
#include <QtGui/QToolBar>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>

#include "mpmapwidget.h"
#include "coresettingswidget.h"
#include "launcherwindow.h"


MpMapWidget::MpMapWidget(MainObject *mOb, QWidget *parent) :
        QWidget(parent)
{

	mainObject = mOb;
	setProperty("settings_namespace", QVariant("mp_map_window"));
	mainObject->settings->restoreWindow(this);

    setWindowTitle(tr("Multi Player Map"));
	setWindowIcon(QIcon(":/icon/mpmap"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);


    //** Toolbar
    QToolBar *toolbar = new QToolBar();
    mainLayout->addWidget(toolbar, 1);

    //** Select server
	QLabel *lblSelectServer = new QLabel(tr("Select Server:"));
    toolbar->addWidget(lblSelectServer);

    comboServer = new QComboBox();
    toolbar->addWidget(comboServer);
	
	//**get callsign
	QString mpmapFollowCallsign;
	mpmapFollowCallsign.append(mainObject->settings->value("callsign").toString());
	
	//**add callsign to url
	QString mapURL1("http://mpmap01.flightgear.org/?follow=");
	QString mapURL2("http://mpmap02.flightgear.org/?follow=");
	mapURL1.append(mpmapFollowCallsign);
	mapURL2.append(mpmapFollowCallsign);
    comboServer->addItem("MpMap-01", QVariant(mapURL1));
    comboServer->addItem("MpMap-02", QVariant(mapURL2));
    connect(comboServer, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combo_server(int)));

	//=============================================================
	//== Cache
	//qDebug() << QDesktopServices::storageLocation(QDesktopServices::CacheLocation);

	networkDiskCache = new QNetworkDiskCache(this);
	networkDiskCache->setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation));

	networkCookieJar = new QNetworkCookieJar(this);

	//== Browser
	webView = new QWebView(this);
	mainLayout->addWidget(webView, 100);
	webView->page()->networkAccessManager()->setCache(networkDiskCache);
	webView->page()->networkAccessManager()->setCookieJar(networkCookieJar);
	connect(webView, SIGNAL(loadStarted()), this, SLOT(start_progress()));
	connect(webView, SIGNAL(loadProgress(int)), this, SLOT(update_progress(int)));
	connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(end_progress(bool)));

    //*** Status Bar
    statusBar = new QStatusBar(this);
    mainLayout->addWidget(statusBar);
    statusBar->showMessage("Idle");

    //** Progress Bar
    progressBar = new QProgressBar();
    progressBar->setVisible(false);
    statusBar->addPermanentWidget(progressBar);

    //*** Initialise
    on_combo_server(0);
}

//** Progress Slots
void MpMapWidget::start_progress(){
    progressBar->setVisible(true);
}

void MpMapWidget::update_progress(int v){
    progressBar->setValue(v);
}
void MpMapWidget::end_progress(bool Ok){
	Q_UNUSED(Ok);
    progressBar->setVisible(false);
	QString statusURL = comboServer->itemData(comboServer->currentIndex()).toString();
	statusURL.append(mpmapFollowCallsign);
    statusBar->showMessage( statusURL );
}


void MpMapWidget::on_combo_server(int index){

    QString server_url = comboServer->itemData(index).toString();
	webView->setUrl(  QUrl(server_url)  );
    statusBar->showMessage(QString("Loading: ").append(server_url));
    //qDebug("on_combo");

 }

//** Overide the closeEvent
void MpMapWidget::closeEvent(QCloseEvent *event)
 {
	mainObject->settings->saveWindow(this);
	Q_UNUSED(event);
 }

