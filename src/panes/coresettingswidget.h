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
#include "xwidgets/xmessagelabel.h"

class MainObject;

class CoreSettingsWidget : public QWidget
{
Q_OBJECT
public:
	explicit CoreSettingsWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;
	
	XMessageLabel *messageLabel;


	//* Callsign
	QLineEdit *txtCallSign;

	//* Screen
	QComboBox *comboScreenSize;
	QCheckBox *checkBoxDisableSplashScreen;
	QCheckBox *checkBoxFullScreenStartup;


	//* MpMap
	QCheckBox *checkBoxShowMpMap;
	QComboBox *comboMpMapServer;


	//** Paths
	QButtonGroup *buttonGroupPaths;
	XGroupHBox *groupBoxFgFs;
	XGroupHBox *groupBoxFgRoot;

	QLineEdit *lineEditFgFsPath;
	QLineEdit *lineEditFgRootPath;
	
	QLabel *labelFgfsProgram;
	QLabel *labelFgfsCheck;
	
	QLabel *labelFgRootData;
	QLabel *labelFgRootCheck;
	
	QCheckBox *checkBoxUseTerrasync;
	QLabel *labelTerrasyncProgram;
	QLabel *labelTerrasyncCheck;
	
	QLabel *labelTerrasyncData;
	QLabel *labelTerrasyncDataCheck;
	
	QLineEdit *lineEditTerraSyncDataPath;
	QLineEdit *lineEditTerraSyncExePath;
	
	QCheckBox *checkBoxUseCustomScenery;
	QLabel *labelCustomScene;
	QLabel *labelCustomSceneCheck;
	QLineEdit *lineEditCustomScenePath;
	
	QLabel *labelFGComExeInfo;
	QLineEdit *lineEditFGComExePath;
	
	//* Controls
	QLabel *labelInputs;

	//* Functions
	void initialize();
	QString validate();


signals:
	void setx(QString option, bool enabled, QString value);


public slots:

	void on_callsign_changed(QString);

	void on_screensize();
	void on_checkbox_splash_screen();

	void on_show_mp_map();
	
	void on_terrasync_enabled();
	void on_custom_scenery_enabled();

	void load_joysticks();

	void on_upx(QString option, bool enabled, QString value);
	
	// Checks
	void fgfs_check_path();
	void fgroot_check_path();
	void terrasync_exe_check_path();
	void terrasync_data_check_path();
	void custom_scenery_check_path();
	
};

#endif // CORESETTINGSWIDGET_H
