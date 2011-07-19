#include "xopt.h"

XOpt::XOpt(QString option, bool enabled, QString value, QString default_value)
{
	this->option = option;
	this->enabled = enabled;
	this->value = value;
	this->default_value = default_value;
}
