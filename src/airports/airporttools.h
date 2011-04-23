#ifndef AIRPORTTOOLS_H
#define AIRPORTTOOLS_H

#include <QtCore/QObject>
#include <QtCore/QFileInfo>

#include "xobjects/mainobject.h"

class AirportTools : public QObject
{
Q_OBJECT
public:
	explicit AirportTools(QObject *parent, MainObject *mOb);

	MainObject *mainObject;

	void scan_airports_xml();

	void parse_runways(QFileInfo thresholdXmlFileInfo);

signals:

public slots:

};

#endif // AIRPORTTOOLS_H
