
#include <QToolBar>


#include "xbrowser.h"

XBrowser::XBrowser(MainObject *mob, QWidget *parent) :
    QWidget(parent)
{

	mainObject = mob;

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0,0,0,0);
	mainLayout->setSpacing(0);


	QToolBar *toolbar = new QToolBar();
	mainLayout->addWidget(toolbar, 0);



	//== Browser storage
	//** TODO check this works? roblem with usign cache first ????
	networkDiskCache = new QNetworkDiskCache(this);
	networkDiskCache->setCacheDirectory(XSettings::cache_dir());

	networkCookieJar = new QNetworkCookieJar(this);

	//== WebView
	webView = new QWebView();
	mainLayout->addWidget(webView);

	webView->page()->mainFrame()->addToJavaScriptWindowObject("QtWidget", this);
	connect(webView, SIGNAL(loadStarted()),
			this, SLOT(progress_start())
			);
	connect(webView, SIGNAL(loadProgress(int)),
			this, SLOT(progress_update(int))
			);
	connect(webView, SIGNAL(loadFinished(bool)),
			this, SLOT(progress_finish(bool)));

	//*********************************
	//** Status Bar
	statusBar = new QStatusBar();
	mainLayout->addWidget(statusBar);

	statusBar->addPermanentWidget(new QLabel("Zoom:"));
	//lblZoom = new QLabel("-");
   //statusBar->addPermanentWidget(lblZoom);


}



//==========================================
//== Progress Slots
//==========================================
void XBrowser::progress_start(){
	progressBar->setVisible(true);
}

void XBrowser::progress_update(int v){
	progressBar->setValue(v);
}
void XBrowser::progress_finish(bool Ok){
	Q_UNUSED(Ok);
	progressBar->setVisible(false);
	//statusBar->showMessage( comboServer->itemData(comboServer->currentIndex()).toString() );
}

