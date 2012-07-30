// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // renderingwidget.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef RENDERINGWIDGET_H
#define RENDERINGWIDGET_H


#include <QtCore/QString>
#include <QtCore/QMap>


#include <QtGui/QWidget>
#include <QtGui/QButtonGroup>
#include <QtGui/QPlainTextEdit>


#include "xobjects/mainobject.h"
class MainObject;


class RenderingWidget : public QWidget
{
Q_OBJECT
public:
	explicit RenderingWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;

	//QMap<QString, QString> mpMapServers;

	//QButtonGroup *buttonGroupTime;
	//QButtonGroup *buttonGroupSeason;

	//QButtonGroup *buttonGroupMetar;
	//QPlainTextEdit *txtMetar;

	//QString validate();

signals:
	//void setx(QString option, bool enabled, QString value);

public slots:
	//void on_weather();
	//void on_time_clicked();
	//void on_season_clicked(int);

	//void on_upx(QString option, bool enabled, QString value);
};

#endif // RENDERINGWIDGET_H
