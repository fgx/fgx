#ifndef XSETTINGSMODEL_H
#define XSETTINGSMODEL_H

#include <QString>
#include <QStringList>

#include <QStandardItemModel>
#include <QModelIndex>

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

	QModelIndex get_index(QString option);

	QString getx(QString option);
	QString getx(QString option, bool return_default);
	QString getx(QString option, QString default_string);

	XOpt get_opt(QString option);
	bool get_ena(QString option);


	QStringList get_fgfs_args();
	QStringList get_fgfs_list();
	QString get_fgfs_command_string();
	QStringList get_fgfs_env();

	bool fgfs_use_default();
	QString fgfs_default_path();
	QString fgfs_path();
	QString fgfs_custom_path();


	QString fgroot();
	QString fgroot(QString append_path);
	bool fgroot_use_default();
	QString fgroot_default_path();
	QString fgroot_custom_path();

	QString aircraft_path();
	QString aircraft_path(QString dir);

	QString airports_path();

	QString scenery_path();

	bool terrasync_enabled();
	QString terrasync_exe_path();
	QString terrasync_sync_data_path();

	QString fgcom_exe_path();


	bool paths_sane();

	void set_row_bg(int row_idx, QColor bgcolor);


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
