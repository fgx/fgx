#ifndef CORESETTINGSWIDGET_H
#define CORESETTINGSWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>

#include "xobjects/xsettings.h"

class CoreSettingsWidget : public QWidget
{
Q_OBJECT
public:
	explicit CoreSettingsWidget(QWidget *parent = 0);

	XSettings settings;

	//* Screen
	QComboBox *comboScreenSize;
	QCheckBox *checkBoxDisableSplashScreen;
	QCheckBox *checkBoxFullScreenStartup;

	//* Controls
	QCheckBox *checkBoxEnableAutoCoordination;
	QComboBox *comboJoystick;

	//* MpMap
	QCheckBox *checkBoxShowMpMap;
	QComboBox *comboMpMapServer;


	QStringList get_args();
	void load_settings();
	void save_settings();


signals:

public slots:
	void on_checkbox_show_mp_map();

};

#endif // CORESETTINGSWIDGET_H
