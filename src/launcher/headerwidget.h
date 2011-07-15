#ifndef XHEADERWIDGET_H
#define XHEADERWIDGET_H

#include <QLabel>
#include <QTimer>

#include "xobjects/mainobject.h"
class MainObject;


class HeaderWidget : public QLabel
{
Q_OBJECT
public:
	explicit HeaderWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;
	QTimer *fadeTimer;

	QWidget *popWidget;
	QLabel *popLabel;


signals:

public slots:


	void showMessage(QString message, int timeout);
	void showMessage(QString message);
	

	void start_fade();
	void on_fade_timer();
};

#endif // XHEADERWIDGET_H
