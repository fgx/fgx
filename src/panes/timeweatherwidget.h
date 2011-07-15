#ifndef TIMEWEATHERWIDGET_H
#define TIMEWEATHERWIDGET_H


#include <QtCore/QString>
#include <QtCore/QMap>


#include <QtGui/QWidget>
#include <QtGui/QButtonGroup>
#include <QtGui/QPlainTextEdit>


#include "xobjects/mainobject.h"
class MainObject;


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


	void load_settings();
	void save_settings();
	QString validate();

signals:
	void setx(QString option, bool enabled, QString value);

public slots:
	void on_metar_clicked();
	void on_time_clicked(int);
	void on_season_clicked(int);

	void on_upx(QString option, bool enabled, QString value);
};

#endif // TIMEWEATHERWIDGET_H
