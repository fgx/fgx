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
	QApplication a(argc, argv);
	fgx w;
	w.show();
	return a.exec();
}


