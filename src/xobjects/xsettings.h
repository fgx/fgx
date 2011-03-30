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


    QString _temp;
    QString temp();
    QString temp(QString append_path);

	QString fgfs_path();

    QString fg_root();
    QString fg_root(QString append_path);

	QString aircraft_path();
	QString aircraft_path(QString dir);


	void saveWindow(QWidget *widget);
	void restoreWindow(QWidget *widget);
	QString _windowName(QWidget *widget);


	int runningOS();
signals:

public slots:

};

#endif // XSETTINGS_H
