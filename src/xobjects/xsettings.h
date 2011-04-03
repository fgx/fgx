#ifndef XSETTINGS_H
#define XSETTINGS_H

#include <QSettings>

class XSettings : public QSettings
{
Q_OBJECT
public:
	enum OS{
		BSD = 1,
		MAC = 2,
		LINUX = 3,
		WINDOWS = 4,
		UNKNOWN = 5,
	};
    explicit XSettings(QObject *parent = 0);

	bool _dev_mode(); //* pete's hack - ../DEV.txt exists for local dev

	QString db_file();

	QString temp_dir();
	QString temp_dir(QString append_path);

	QString fgfs_path();
	QString default_fgfs_path();

    QString fg_root();
    QString fg_root(QString append_path);
	QString default_fg_root();

	QString aircraft_path();
	QString aircraft_path(QString dir);

	QString airports_path();

	QString scenery_path();
	bool use_terrasync();
	QString terrasync_path();

	void saveWindow(QWidget *widget);
	void restoreWindow(QWidget *widget);
	QString _windowName(QWidget *widget);

	QString default_fgcom_no();
	QString default_fgcom_port();

	int runningOS();
	bool paths_sane();

signals:

public slots:

};

#endif // XSETTINGS_H
