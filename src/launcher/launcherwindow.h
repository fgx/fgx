// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // launcherwindow.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef LAUNCHERWINDOW_H
#define LAUNCHERWINDOW_H

#include <QWidget>

#include <QAction>
#include <QActionGroup>
#include <QPushButton>
#include <QMessageBox>


#include "panes/coresettingswidget.h"
class CoreSettingsWidget;

#include "panes/timeweatherwidget.h"
class TimeWeatherWidget;

#include "panes/renderingwidget.h"
class RenderingWidget;

#include "aircraft/aircraftwidget.h"
class AircraftWidget;

#include "airports/airportswidget.h"
class AirportsWidget;

#include "network/networkwidget.h"
class NetworkWidget;

#include "panes/expertoptionswidget.h"
class ExpertOptionsWidget;

#include "xwidgets/execontrols.h"
class ExeControls;

#include "launcher/headerwidget.h"
class HeaderWidget;

#include "launcher/menuwidget.h"
class MenuWidget;

#include "utilities/statusbar.h"

#include "utilities/messagebox.h"

#include "xobjects/mainobject.h"
#include "xwidgets/xgroupboxes.h"




class LauncherWindow : public QWidget
{
    Q_OBJECT

public:

    LauncherWindow(MainObject *mob, QWidget *parent = 0);

    ~LauncherWindow();

    MainObject *mainObject;
    bool initializing;

    MenuWidget *menuWidget;
    HeaderWidget *headerWidget;

    XGroupHBox *toolBox;

    QActionGroup *actionGroupStyle;
    QTabWidget *tabWidget;

    CoreSettingsWidget *coreSettingsWidget;
    TimeWeatherWidget *timeWeatherWidget;
    RenderingWidget *renderingWidget;
    AircraftWidget *aircraftWidget;
    AirportsWidget *airportsWidget;
    NetworkWidget *networkWidget;
    ExpertOptionsWidget *expertOptionsWidget;

    QPushButton *buttonLoadSettings;
    QPushButton *buttonSaveSettings;
    QPushButton *buttonResetSettings;

    ExeControls *exeAll;
    ExeControls *exeFgfs;
    ExeControls *exeTerraSync;

    StatusBar *statusBar;
    MessageBox *messageBox;

    QMessageBox *NativeMessageBox;

    void closeEvent(QCloseEvent *event);
    void moveEvent(QMoveEvent *ev);

signals:
    void setx(QString option, bool enabled, QString value);

public slots:


    //= Start/Stop
    void on_start_all_clicked();
    void on_start_fgfs_clicked();
    void on_start_terrasync_clicked();



    //* Functions
    void save_settings();
    void load_profile();
    void save_profile();
    void reset_profile();
    void initialize();
    bool validate();

    //* Header messages
    void header_show_message(QString message);


    //* Ouput Buttons
    void on_command_preview();


    //* Widget Events
    void on_tab_changed(int);

    void on_upx(QString option, bool enabled, QString value);

    void on_debug_mode();
};

#endif // LAUNCHERWINDOW_H
