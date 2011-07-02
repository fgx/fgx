#ifndef XBROWSER_H
#define XBROWSER_H

#include <QtGui/QWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QProgressBar>
#include <QtGui/QAction>

#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>
#include <QtNetwork/QNetworkDiskCache>
#include <QtNetwork/QNetworkCookieJar>

#include "xobjects/mainobject.h"

class MainObject;


class XBrowser : public QWidget
{
Q_OBJECT
public:
	explicit XBrowser(MainObject *mob, QWidget *parent = 0);


	MainObject *mainObject;

	QToolBar *toolbar;

	QProgressBar *progressBar;

	QWebView *webView;
	QNetworkDiskCache *networkDiskCache;
	QNetworkCookieJar *networkCookieJar;

	QStatusBar *statusBar;


signals:

public slots:

	void progress_start();
	void progress_update(int progress);
	void progress_finish(bool Ok);

};

#endif // XBROWSER_H
