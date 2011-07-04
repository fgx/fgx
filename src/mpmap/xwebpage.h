#ifndef XWEBPAGE_H
#define XWEBPAGE_H

#include <QWebPage>

class XWebPage : public QWebPage

{
	virtual QString userAgentForUrl(const QUrl& url) const {
		return "Chrome/1.0";
	};


Q_OBJECT
public:
	

	
    explicit XWebPage(QObject *parent = 0);
	
	void javaScriptConsoleMessage ( const QString & message, int lineNumber, const QString & sourceID );
signals:

public slots:

};

#endif // XWEBPAGE_H
