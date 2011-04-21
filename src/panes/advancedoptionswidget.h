#ifndef ADVANCEDOPTIONSWIDGET_H
#define ADVANCEDOPTIONSWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QButtonGroup>

#include "xobjects/xsettings.h"

class AdvancedOptionsWidget : public QWidget
{
Q_OBJECT
public:
    explicit AdvancedOptionsWidget(QWidget *parent = 0);

	XSettings settings;

	QPlainTextEdit *txtExtraArgs;

	QGroupBox *groupBoxWriteLog;
	QButtonGroup *buttonGroupLogOptions;

	QStringList get_args();
	void load_settings();
	void save_settings();

signals:

public slots:

};

#endif // ADVANCEDOPTIONSWIDGET_H
