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

void fgxErrorMessage(QtMsgType type, const char *msg)
{
	switch (type) {
		case QtDebugMsg:
			fprintf(stderr, "FGX Debug: %s\n", msg);
			break;
		case QtWarningMsg:
			fprintf(stderr, "FGX Warning: %s\n", msg);
			break;
		case QtCriticalMsg:
			fprintf(stderr, "FGX Critical: %s\n", msg);
			break;
		case QtFatalMsg:
			fprintf(stderr, "FGX Fatal: %s\n", msg);
			abort();
	}
}

int main( int argc, char* argv[])
{
	
	QApplication app(argc, argv);
	QApplication::setOrganizationName("FGX");
	QApplication::setOrganizationDomain("fgx.googlecode.com");
	QApplication::setApplicationName("FGX FlightGear Launcher");
	QApplication::setApplicationVersion("2.3-0pre");

	MainObject mainObject;
	
	// Global Styles
	QString styles;
	styles.append("QLineEdit { background-color: white }");
	styles.append("QGroupBox { font-size: 11px; text-transform: uppercase }");
	styles.append("QPushButton { padding: 5px; }");
	styles.append("QStatusBar { font-size: 9px; color: #666666 }");
	
	qApp->setStyleSheet(styles);

	qInstallMsgHandler(fgxErrorMessage);
	
	return app.exec();
}





