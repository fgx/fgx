/*
 *  main.cpp
 *  fgx
 *
 *  Created by Yves Sablonier, Zurich
 *  Copyright 2011 --- GPL2
 *
 */


#include "fgx.h"
#include <QApplication>

int main( int argc, char* argv[])
{

	QApplication app(argc, argv);
	QApplication::setOrganizationName("fgx");
	QApplication::setOrganizationDomain("fgx.gral.com");
	QApplication::setApplicationName("FlightGear Starter OSX");
	QApplication::setApplicationVersion("0.1");


	fgx w;
	w.show();

	return app.exec();
}


