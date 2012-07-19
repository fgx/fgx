// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // xopt.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef XOPT_H
#define XOPT_H

#include <QString>

class XOpt
{
public:
	explicit XOpt(QString option, bool enabled, QString value, QString default_value);
	QString option;
	QString value;
	bool enabled;
	QString default_value;

};

#endif // XOPT_H
