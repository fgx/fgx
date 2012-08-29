// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // xwebpage.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef XWEBPAGE_H
#define XWEBPAGE_H

#include <QWebPage>

class XWebPage : public QWebPage {

Q_OBJECT
public:
	
    explicit XWebPage(QObject *parent = 0);
	
	void javaScriptConsoleMessage ( const QString & message, int lineNumber, const QString & sourceID );

signals:

public slots:

};

#endif // XWEBPAGE_H
