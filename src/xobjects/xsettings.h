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

	static QString cache_dir();

	QString fgx_path();
	static QString fgx_current_dir();


	void saveWindow(QWidget *widget);
	void restoreWindow(QWidget *widget);


	QString fgcom_exe_path();
	QString fgcom_port();
	QString fgcom_no();
	QString default_fgcom_no();
	QString default_fgcom_port();

	
	static QString log_file_path();

	QString style_current();
	QString style_default();

private:
	QString _windowName(QWidget *widget);

signals:

public slots:

};

#endif // XSETTINGS_H
