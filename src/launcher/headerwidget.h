#ifndef XHEADERWIDGET_H
#define XHEADERWIDGET_H

#include <QTimer>


#include <QWidget>
#include <QLabel>


#include "xobjects/mainobject.h"
class MainObject;


class HeaderWidget : public QWidget
{
Q_OBJECT
public:
	explicit HeaderWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;

	//QTimer *fadeTimer;

	QWidget *headerWidget;
	QLabel *headerLabel;

	QWidget *popWidget;
	QLabel *popLabel;

	void setText(QString text);



signals:

public slots:


	void showMessage(QString message, int timeout);
	void showMessage(QString message);
	
};

#endif // XHEADERWIDGET_H
