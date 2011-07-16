#ifndef XSETTINGSMODEL_H
#define XSETTINGSMODEL_H

#include <QStandardItemModel>

#include "xobjects/xopt.h"
#include "xobjects/mainobject.h"
class MainObject;


class XSettingsModel : public QStandardItemModel
{
Q_OBJECT
public:

	enum COLS{
		C_OPTION = 0,
		C_ENABLED = 1,
		C_VALUE = 2,
		C_DEFAULT = 3,
		C_LEVEL = 4,
		C_DESCRIPTION = 5,
		C_AREA = 6
	};

	explicit XSettingsModel(MainObject *mob, QObject *parent = 0);

	MainObject *mainObject;
	bool _loading;

	void add_option(QString option, bool enabled, QString value, QString preset, int level, QString description, QString area);

	QString getx(QString option);
	XOpt getob(QString option);
	bool get_ena(QString option);

	QString get_fgfs_command_string();

	QStringList get_fgfs_options();

	QStringList get_fgfs_list();

	QStringList get_fgfs_args();



signals:
	void upx(QString option, bool enabled, QString value);
	void updated(QStringList lines);


public slots:

	void set_option(QString option, bool enabled, QString value);

	QString ini_file_path();
	void write_ini();
	void read_ini();
};

#endif // XSETTINGSMODEL_H
