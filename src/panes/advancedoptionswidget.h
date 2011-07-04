#ifndef ADVANCEDOPTIONSWIDGET_H
#define ADVANCEDOPTIONSWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QButtonGroup>
#include <QtGui/QLineEdit>

#include "xwidgets/xgroupboxes.h"
#include "xobjects/mainobject.h"

class MainObject;


class AdvancedOptionsWidget : public QWidget
{
Q_OBJECT
public:
	explicit AdvancedOptionsWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;

	QPlainTextEdit *txtExtraArgs;
	QPlainTextEdit *txtExtraEnv;
	QLineEdit *txtRuntime;

	XGroupVBox *groupBoxWriteLog;
	QButtonGroup *buttonGroupLogOptions;

	QStringList get_args();
	QStringList get_env();
	QString     get_runtime();

	void load_settings();
	void save_settings();

signals:

public slots:

};

#endif // ADVANCEDOPTIONSWIDGET_H
