#ifndef FGXDEBUGWIDGET_H
#define FGXDEBUGWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>


#include "xobjects/mainobject.h"
class MainObject;

#include "xwidgets/xdebugtreewidget.h"
class XDebugTreeWidget;

#include "xwidgets/xcommandpreviewidget.h"
class XCommandPrevieWidget;


class FgxDebugWidget : public QWidget
{
Q_OBJECT
public:
	explicit FgxDebugWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;

	XDebugTreeWidget *debugTreeWidget;
	XCommandPrevieWidget *commandPreviewWidget;

signals:

public slots:
	void on_tab(int idx);

	void closeEvent(QCloseEvent *event);
};

#endif // FGXDEBUGWIDGET_H
