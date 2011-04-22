#ifndef TIMEWEATHERWIDGET_H
#define TIMEWEATHERWIDGET_H


#include <QtCore/QString>
#include <QtCore/QMap>


#include <QtGui/QWidget>
#include <QtGui/QButtonGroup>
#include <QtGui/QPlainTextEdit>


#include "xobjects/mainobject.h"

class TimeWeatherWidget : public QWidget
{
Q_OBJECT
public:
	explicit TimeWeatherWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;

	QMap<QString, QString> mpMapServers;

	QButtonGroup *buttonGroupTime;
	QButtonGroup *buttonGroupSeason;

	QButtonGroup *buttonGroupMetar;
	QPlainTextEdit *txtMetar;

	QStringList get_args();
	void load_settings();
	void save_settings();

signals:

public slots:
	void on_metar_clicked(int idx);
};

#endif // TIMEWEATHERWIDGET_H
