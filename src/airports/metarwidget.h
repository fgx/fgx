#ifndef METARWIDGET_H
#define METARWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>



#include "xobjects/mainobject.h"
class MainObject;


class MetarWidget : public QWidget
{
Q_OBJECT
public:
	explicit MetarWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;

	QPlainTextEdit *txtMetar;


signals:

public slots:
	void load_metar(QString apt);
	void clear_metar();

};

#endif // METARWIDGET_H
