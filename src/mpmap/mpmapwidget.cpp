
#include "mpmapwidget.h"

#include <QtCore/QString>
#include <QtCore/QVariant>


#include <QtGui/QCloseEvent>
#include <QtGui/QVBoxLayout>
#include <QtGui/QToolBar>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>

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
    comboServer->addItem("MpMap-01", QVariant("http://mpmap01.flightgear.org"));
    comboServer->addItem("MpMap-02", QVariant("http://mpmap02.flightgear.org"));
    connect(comboServer, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combo_server(int)));



    //** Browser
    browser = new QWebView(this);
    mainLayout->addWidget(browser, 100);
    connect(browser, SIGNAL(loadStarted()), this, SLOT(start_progress()));
    connect(browser, SIGNAL(loadProgress(int)), this, SLOT(update_progress(int)));
    connect(browser, SIGNAL(loadFinished(bool)), this, SLOT(end_progress(bool)));

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
    statusBar->showMessage( comboServer->itemData(comboServer->currentIndex()).toString() );
}


void MpMapWidget::on_combo_server(int index){

    QString server_url = comboServer->itemData(index).toString();
    browser->setUrl(  QUrl(server_url)  );
    statusBar->showMessage(QString("Loading: ").append(server_url));
    //qDebug("on_combo");

 }

//** Overide the closeEvent
void MpMapWidget::closeEvent(QCloseEvent *event)
 {
	mainObject->settings->saveWindow(this);
	Q_UNUSED(event);
 }

