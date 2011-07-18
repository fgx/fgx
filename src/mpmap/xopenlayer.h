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


class XOpenLayerWidget : public QWidget
{
Q_OBJECT
public:
	explicit XOpenLayerWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;

    QWebView *webView;


signals:

public slots:
	
	void init_map();
	
};

#endif // XOPENLAYERWIDGET