// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // xwebpage.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information


#include <QtDebug>

#include "xwebpage.h"

XWebPage::XWebPage(QObject *parent) :
    QWebPage(parent)
{
}


void XWebPage::javaScriptConsoleMessage ( const QString & message, int lineNumber, const QString & sourceID ){
	qDebug() << "ERR>" << message << " - " << lineNumber << " - " << sourceID;
}
