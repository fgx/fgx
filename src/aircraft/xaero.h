#ifndef XAERO_H
#define XAERO_H

#include <QString>

class XAero
{
public:
	XAero(QString callsign);

	QString callsign;
	QString lat;
	QString lon;
	QString altitude;
	QString heading;

};

#endif // XAERO_H
