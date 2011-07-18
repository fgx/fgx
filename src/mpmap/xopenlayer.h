#ifndef XOPENLAYERWIDGET_H
#define XOPENLAYERWIDGET_H

#include <QtCore/QVariant>
#include <QtCore/QString>

#include <QtWebKit/QWebView>
#include <QtNetwork/QNetworkDiskCache>
#include <QtNetwork/QNetworkCookieJar>


#include <QtGui/QStatusBar>
#include <QtGui/QProgressBar>
#include <QtGui/QLabel>
#include <QtGui/QWidget>
#include <QtGui/QActionGroup>
#include <QtGui/QToolButton>

#include "xobjects/mainobject.h"
#include "xobjects/latlng.h"

class MainObject;
QT_FORWARD_DECLARE_CLASS(QNetworkDiskCache)

class XOpenLayerWidget : public QWidget
{
Q_OBJECT
public:
	explicit XOpenLayerWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;
	QString *mpmapFollowCallsign;
	
private:
	
	QProgressBar *progressBar;
	QStatusBar *statusBar;
	QComboBox *comboServer;
	
	
	QWebView *webView;
	QNetworkDiskCache *networkDiskCache;
	QNetworkCookieJar *networkCookieJar;
	
signals:
	
	public slots:
    void start_progress();
    void update_progress(int progress);
    void end_progress(bool Ok);
	
	void init_xmap();
    void closeEvent(QCloseEvent *event);
	void loadFinished(bool);
	void setLatLon();
	void setZoom();
};

#endif // XOPENLAYERWIDGET