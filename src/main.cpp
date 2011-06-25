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
        outLog("*** FGx says: "+m+" ***\n",0);
        break;
    case QtWarningMsg:
        outLog("*** FGx warning: "+m+" ***\n",0);
        break;
    case QtCriticalMsg:
        outLog("*** FGx critical: "+m+" ***\n",0);
        break;
    case QtFatalMsg:
        outLog("*** FGx fatal: "+m+" ***\n",0);
        abort();
        break;
    }
}

int main( int argc, char* argv[])
{
	
	//* versioning
	QApplication app(argc, argv);
	QApplication::setOrganizationName("FGx");
	QApplication::setOrganizationDomain("googlecode.com");
	QApplication::setApplicationName("FGx");
	QApplication::setApplicationVersion("2.3.0rc3");

	MainObject mainObject;
	
	//* global styles, should go to a qss once
	QString styles;
	styles.append("QLineEdit { background-color: white }");
	styles.append("QGroupBox { font-size: 11px; text-transform: uppercase }");
	styles.append("QPushButton { padding: 5px; }");
	styles.append("QStatusBar { font-family: Andale mono, monospace; font-size: 12px; font-weight: normal; color: #FF0000; background-image: url(:images/background_yellow); }");
	styles.append("LauncherWindow { background: #eeeeee }");
	
	//* apply styles
	qApp->setStyleSheet(styles);

	//* install message handler - with Pete's TEMP hack to be removed later
	if( !QFile::exists("/home/ffs/fgx/LOCAL") ){
		qInstallMsgHandler(fgxErrorMessage);
	}

	return app.exec();
}





