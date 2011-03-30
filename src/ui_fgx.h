/********************************************************************************
** Form generated from reading UI file 'fgx.ui'
**
** Created: Wed Mar 30 13:58:46 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FGX_H
#define UI_FGX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWindow
{
public:
    QAction *actionAbout_FGX;
    QAction *actionAbout_FlightGear;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *fgx_logo;
    QTabWidget *tabs;
    QWidget *toptab;
    QGridLayout *gridLayout_4;
    QLabel *label;
    QLineEdit *fgfsPath;
    QCheckBox *useFGXfgfs;
    QLabel *label_2;
    QLineEdit *fgdataPath;
    QPushButton *set_fgdata_path_Button;
    QSpacerItem *verticalSpacer_5;
    QLabel *label_10;
    QCheckBox *useTerraSync;
    QSpacerItem *verticalSpacer_6;
    QLabel *label_3;
    QWidget *tab_2;
    QGridLayout *gridLayout_6;
    QLabel *labelWindow;
    QComboBox *screenSize;
    QCheckBox *disableSplashScreen;
    QCheckBox *useFullScreen;
    QSpacerItem *verticalSpacer_7;
    QWidget *tab;
    QGridLayout *gridLayout_7;
    QLabel *labelAircraft;
    QLabel *aircraftImage;
    QComboBox *airCraft;
    QSpacerItem *verticalSpacer_9;
    QSpacerItem *verticalSpacer_8;
    QWidget *tab_4;
    QGridLayout *gridLayout_9;
    QCheckBox *usecustomScenery;
    QLabel *labelLocation;
    QComboBox *locationIcao;
    QLabel *label_runway;
    QComboBox *runWay;
    QCheckBox *useParkPosition;
    QSpacerItem *verticalSpacer_10;
    QComboBox *parkPosition;
    QLabel *sceneryCheck;
    QSpacerItem *verticalSpacer_11;
    QGroupBox *useCoordinates;
    QGridLayout *gridLayout_8;
    QLabel *labelLatitude;
    QLabel *labelLongitude;
    QLineEdit *Latitude;
    QLineEdit *Longitude;
    QWidget *tab_5;
    QGridLayout *gridLayout_5;
    QGroupBox *groupBoxSetTime;
    QGridLayout *gridLayout;
    QLabel *label_date;
    QLineEdit *year;
    QLineEdit *month;
    QLineEdit *day;
    QLabel *label_date_2;
    QLineEdit *hour;
    QLineEdit *minute;
    QLineEdit *second;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QComboBox *Weather;
    QCheckBox *useMetar;
    QPlainTextEdit *metarText;
    QGroupBox *groupBoxTime;
    QVBoxLayout *verticalLayout_2;
    QComboBox *timeoftheDay;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_3;
    QTabWidget *tabWidget;
    QWidget *bottomtab;
    QGridLayout *gridLayout_2;
    QCheckBox *enableAITraffic;
    QLabel *label_Multiplayerserver;
    QCheckBox *enableMultiplayer;
    QComboBox *pathMultiplayerOut;
    QLabel *label_Multiplayerserver_2;
    QLabel *label_Multiplayerserver_3;
    QLineEdit *callSign;
    QLineEdit *multiplayerPort;
    QLabel *label_5;
    QLabel *label_6;
    QSpacerItem *verticalSpacer_4;
    QWidget *tab_6;
    QGridLayout *gridLayout_3;
    QCheckBox *enableFGCom;
    QLabel *label_4;
    QLabel *label_port;
    QLineEdit *fgcomShell;
    QLineEdit *portFGCOM;
    QLabel *lblDefaultFgComServer;
    QLabel *labelDefaultFgComPort;
    QSpacerItem *verticalSpacer;
    QWidget *tab_7;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_loglevel_2;
    QPlainTextEdit *commandLine;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *enableLog;
    QLabel *label_loglevel;
    QComboBox *logLevel;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *fgStart;
    QLabel *label_9;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *mainWindow)
    {
        if (mainWindow->objectName().isEmpty())
            mainWindow->setObjectName(QString::fromUtf8("mainWindow"));
        mainWindow->resize(792, 664);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/favicon"), QSize(), QIcon::Normal, QIcon::Off);
        mainWindow->setWindowIcon(icon);
        mainWindow->setAutoFillBackground(false);
        mainWindow->setStyleSheet(QString::fromUtf8(""));
        mainWindow->setTabShape(QTabWidget::Rounded);
        actionAbout_FGX = new QAction(mainWindow);
        actionAbout_FGX->setObjectName(QString::fromUtf8("actionAbout_FGX"));
        actionAbout_FlightGear = new QAction(mainWindow);
        actionAbout_FlightGear->setObjectName(QString::fromUtf8("actionAbout_FlightGear"));
        centralwidget = new QWidget(mainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        fgx_logo = new QLabel(centralwidget);
        fgx_logo->setObjectName(QString::fromUtf8("fgx_logo"));
        fgx_logo->setStyleSheet(QString::fromUtf8(""));
        fgx_logo->setPixmap(QPixmap(QString::fromUtf8("../../originaldateien/logo-version-release22.png")));
        fgx_logo->setMargin(10);

        verticalLayout->addWidget(fgx_logo);

        tabs = new QTabWidget(centralwidget);
        tabs->setObjectName(QString::fromUtf8("tabs"));
        tabs->setAutoFillBackground(true);
        tabs->setStyleSheet(QString::fromUtf8(""));
        tabs->setTabShape(QTabWidget::Rounded);
        tabs->setUsesScrollButtons(false);
        tabs->setDocumentMode(false);
        tabs->setTabsClosable(false);
        tabs->setMovable(false);
        toptab = new QWidget();
        toptab->setObjectName(QString::fromUtf8("toptab"));
        gridLayout_4 = new QGridLayout(toptab);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label = new QLabel(toptab);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_4->addWidget(label, 0, 0, 1, 1);

        fgfsPath = new QLineEdit(toptab);
        fgfsPath->setObjectName(QString::fromUtf8("fgfsPath"));

        gridLayout_4->addWidget(fgfsPath, 1, 0, 1, 1);

        useFGXfgfs = new QCheckBox(toptab);
        useFGXfgfs->setObjectName(QString::fromUtf8("useFGXfgfs"));
        useFGXfgfs->setChecked(true);

        gridLayout_4->addWidget(useFGXfgfs, 1, 1, 1, 1);

        label_2 = new QLabel(toptab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_4->addWidget(label_2, 2, 0, 1, 1);

        fgdataPath = new QLineEdit(toptab);
        fgdataPath->setObjectName(QString::fromUtf8("fgdataPath"));

        gridLayout_4->addWidget(fgdataPath, 3, 0, 1, 1);

        set_fgdata_path_Button = new QPushButton(toptab);
        set_fgdata_path_Button->setObjectName(QString::fromUtf8("set_fgdata_path_Button"));

        gridLayout_4->addWidget(set_fgdata_path_Button, 3, 1, 3, 1);

        verticalSpacer_5 = new QSpacerItem(663, 44, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_5, 6, 0, 1, 1);

        label_10 = new QLabel(toptab);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_4->addWidget(label_10, 7, 0, 1, 1);

        useTerraSync = new QCheckBox(toptab);
        useTerraSync->setObjectName(QString::fromUtf8("useTerraSync"));

        gridLayout_4->addWidget(useTerraSync, 8, 0, 1, 1);

        verticalSpacer_6 = new QSpacerItem(663, 44, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_6, 9, 0, 1, 1);

        label_3 = new QLabel(toptab);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_4->addWidget(label_3, 4, 0, 1, 1);

        tabs->addTab(toptab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_6 = new QGridLayout(tab_2);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        labelWindow = new QLabel(tab_2);
        labelWindow->setObjectName(QString::fromUtf8("labelWindow"));

        gridLayout_6->addWidget(labelWindow, 0, 0, 1, 1);

        screenSize = new QComboBox(tab_2);
        screenSize->setObjectName(QString::fromUtf8("screenSize"));
        screenSize->setEditable(true);
        screenSize->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);

        gridLayout_6->addWidget(screenSize, 1, 0, 1, 1);

        disableSplashScreen = new QCheckBox(tab_2);
        disableSplashScreen->setObjectName(QString::fromUtf8("disableSplashScreen"));
        disableSplashScreen->setChecked(true);

        gridLayout_6->addWidget(disableSplashScreen, 1, 1, 1, 1);

        useFullScreen = new QCheckBox(tab_2);
        useFullScreen->setObjectName(QString::fromUtf8("useFullScreen"));

        gridLayout_6->addWidget(useFullScreen, 2, 0, 1, 1);

        verticalSpacer_7 = new QSpacerItem(20, 193, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_7, 3, 0, 1, 1);

        tabs->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_7 = new QGridLayout(tab);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        labelAircraft = new QLabel(tab);
        labelAircraft->setObjectName(QString::fromUtf8("labelAircraft"));

        gridLayout_7->addWidget(labelAircraft, 0, 0, 1, 1);

        aircraftImage = new QLabel(tab);
        aircraftImage->setObjectName(QString::fromUtf8("aircraftImage"));
        aircraftImage->setMinimumSize(QSize(171, 128));
        aircraftImage->setMaximumSize(QSize(171, 128));
        aircraftImage->setLayoutDirection(Qt::LeftToRight);
        aircraftImage->setAutoFillBackground(false);
        aircraftImage->setOpenExternalLinks(false);

        gridLayout_7->addWidget(aircraftImage, 0, 1, 3, 1);

        airCraft = new QComboBox(tab);
        airCraft->setObjectName(QString::fromUtf8("airCraft"));
        airCraft->setEditable(true);

        gridLayout_7->addWidget(airCraft, 1, 0, 1, 1);

        verticalSpacer_9 = new QSpacerItem(20, 105, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer_9, 2, 0, 1, 1);

        verticalSpacer_8 = new QSpacerItem(20, 104, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer_8, 3, 1, 1, 1);

        tabs->addTab(tab, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        gridLayout_9 = new QGridLayout(tab_4);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        usecustomScenery = new QCheckBox(tab_4);
        usecustomScenery->setObjectName(QString::fromUtf8("usecustomScenery"));
        usecustomScenery->setChecked(true);

        gridLayout_9->addWidget(usecustomScenery, 0, 0, 1, 2);

        labelLocation = new QLabel(tab_4);
        labelLocation->setObjectName(QString::fromUtf8("labelLocation"));

        gridLayout_9->addWidget(labelLocation, 1, 0, 1, 1);

        locationIcao = new QComboBox(tab_4);
        locationIcao->setObjectName(QString::fromUtf8("locationIcao"));
        locationIcao->setEditable(true);

        gridLayout_9->addWidget(locationIcao, 2, 0, 1, 1);

        label_runway = new QLabel(tab_4);
        label_runway->setObjectName(QString::fromUtf8("label_runway"));

        gridLayout_9->addWidget(label_runway, 3, 0, 1, 1);

        runWay = new QComboBox(tab_4);
        runWay->setObjectName(QString::fromUtf8("runWay"));
        runWay->setEditable(true);

        gridLayout_9->addWidget(runWay, 4, 0, 1, 1);

        useParkPosition = new QCheckBox(tab_4);
        useParkPosition->setObjectName(QString::fromUtf8("useParkPosition"));

        gridLayout_9->addWidget(useParkPosition, 5, 0, 1, 1);

        verticalSpacer_10 = new QSpacerItem(244, 46, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_10, 5, 2, 2, 1);

        parkPosition = new QComboBox(tab_4);
        parkPosition->setObjectName(QString::fromUtf8("parkPosition"));
        parkPosition->setEditable(true);

        gridLayout_9->addWidget(parkPosition, 6, 0, 1, 1);

        sceneryCheck = new QLabel(tab_4);
        sceneryCheck->setObjectName(QString::fromUtf8("sceneryCheck"));

        gridLayout_9->addWidget(sceneryCheck, 7, 0, 1, 3);

        verticalSpacer_11 = new QSpacerItem(244, 54, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_11, 8, 0, 1, 1);

        useCoordinates = new QGroupBox(tab_4);
        useCoordinates->setObjectName(QString::fromUtf8("useCoordinates"));
        useCoordinates->setCheckable(true);
        gridLayout_8 = new QGridLayout(useCoordinates);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        labelLatitude = new QLabel(useCoordinates);
        labelLatitude->setObjectName(QString::fromUtf8("labelLatitude"));

        gridLayout_8->addWidget(labelLatitude, 0, 0, 1, 1);

        labelLongitude = new QLabel(useCoordinates);
        labelLongitude->setObjectName(QString::fromUtf8("labelLongitude"));

        gridLayout_8->addWidget(labelLongitude, 0, 1, 1, 1);

        Latitude = new QLineEdit(useCoordinates);
        Latitude->setObjectName(QString::fromUtf8("Latitude"));
        Latitude->setReadOnly(false);

        gridLayout_8->addWidget(Latitude, 1, 0, 1, 1);

        Longitude = new QLineEdit(useCoordinates);
        Longitude->setObjectName(QString::fromUtf8("Longitude"));
        Longitude->setReadOnly(false);

        gridLayout_8->addWidget(Longitude, 1, 1, 1, 1);


        gridLayout_9->addWidget(useCoordinates, 1, 1, 3, 2);

        tabs->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        gridLayout_5 = new QGridLayout(tab_5);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        groupBoxSetTime = new QGroupBox(tab_5);
        groupBoxSetTime->setObjectName(QString::fromUtf8("groupBoxSetTime"));
        groupBoxSetTime->setCheckable(true);
        gridLayout = new QGridLayout(groupBoxSetTime);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_date = new QLabel(groupBoxSetTime);
        label_date->setObjectName(QString::fromUtf8("label_date"));

        gridLayout->addWidget(label_date, 0, 0, 1, 1);

        year = new QLineEdit(groupBoxSetTime);
        year->setObjectName(QString::fromUtf8("year"));
        year->setMaxLength(4);

        gridLayout->addWidget(year, 0, 1, 1, 1);

        month = new QLineEdit(groupBoxSetTime);
        month->setObjectName(QString::fromUtf8("month"));
        month->setMaxLength(2);

        gridLayout->addWidget(month, 0, 2, 1, 1);

        day = new QLineEdit(groupBoxSetTime);
        day->setObjectName(QString::fromUtf8("day"));
        day->setMaxLength(2);

        gridLayout->addWidget(day, 0, 3, 1, 1);

        label_date_2 = new QLabel(groupBoxSetTime);
        label_date_2->setObjectName(QString::fromUtf8("label_date_2"));

        gridLayout->addWidget(label_date_2, 1, 0, 1, 1);

        hour = new QLineEdit(groupBoxSetTime);
        hour->setObjectName(QString::fromUtf8("hour"));
        hour->setMaxLength(2);

        gridLayout->addWidget(hour, 1, 1, 1, 1);

        minute = new QLineEdit(groupBoxSetTime);
        minute->setObjectName(QString::fromUtf8("minute"));
        minute->setMaxLength(2);

        gridLayout->addWidget(minute, 1, 2, 1, 1);

        second = new QLineEdit(groupBoxSetTime);
        second->setObjectName(QString::fromUtf8("second"));
        second->setMaxLength(2);

        gridLayout->addWidget(second, 1, 3, 1, 1);


        gridLayout_5->addWidget(groupBoxSetTime, 0, 0, 1, 3);

        groupBox = new QGroupBox(tab_5);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        Weather = new QComboBox(groupBox);
        Weather->setObjectName(QString::fromUtf8("Weather"));

        verticalLayout_3->addWidget(Weather);

        useMetar = new QCheckBox(groupBox);
        useMetar->setObjectName(QString::fromUtf8("useMetar"));

        verticalLayout_3->addWidget(useMetar);

        metarText = new QPlainTextEdit(groupBox);
        metarText->setObjectName(QString::fromUtf8("metarText"));

        verticalLayout_3->addWidget(metarText);


        gridLayout_5->addWidget(groupBox, 0, 3, 3, 1);

        groupBoxTime = new QGroupBox(tab_5);
        groupBoxTime->setObjectName(QString::fromUtf8("groupBoxTime"));
        verticalLayout_2 = new QVBoxLayout(groupBoxTime);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        timeoftheDay = new QComboBox(groupBoxTime);
        timeoftheDay->setObjectName(QString::fromUtf8("timeoftheDay"));

        verticalLayout_2->addWidget(timeoftheDay);


        gridLayout_5->addWidget(groupBoxTime, 1, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(298, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer, 1, 2, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 107, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_3, 2, 1, 1, 1);

        tabs->addTab(tab_5, QString());

        verticalLayout->addWidget(tabs);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setStyleSheet(QString::fromUtf8(""));
        bottomtab = new QWidget();
        bottomtab->setObjectName(QString::fromUtf8("bottomtab"));
        gridLayout_2 = new QGridLayout(bottomtab);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        enableAITraffic = new QCheckBox(bottomtab);
        enableAITraffic->setObjectName(QString::fromUtf8("enableAITraffic"));

        gridLayout_2->addWidget(enableAITraffic, 0, 0, 1, 1);

        label_Multiplayerserver = new QLabel(bottomtab);
        label_Multiplayerserver->setObjectName(QString::fromUtf8("label_Multiplayerserver"));

        gridLayout_2->addWidget(label_Multiplayerserver, 0, 1, 1, 1);

        enableMultiplayer = new QCheckBox(bottomtab);
        enableMultiplayer->setObjectName(QString::fromUtf8("enableMultiplayer"));

        gridLayout_2->addWidget(enableMultiplayer, 1, 0, 1, 1);

        pathMultiplayerOut = new QComboBox(bottomtab);
        pathMultiplayerOut->setObjectName(QString::fromUtf8("pathMultiplayerOut"));
        pathMultiplayerOut->setEditable(true);

        gridLayout_2->addWidget(pathMultiplayerOut, 1, 1, 1, 1);

        label_Multiplayerserver_2 = new QLabel(bottomtab);
        label_Multiplayerserver_2->setObjectName(QString::fromUtf8("label_Multiplayerserver_2"));

        gridLayout_2->addWidget(label_Multiplayerserver_2, 2, 0, 1, 1);

        label_Multiplayerserver_3 = new QLabel(bottomtab);
        label_Multiplayerserver_3->setObjectName(QString::fromUtf8("label_Multiplayerserver_3"));

        gridLayout_2->addWidget(label_Multiplayerserver_3, 2, 1, 1, 1);

        callSign = new QLineEdit(bottomtab);
        callSign->setObjectName(QString::fromUtf8("callSign"));
        callSign->setMaxLength(7);
        callSign->setReadOnly(false);

        gridLayout_2->addWidget(callSign, 3, 0, 1, 1);

        multiplayerPort = new QLineEdit(bottomtab);
        multiplayerPort->setObjectName(QString::fromUtf8("multiplayerPort"));
        multiplayerPort->setReadOnly(false);

        gridLayout_2->addWidget(multiplayerPort, 3, 1, 1, 1);

        label_5 = new QLabel(bottomtab);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 4, 0, 1, 1);

        label_6 = new QLabel(bottomtab);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 4, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_4, 5, 0, 1, 1);

        tabWidget->addTab(bottomtab, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        gridLayout_3 = new QGridLayout(tab_6);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        enableFGCom = new QCheckBox(tab_6);
        enableFGCom->setObjectName(QString::fromUtf8("enableFGCom"));

        gridLayout_3->addWidget(enableFGCom, 0, 0, 1, 1);

        label_4 = new QLabel(tab_6);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_3->addWidget(label_4, 1, 0, 1, 1);

        label_port = new QLabel(tab_6);
        label_port->setObjectName(QString::fromUtf8("label_port"));

        gridLayout_3->addWidget(label_port, 1, 1, 1, 1);

        fgcomShell = new QLineEdit(tab_6);
        fgcomShell->setObjectName(QString::fromUtf8("fgcomShell"));
        fgcomShell->setReadOnly(true);

        gridLayout_3->addWidget(fgcomShell, 2, 0, 1, 1);

        portFGCOM = new QLineEdit(tab_6);
        portFGCOM->setObjectName(QString::fromUtf8("portFGCOM"));

        gridLayout_3->addWidget(portFGCOM, 2, 1, 1, 1);

        lblDefaultFgComServer = new QLabel(tab_6);
        lblDefaultFgComServer->setObjectName(QString::fromUtf8("lblDefaultFgComServer"));

        gridLayout_3->addWidget(lblDefaultFgComServer, 3, 0, 1, 1);

        labelDefaultFgComPort = new QLabel(tab_6);
        labelDefaultFgComPort->setObjectName(QString::fromUtf8("labelDefaultFgComPort"));

        gridLayout_3->addWidget(labelDefaultFgComPort, 3, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 4, 0, 1, 1);

        tabWidget->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QString::fromUtf8("tab_7"));
        horizontalLayout_2 = new QHBoxLayout(tab_7);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_loglevel_2 = new QLabel(tab_7);
        label_loglevel_2->setObjectName(QString::fromUtf8("label_loglevel_2"));

        verticalLayout_5->addWidget(label_loglevel_2);

        commandLine = new QPlainTextEdit(tab_7);
        commandLine->setObjectName(QString::fromUtf8("commandLine"));

        verticalLayout_5->addWidget(commandLine);


        horizontalLayout_2->addLayout(verticalLayout_5);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        enableLog = new QCheckBox(tab_7);
        enableLog->setObjectName(QString::fromUtf8("enableLog"));

        verticalLayout_4->addWidget(enableLog);

        label_loglevel = new QLabel(tab_7);
        label_loglevel->setObjectName(QString::fromUtf8("label_loglevel"));

        verticalLayout_4->addWidget(label_loglevel);

        logLevel = new QComboBox(tab_7);
        logLevel->setObjectName(QString::fromUtf8("logLevel"));

        verticalLayout_4->addWidget(logLevel);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);


        horizontalLayout_2->addLayout(verticalLayout_4);

        tabWidget->addTab(tab_7, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(20);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        fgStart = new QPushButton(centralwidget);
        fgStart->setObjectName(QString::fromUtf8("fgStart"));

        horizontalLayout->addWidget(fgStart);

        label_9 = new QLabel(centralwidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setOpenExternalLinks(true);
        label_9->setTextInteractionFlags(Qt::TextBrowserInteraction);

        horizontalLayout->addWidget(label_9);


        verticalLayout->addLayout(horizontalLayout);

        mainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(mainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        mainWindow->setStatusBar(statusbar);

        retranslateUi(mainWindow);

        tabs->setCurrentIndex(0);
        screenSize->setCurrentIndex(1);
        tabWidget->setCurrentIndex(2);
        logLevel->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(mainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindow)
    {
        mainWindow->setWindowTitle(QApplication::translate("mainWindow", "FlightGear Launcher", 0, QApplication::UnicodeUTF8));
        actionAbout_FGX->setText(QApplication::translate("mainWindow", "About FGX", 0, QApplication::UnicodeUTF8));
        actionAbout_FlightGear->setText(QApplication::translate("mainWindow", "About FlightGear", 0, QApplication::UnicodeUTF8));
        fgx_logo->setText(QApplication::translate("mainWindow", "SSSSSSSSSSSSSSSSSSSSSSS", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("mainWindow", "Directory of FlightGear binary (fgfs)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fgfsPath->setToolTip(QApplication::translate("mainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Example for development version installed: </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#0035ff;\">/usr/local/bin</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Example for hudson nightly builds:</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#0035ff;\">/Users/joef"
                        "oo/Desktop/nightly/FlightGear.app/Contents/MacOS</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#0035ff;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        fgfsPath->setText(QString());
#ifndef QT_NO_TOOLTIP
        useFGXfgfs->setToolTip(QApplication::translate("mainWindow", "Check this box when you want to use built-in FlightGear/data", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        useFGXfgfs->setText(QApplication::translate("mainWindow", "built-in", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("mainWindow", "Path to FlightGear data (fgdata)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fgdataPath->setToolTip(QApplication::translate("mainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ff1500;\">Click Refresh</span> button after editing this path!</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        fgdataPath->setText(QString());
#ifndef QT_NO_TOOLTIP
        set_fgdata_path_Button->setToolTip(QApplication::translate("mainWindow", "Click this when you enter a new path to flightgear data. Will refresh aircraft and airport list with the new fgdata path (if exists!)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        set_fgdata_path_Button->setText(QApplication::translate("mainWindow", "refresh", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("mainWindow", "Custom scenery", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        useTerraSync->setToolTip(QApplication::translate("mainWindow", "Check this box when terrasync should start to download latest custom scenery", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        useTerraSync->setText(QApplication::translate("mainWindow", "Use Terrasync for custom scenery download", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("mainWindow", "i.e. /Users/joefoo/Documents/fgdata", 0, QApplication::UnicodeUTF8));
        tabs->setTabText(tabs->indexOf(toptab), QApplication::translate("mainWindow", "Paths and Programs", 0, QApplication::UnicodeUTF8));
        labelWindow->setText(QApplication::translate("mainWindow", "Window size", 0, QApplication::UnicodeUTF8));
        screenSize->clear();
        screenSize->insertItems(0, QStringList()
         << QApplication::translate("mainWindow", "800x600", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "1024x768", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "1280x1024", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "1600x900", 0, QApplication::UnicodeUTF8)
        );
        disableSplashScreen->setText(QApplication::translate("mainWindow", "Disable Splash Screen", 0, QApplication::UnicodeUTF8));
        useFullScreen->setText(QApplication::translate("mainWindow", "Use full screen mode", 0, QApplication::UnicodeUTF8));
        tabs->setTabText(tabs->indexOf(tab_2), QApplication::translate("mainWindow", "Screen", 0, QApplication::UnicodeUTF8));
        labelAircraft->setText(QApplication::translate("mainWindow", "Aircraft", 0, QApplication::UnicodeUTF8));
        aircraftImage->setText(QApplication::translate("mainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">No image.</p></body></html>", 0, QApplication::UnicodeUTF8));
        tabs->setTabText(tabs->indexOf(tab), QApplication::translate("mainWindow", "Aircraft", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        usecustomScenery->setToolTip(QApplication::translate("mainWindow", "Don\342\200\231t forget to check Terrasync download at Paths and Programs tab", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        usecustomScenery->setText(QApplication::translate("mainWindow", "Use custom scenery", 0, QApplication::UnicodeUTF8));
        labelLocation->setText(QApplication::translate("mainWindow", "Airport (ICAO/IATA)", 0, QApplication::UnicodeUTF8));
        label_runway->setText(QApplication::translate("mainWindow", "Runway", 0, QApplication::UnicodeUTF8));
        useParkPosition->setText(QApplication::translate("mainWindow", "Use Park Position:", 0, QApplication::UnicodeUTF8));
        sceneryCheck->setText(QApplication::translate("mainWindow", "Scenery Check", 0, QApplication::UnicodeUTF8));
        useCoordinates->setTitle(QApplication::translate("mainWindow", "Use coordinates", 0, QApplication::UnicodeUTF8));
        labelLatitude->setText(QApplication::translate("mainWindow", "Latitude", 0, QApplication::UnicodeUTF8));
        labelLongitude->setText(QApplication::translate("mainWindow", "Longitude", 0, QApplication::UnicodeUTF8));
        tabs->setTabText(tabs->indexOf(tab_4), QApplication::translate("mainWindow", "Location", 0, QApplication::UnicodeUTF8));
        groupBoxSetTime->setTitle(QApplication::translate("mainWindow", "Set Time", 0, QApplication::UnicodeUTF8));
        label_date->setText(QApplication::translate("mainWindow", "Date", 0, QApplication::UnicodeUTF8));
        year->setText(QApplication::translate("mainWindow", "yyyy", 0, QApplication::UnicodeUTF8));
        month->setText(QApplication::translate("mainWindow", "mm", 0, QApplication::UnicodeUTF8));
        day->setText(QApplication::translate("mainWindow", "dd", 0, QApplication::UnicodeUTF8));
        label_date_2->setText(QApplication::translate("mainWindow", "Time", 0, QApplication::UnicodeUTF8));
        hour->setText(QApplication::translate("mainWindow", "hh", 0, QApplication::UnicodeUTF8));
        minute->setText(QApplication::translate("mainWindow", "mm", 0, QApplication::UnicodeUTF8));
        second->setText(QApplication::translate("mainWindow", "ss", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("mainWindow", "Weather Fetch", 0, QApplication::UnicodeUTF8));
        Weather->clear();
        Weather->insertItems(0, QStringList()
         << QApplication::translate("mainWindow", "real", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "disable", 0, QApplication::UnicodeUTF8)
        );
        useMetar->setText(QApplication::translate("mainWindow", "Use this METAR:", 0, QApplication::UnicodeUTF8));
        groupBoxTime->setTitle(QApplication::translate("mainWindow", "Time", 0, QApplication::UnicodeUTF8));
        timeoftheDay->clear();
        timeoftheDay->insertItems(0, QStringList()
         << QApplication::translate("mainWindow", "real", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "dawn", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "morning", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "noon", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "afternoon", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "dusk", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "evening", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "midnight", 0, QApplication::UnicodeUTF8)
        );
        tabs->setTabText(tabs->indexOf(tab_5), QApplication::translate("mainWindow", "Time and Weather", 0, QApplication::UnicodeUTF8));
        enableAITraffic->setText(QApplication::translate("mainWindow", "Enable AI traffic", 0, QApplication::UnicodeUTF8));
        label_Multiplayerserver->setText(QApplication::translate("mainWindow", "Multiplayer server", 0, QApplication::UnicodeUTF8));
        enableMultiplayer->setText(QApplication::translate("mainWindow", "Enable Multiplayer", 0, QApplication::UnicodeUTF8));
        pathMultiplayerOut->clear();
        pathMultiplayerOut->insertItems(0, QStringList()
         << QApplication::translate("mainWindow", "mpserver02.flightgear.org", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "mpserver03.flightgear.org", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "mpserver04.flightgear.org", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "mpserver05.flightgear.org", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "mpserver06.flightgear.org", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "mpserver07.flightgear.org", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "mpserver08.flightgear.org", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "mpserver09.flightgear.org", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "mpserver10.flightgear.org", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "mpserver11.flightgear.org", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "mpserver12.flightgear.org", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "mpserver13.flightgear.org", 0, QApplication::UnicodeUTF8)
        );
        label_Multiplayerserver_2->setText(QApplication::translate("mainWindow", "Your Callsign", 0, QApplication::UnicodeUTF8));
        label_Multiplayerserver_3->setText(QApplication::translate("mainWindow", "Multiplayer port", 0, QApplication::UnicodeUTF8));
        callSign->setText(QString());
        multiplayerPort->setText(QApplication::translate("mainWindow", "5000", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("mainWindow", "max. 7 letters", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("mainWindow", "5000 is standard.", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(bottomtab), QApplication::translate("mainWindow", "Multiplayer", 0, QApplication::UnicodeUTF8));
        enableFGCom->setText(QApplication::translate("mainWindow", "Enable FGCom", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("mainWindow", "Server", 0, QApplication::UnicodeUTF8));
        label_port->setText(QApplication::translate("mainWindow", "UDP Port", 0, QApplication::UnicodeUTF8));
        fgcomShell->setText(QApplication::translate("mainWindow", "fgcom -Sfgcom.flightgear.org.uk", 0, QApplication::UnicodeUTF8));
        portFGCOM->setText(QApplication::translate("mainWindow", "16661", 0, QApplication::UnicodeUTF8));
        lblDefaultFgComServer->setText(QApplication::translate("mainWindow", "Call FlightGear default fgcom server", 0, QApplication::UnicodeUTF8));
        labelDefaultFgComPort->setText(QApplication::translate("mainWindow", "Default FGCOM UDP Port ", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QApplication::translate("mainWindow", "Voice Communication", 0, QApplication::UnicodeUTF8));
        label_loglevel_2->setText(QApplication::translate("mainWindow", "Add other command line options:", 0, QApplication::UnicodeUTF8));
        enableLog->setText(QApplication::translate("mainWindow", "Write log files to disk", 0, QApplication::UnicodeUTF8));
        label_loglevel->setText(QApplication::translate("mainWindow", "Log-level", 0, QApplication::UnicodeUTF8));
        logLevel->clear();
        logLevel->insertItems(0, QStringList()
         << QApplication::translate("mainWindow", "warn", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "info", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "debug", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "bulk", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "alert", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(tab_7), QApplication::translate("mainWindow", "Command line / Log", 0, QApplication::UnicodeUTF8));
        fgStart->setText(QApplication::translate("mainWindow", "Start FlightGear", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("mainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt; font-weight:600; color:#717171;\">FGX</span><span style=\" font-size:9pt; color:#717171;\"> \302\251 2011 Gral aka Yves Sablonier, Zurich //  </span><a href=\"http://www.gnu.org/licenses/gpl-2.0.txt\"><span style=\" font-size:9pt; text-decoration: underline; color:#0000ff;\">GPL2</span></a><span style=\" font-size:9pt; color:#717171;\"> // </span><a href=\"http://wiki.flightgear.org\"><span style=\" font-size:9pt; text-decoration: underline; color:#0000ff;\">FlightGear</span></a><span style=\" font-size:9pt; color:#717171;\"> l"
                        "auncher for OSX</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class mainWindow: public Ui_mainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FGX_H
