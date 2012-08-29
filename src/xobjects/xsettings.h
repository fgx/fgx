// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // xsettings.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef XSETTINGS_H
#define XSETTINGS_H

#include <QSettings>

class XSettings : public QSettings
{
Q_OBJECT
public:

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
