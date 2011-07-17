#ifndef XDEBUGTREEWIDGET_H
#define XDEBUGTREEWIDGET_H


#include <QWidget>
#include <QButtonGroup>

#include "xobjects/mainobject.h"
class MainObject;

class XDebugTreeWidget : public QWidget
{
Q_OBJECT
public:
	explicit XDebugTreeWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;

	QButtonGroup *buttonGroupFilter;


signals:

public slots:

};

#endif // XDEBUGTREEWIDGET_H
