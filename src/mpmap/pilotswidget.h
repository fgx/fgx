#ifndef PILOTSWIDGET_H
#define PILOTSWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QStatusBar>

#include "xobjects/mainobject.h"

class MainObject;

class PilotsWidget : public QWidget
{
Q_OBJECT
public:
	explicit PilotsWidget(MainObject *mob, QWidget *parent = 0);

	enum COLS{
		C_CALLSIGN,
		C_AERO,
		C_ALTITUDE
	};

	MainObject *mainObject;

	QTreeWidget *tree;

	QStatusBar *statusBar;

signals:

public slots:

};

#endif // PILOTSWIDGET_H
