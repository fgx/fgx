#ifndef XOPT_H
#define XOPT_H

#include <QString>

class XOpt
{
public:
	explicit XOpt(QString option, bool enabled, QString value);
	QString option;
	QString value;
	bool enabled;


};

#endif // XOPT_H
