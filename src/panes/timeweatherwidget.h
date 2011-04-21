#ifndef TIMEWEATHERWIDGET_H
#define TIMEWEATHERWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QButtonGroup>
#include <QtGui/QPlainTextEdit>

class TimeWeatherWidget : public QWidget
{
Q_OBJECT
public:
    explicit TimeWeatherWidget(QWidget *parent = 0);

	QButtonGroup *buttonGroupTime;
	QButtonGroup *buttonGroupSeason;

	QButtonGroup *buttonGroupMetar;
	QPlainTextEdit *txtMetar;

signals:

public slots:
	void on_metar_clicked(int idx);
};

#endif // TIMEWEATHERWIDGET_H
