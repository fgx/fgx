#ifndef CORESETTINGSWIDGET_H
#define CORESETTINGSWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QButtonGroup>
#include <QtGui/QRadioButton>

#include "xobjects/mainobject.h"
#include "xwidgets/xgroupboxes.h"

class CoreSettingsWidget : public QWidget
{
Q_OBJECT
public:
	explicit CoreSettingsWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;


	//* Callsign
	QLineEdit *txtCallSign;

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

	//QRadioButton *radioFgUseDefault;
	//QRadioButton *radioFgUseCustom;
	QLabel *labelFgFsPath;
	QLabel *labelFgRootPath;
	QLabel *labelFgFsInfo;
	QLabel *labelFgRootInfo;

	QLabel *labelTerraSyncInfo;
	QLabel *labelTerraSyncDataPath;

	//XGroupHBox *groupBoxTerraSync;
	//QLineEdit *txtTerraSyncPath;


	//* Functions
	void initialize();


	void save_settings();


signals:

public slots:

	void on_callsign_changed(QString);
	void on_checkbox_show_mp_map();
	void load_joysticks();
	bool check_js_demo_exists();

	void on_radio_fg_path();

	//void on_button_fgfs_path();
	//void on_button_fgroot_path();
	//void on_button_terrasync_path();

	//void show_settings_dialog();
	void show_setup_wizard();
	void load_settings();
};

#endif // CORESETTINGSWIDGET_H
