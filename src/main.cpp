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

	return app.exec();
}


