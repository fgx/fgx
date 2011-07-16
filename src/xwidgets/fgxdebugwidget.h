#ifndef FGXDEBUGWIDGET_H
#define FGXDEBUGWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>

#include "xobjects/mainobject.h"

class MainObject;

class FgxDebugWidget : public QWidget
{
Q_OBJECT
public:
	explicit FgxDebugWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;

	QPlainTextEdit *txtCommand;

signals:

public slots:
	void on_tab(int idx);


};

#endif // FGXDEBUGWIDGET_H
