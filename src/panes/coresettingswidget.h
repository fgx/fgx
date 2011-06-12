#ifndef CORESETTINGSWIDGET_H
#define CORESETTINGSWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QButtonGroup>

#include "xobjects/mainobject.h"
#include "xwidgets/xgroupboxes.h"

class CoreSettingsWidget : public QWidget
{
Q_OBJECT
public:
	explicit CoreSettingsWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;


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


	//** Paths
	QButtonGroup *buttonGroupPaths;
	XGroupHBox *groupBoxFgFs;
	XGroupHBox *groupBoxFgRoot;

	QLabel *txtFgFs;
	QLabel *txtFgRoot;
	QLabel *FgRootPathLabel;
	QLabel *FgFsPathLabel;

	XGroupHBox *groupBoxTerraSync;
	QLineEdit *txtTerraSyncPath;


	//* Functions
	void initialize();
	QStringList get_args();
	void load_settings();
	void save_settings();


signals:

public slots:
	void on_checkbox_show_mp_map();
	void load_joysticks();

	void on_radio_fg_path();

	void on_button_fgfs_path();
	void on_button_fgroot_path();
	void on_button_terrasync_path();

	void show_settings_dialog();

};

#endif // CORESETTINGSWIDGET_H
