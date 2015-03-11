// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // coresettingswidget.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef CORESETTINGSWIDGET_H
#define CORESETTINGSWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QToolButton>

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

    bool fgrootcheck;

    XMessageLabel *messageLabel;

    //* Images
    //QLabel *imageLabel;


    //* Callsign
    QLineEdit *txtCallSign;

    //* Screen
    QComboBox *comboScreenSize;
    QCheckBox *checkBoxDisableSplashScreen;
    QCheckBox *checkBoxFullScreenStartup;

    QHBoxLayout *screenSizeBox;
    QLineEdit *lineEditScreenSizeW;
    QLineEdit *lineEditScreenSizeH;
    //QLabel *lineEditScreenSizeWLabel;
    //QLabel *lineEditScreenSizeHLabel;



    //* MpMap
    QCheckBox *checkBoxShowMpMap;
    QComboBox *comboMpMapServer;


    //** Paths
    QButtonGroup *buttonGroupPaths;
    XGroupHBox *groupBoxFgFs;
    XGroupHBox *groupBoxFgRoot;

    QLineEdit *lineEditFgFsPath;
    QComboBox *comboFgFsPath;
    QLineEdit *lineEditFgRootPath;
    QToolButton *buttonSetFgfsPath;
    QToolButton *buttonSetFgRootPath;
    QLabel *labelFgfsProgram;
    QLabel *labelFgfsCheck;

    QLabel *labelFgRootData;
    QLabel *labelFgRootCheck;

    QCheckBox *checkBoxUseTerrasync;
    QLabel *labelTerrasyncProgram;
    QLabel *labelTerrasyncCheck;
    QToolButton *buttonSetTerrasyncExePath;
    QToolButton *buttonSetTerrasyncDataPath;

    QLabel *labelTerrasyncData;
    QLabel *labelTerrasyncDataCheck;

    QLineEdit *lineEditTerraSyncDataPath;
    QLineEdit *lineEditTerraSyncExePath;

    QCheckBox *checkBoxUseCustomScenery;
    QLabel *labelCustomScene;
    QLabel *labelCustomSceneCheck;
    QLineEdit *lineEditCustomScenePath;
    QToolButton *buttonSetCustomSceneryPath;

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

    void on_show_mp_map();

    void on_fgfs_path(QString);
    void on_fgroot_path(QString);
    void on_terrasync_path(QString);
    void on_terrasync_data_path(QString);
    void on_terrasync_enabled();
    void terrasync_enabled_checkstate();
    void on_custom_scenery_enabled();
    void on_custom_scenery_path(QString);
    void custom_scenery_enabled_checkstate();

    void load_joysticks();

    void on_upx(QString option, bool enabled, QString value);

    // Path checks
    void fgfs_check_path();
    void fgroot_check_path();
    void terrasync_exe_check_path();
    void terrasync_data_check_path();
    void custom_scenery_check_path();

    // "Set" buttons
    void on_select_fgfsbutton();
    void on_select_fgrootbutton();
    void on_select_terrasyncexebutton();
    void on_select_terrasyncdatabutton();
    void on_select_customscenerybutton();

    void reload_lists();

    // Emit settings when changed
    void fgfs_set_path();
    void fgroot_set_path();
    void terrasyncexe_set_path();
    void terrasyncdata_set_path();
    void customscenery_set_path();

    int randInt(int low, int high);

    QString which_fgfs();
    void add_fgfs_paths();

};

#endif // CORESETTINGSWIDGET_H
