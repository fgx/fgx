// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // metarwidget.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef METARWIDGET_H
#define METARWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>



#include "xobjects/mainobject.h"
class MainObject;


class MetarWidget : public QWidget
{
Q_OBJECT
public:
	explicit MetarWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;

	QPlainTextEdit *txtMetar;


signals:

public slots:
	void load_metar(QString apt);
	void clear_metar();

};

#endif // METARWIDGET_H
