#ifndef FGXDEBUGWIDGET_H
#define FGXDEBUGWIDGET_H

#include <QWidget>

#include "xobjects/mainobject.h"

class MainObject;

class FgxDebugWidget : public QWidget
{
Q_OBJECT
public:
	explicit FgxDebugWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;

signals:

public slots:



};

#endif // FGXDEBUGWIDGET_H
