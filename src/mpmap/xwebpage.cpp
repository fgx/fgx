
#include <QtDebug>

#include "xwebpage.h"

XWebPage::XWebPage(QObject *parent) :
    QWebPage(parent)
{
}


void XWebPage::javaScriptConsoleMessage ( const QString & message, int lineNumber, const QString & sourceID ){
	qDebug() << "ERR>" << message << " - " << lineNumber << " - " << sourceID;
}
