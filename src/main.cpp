/*
 *  main.cpp
 *  fgx
 *
 *  Created by Yves Sablonier, Zurich
 *  © 2011 --- GPL2
 *
 */

#include <QDebug>
#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <QFile>
#include <QTextStream>
#include <iostream>


#include "xobjects/mainobject.h"

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

	
	
	return app.exec();
}


