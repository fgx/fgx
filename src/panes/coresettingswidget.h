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

	//QRadioButton *radioFgUseDefault;
	//QRadioButton *radioFgUseCustom;
	QLabel *labelFgFsPath;
	QLabel *labelFgRootPath;
	QLabel *labelFgFsInfo;
	QLabel *labelFgRootInfo;

	QLabel *labelTerraSyncInfo;
	QLabel *labelTerraSyncDataPath;
	
	//* Controls
	QLabel *labelInputs;

	//XGroupHBox *groupBoxTerraSync;
	//QLineEdit *txtTerraSyncPath;


	//* Functions
	void initialize();
	QString validate();

	void save_settings();


signals:
	void setx(bool enabled, QString option, QString value);


public slots:

	void on_callsign_changed(QString);
	void on_checkbox_show_mp_map();
	void on_checkbox_fullscreen();
	void load_joysticks();
	void on_radio_fg_path();
	void load_settings();

	void upx(bool enabled, QString option, QString value);
};

#endif // CORESETTINGSWIDGET_H
