#ifndef AIRPORTTOOLS_H
#define AIRPORTTOOLS_H

#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "xobjects/mainobject.h"

class AirportTools : public QObject
{
Q_OBJECT
public:
	explicit AirportTools(QObject *parent, MainObject *mOb);

	MainObject *mainObject;

	void scan_airports_xml();
	void parse_runways_xml(QDir dir, QString airport_code);
	void parse_ils_xml(QDir dir, QString airport_code);
	void parse_parking_xml(QDir dir, QString airport_code);

	QStringList listParkingPositions;
signals:

public slots:

};

#endif // AIRPORTTOOLS_H
