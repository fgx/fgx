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

	static bool dev_mode();

	QString data_file(QString file_name);

	static QString temp_dir();
	static QString temp_dir(QString append_path);

	static QString cache_dir();

	QString fgx_path();
	static QString fgx_current_dir();

	QString fgfs_path();
	bool fgfs_use_default();
	QString fgfs_default_path();

	QString fgroot();
	QString fgroot(QString append_path);
	bool fgroot_use_default();
	QString fgroot_default_path();

	QString aircraft_path();
	QString aircraft_path(QString dir);

	QString airports_path();

	QString scenery_path();

	bool terrasync_enabled();
	QString terrasync_exe_path();
	QString terrasync_sync_data_path();


	void saveWindow(QWidget *widget);
	void restoreWindow(QWidget *widget);


	QString fgcom_exe_path();
	QString fgcom_port();
	QString fgcom_no();
	QString default_fgcom_no();
	QString default_fgcom_port();

	static int runningOs();
	bool paths_sane();
	
	static QString log_file_path();

	QString style_current();
	QString style_default();

private:
	QString _windowName(QWidget *widget);

signals:

public slots:

};

#endif // XSETTINGS_H
