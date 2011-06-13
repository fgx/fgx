/*
 *  main.cpp
 *  fgx
 *
 *  Created by Yves Sablonier, Zurich
 *  © 2011 --- GPL2
 *
 */

#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <QFile>
#include <QTextStream>


#include "xobjects/mainobject.h"
#include "utilities/utilities.h"

//* message handler
void fgxErrorMessage(QtMsgType type, const char *msg)
{
    QString m = msg;
    switch (type)
    {
    case QtDebugMsg:
        outLog("*** FGX says: "+m+" ***\n",0);
        break;
    case QtWarningMsg:
        outLog("*** FGX warning: "+m+" ***\n",0);
        break;
    case QtCriticalMsg:
        outLog("*** FGX critical: "+m+" ***\n",0);
        break;
    case QtFatalMsg:
        outLog("*** FGX fatal: "+m+" ***\n",0);
        abort();
        break;
    }
}

int main( int argc, char* argv[])
{
	
	//* versioning
	QApplication app(argc, argv);
	QApplication::setOrganizationName("FGx");
	QApplication::setOrganizationDomain("fgx.googlecode.com");
	QApplication::setApplicationName("FGx");
	QApplication::setApplicationVersion("2.3-0alpha");

	MainObject mainObject;
	
	//* global styles, should go to a qss once
	QString styles;
	styles.append("QLineEdit { background-color: white }");
	styles.append("QGroupBox { font-size: 11px; text-transform: uppercase }");
	styles.append("QPushButton { padding: 5px; }");
	styles.append("QStatusBar { font-family: monospace; font-size: 9px; font-weight: bold; color: #666666; background-image: url(:images/background_yellow); }");
	styles.append("LauncherWindow { background: #eeeeee }");
	
	//* apply styles
	qApp->setStyleSheet(styles);

	//* install message handler
	qInstallMsgHandler(fgxErrorMessage);
	
	return app.exec();
}





