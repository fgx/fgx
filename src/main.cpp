/*
 *  main.cpp
 *  fgx
 *
 *  Created by Yves Sablonier, Zurich
 *  Copyright 2011 --- GPL2
 *
 */


//#include "fgx.h"

#include <QApplication>

#include "xobjects/mainobject.h"

int main( int argc, char* argv[])
{

	QApplication app(argc, argv);
	QApplication::setOrganizationName("fgx");
	QApplication::setOrganizationDomain("fgx.gral.com");
	QApplication::setApplicationName("FGX FlightGear Launcher");
	QApplication::setApplicationVersion("2.2-1beta");
	QApplication::setQuitOnLastWindowClosed(false);

	MainObject mainObject;


	return app.exec();
}


