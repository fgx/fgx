/********************************************************************************
** Form generated from reading UI file 'fgx.ui'
**
** Created: Sun Mar 20 13:48:39 2011
**      by: Qt User Interface Compiler version 4.7.1
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
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *fgStart;
    QTabWidget *tabs;
    QWidget *tab_3;
    QLineEdit *fgfsPath;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *fgdataPath;
    QPushButton *set_fgdata_path_Button;
    QLabel *label_3;
    QCheckBox *useTerraSync;
    QCheckBox *useFGXfgfs;
    QWidget *tab_2;
    QLabel *labelWindow;
    QComboBox *screenSize;
    QCheckBox *disableSplashScreen;
    QCheckBox *useFullScreen;
    QWidget *tab;
    QLabel *labelAircraft;
    QComboBox *airCraft;
    QLabel *aircraftImage;
    QWidget *tab_4;
    QLabel *labelLocation;
    QComboBox *locationIcao;
    QLabel *labelLatitude;
    QLabel *labelLongitude;
    QLineEdit *Latitude;
    QLineEdit *Longitude;
    QLabel *label_runway;
    QComboBox *runWay;
    QComboBox *parkPosition;
    QLabel *sceneryCheck;
    QCheckBox *useCoordinates;
    QCheckBox *useParkPosition;
    QWidget *tab_5;
    QLabel *labelTime;
    QComboBox *timeoftheDay;
    QLabel *label_Weather;
    QComboBox *Weather;
    QPlainTextEdit *metarText;
    QCheckBox *setTime;
    QCheckBox *useMetar;
    QLineEdit *year;
    QLineEdit *month;
    QLineEdit *day;
    QLabel *label_date;
    QLineEdit *second;
    QLineEdit *minute;
    QLineEdit *hour;
    QLabel *label_date_2;
    QLabel *label_4;
    QTabWidget *tabWidget;
    QWidget *tab_8;
    QLabel *label_Multiplayerserver;
    QLabel *label_Multiplayerserver_2;
    QLineEdit *callSign;
    QLabel *label_7;
    QLabel *label_Multiplayerserver_3;
    QLineEdit *multiplayerPort;
    QLabel *label_8;
    QCheckBox *enableMultiplayer;
    QComboBox *pathMultiplayerOut;
    QWidget *tab_6;
    QCheckBox *enableFGCom;
    QLineEdit *fgcomShell;
    QLabel *label_5;
    QLabel *label_port;
    QLineEdit *portFGCOM;
    QLabel *label_6;
    QWidget *tab_7;
    QCheckBox *enableLog;
    QComboBox *logLevel;
    QLabel *label_loglevel;
    QPlainTextEdit *commandLine;
    QLabel *label_loglevel_2;
    QLabel *label_9;
    QStatusBar *statusbar;
    QMenuBar *menubar;

    void setupUi(QMainWindow *mainWindow)
    {
        if (mainWindow->objectName().isEmpty())
            mainWindow->setObjectName(QString::fromUtf8("mainWindow"));
        mainWindow->resize(568, 620);
        mainWindow->setTabShape(QTabWidget::Rounded);
        centralwidget = new QWidget(mainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        fgStart = new QPushButton(centralwidget);
        fgStart->setObjectName(QString::fromUtf8("fgStart"));
        fgStart->setGeometry(QRect(20, 550, 131, 32));
        tabs = new QTabWidget(centralwidget);
        tabs->setObjectName(QString::fromUtf8("tabs"));
        tabs->setGeometry(QRect(20, 60, 521, 281));
        tabs->setTabShape(QTabWidget::Triangular);
        tabs->setUsesScrollButtons(false);
        tabs->setDocumentMode(false);
        tabs->setTabsClosable(false);
        tabs->setMovable(false);
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        fgfsPath = new QLineEdit(tab_3);
        fgfsPath->setObjectName(QString::fromUtf8("fgfsPath"));
        fgfsPath->setGeometry(QRect(20, 40, 351, 22));
        label = new QLabel(tab_3);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 301, 16));
        label_2 = new QLabel(tab_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 80, 241, 16));
        fgdataPath = new QLineEdit(tab_3);
        fgdataPath->setObjectName(QString::fromUtf8("fgdataPath"));
        fgdataPath->setGeometry(QRect(20, 100, 351, 22));
        set_fgdata_path_Button = new QPushButton(tab_3);
        set_fgdata_path_Button->setObjectName(QString::fromUtf8("set_fgdata_path_Button"));
        set_fgdata_path_Button->setGeometry(QRect(390, 95, 114, 32));
        label_3 = new QLabel(tab_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 125, 311, 16));
        useTerraSync = new QCheckBox(tab_3);
        useTerraSync->setObjectName(QString::fromUtf8("useTerraSync"));
        useTerraSync->setGeometry(QRect(20, 160, 271, 20));
        useFGXfgfs = new QCheckBox(tab_3);
        useFGXfgfs->setObjectName(QString::fromUtf8("useFGXfgfs"));
        useFGXfgfs->setGeometry(QRect(395, 40, 101, 20));
        useFGXfgfs->setChecked(true);
        tabs->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        labelWindow = new QLabel(tab_2);
        labelWindow->setObjectName(QString::fromUtf8("labelWindow"));
        labelWindow->setGeometry(QRect(20, 20, 121, 16));
        screenSize = new QComboBox(tab_2);
        screenSize->setObjectName(QString::fromUtf8("screenSize"));
        screenSize->setGeometry(QRect(15, 40, 111, 26));
        screenSize->setEditable(true);
        screenSize->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
        disableSplashScreen = new QCheckBox(tab_2);
        disableSplashScreen->setObjectName(QString::fromUtf8("disableSplashScreen"));
        disableSplashScreen->setGeometry(QRect(240, 43, 241, 20));
        disableSplashScreen->setChecked(true);
        useFullScreen = new QCheckBox(tab_2);
        useFullScreen->setObjectName(QString::fromUtf8("useFullScreen"));
        useFullScreen->setGeometry(QRect(17, 80, 191, 20));
        tabs->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        labelAircraft = new QLabel(tab);
        labelAircraft->setObjectName(QString::fromUtf8("labelAircraft"));
        labelAircraft->setGeometry(QRect(20, 20, 121, 16));
        airCraft = new QComboBox(tab);
        airCraft->setObjectName(QString::fromUtf8("airCraft"));
        airCraft->setGeometry(QRect(16, 40, 161, 26));
        airCraft->setEditable(true);
        aircraftImage = new QLabel(tab);
        aircraftImage->setObjectName(QString::fromUtf8("aircraftImage"));
        aircraftImage->setGeometry(QRect(300, 40, 171, 128));
        aircraftImage->setMinimumSize(QSize(171, 128));
        aircraftImage->setMaximumSize(QSize(171, 128));
        aircraftImage->setLayoutDirection(Qt::LeftToRight);
        aircraftImage->setAutoFillBackground(false);
        tabs->addTab(tab, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        labelLocation = new QLabel(tab_4);
        labelLocation->setObjectName(QString::fromUtf8("labelLocation"));
        labelLocation->setGeometry(QRect(20, 20, 141, 16));
        locationIcao = new QComboBox(tab_4);
        locationIcao->setObjectName(QString::fromUtf8("locationIcao"));
        locationIcao->setGeometry(QRect(17, 40, 160, 26));
        locationIcao->setEditable(true);
        labelLatitude = new QLabel(tab_4);
        labelLatitude->setObjectName(QString::fromUtf8("labelLatitude"));
        labelLatitude->setGeometry(QRect(220, 50, 61, 16));
        labelLongitude = new QLabel(tab_4);
        labelLongitude->setObjectName(QString::fromUtf8("labelLongitude"));
        labelLongitude->setGeometry(QRect(370, 50, 81, 16));
        Latitude = new QLineEdit(tab_4);
        Latitude->setObjectName(QString::fromUtf8("Latitude"));
        Latitude->setGeometry(QRect(220, 72, 120, 22));
        Latitude->setReadOnly(false);
        Longitude = new QLineEdit(tab_4);
        Longitude->setObjectName(QString::fromUtf8("Longitude"));
        Longitude->setGeometry(QRect(370, 72, 120, 22));
        Longitude->setReadOnly(false);
        label_runway = new QLabel(tab_4);
        label_runway->setObjectName(QString::fromUtf8("label_runway"));
        label_runway->setGeometry(QRect(20, 80, 141, 16));
        runWay = new QComboBox(tab_4);
        runWay->setObjectName(QString::fromUtf8("runWay"));
        runWay->setGeometry(QRect(17, 100, 85, 26));
        runWay->setEditable(true);
        parkPosition = new QComboBox(tab_4);
        parkPosition->setObjectName(QString::fromUtf8("parkPosition"));
        parkPosition->setGeometry(QRect(17, 160, 85, 26));
        parkPosition->setEditable(true);
        sceneryCheck = new QLabel(tab_4);
        sceneryCheck->setObjectName(QString::fromUtf8("sceneryCheck"));
        sceneryCheck->setGeometry(QRect(20, 210, 481, 20));
        useCoordinates = new QCheckBox(tab_4);
        useCoordinates->setObjectName(QString::fromUtf8("useCoordinates"));
        useCoordinates->setGeometry(QRect(217, 18, 191, 20));
        useParkPosition = new QCheckBox(tab_4);
        useParkPosition->setObjectName(QString::fromUtf8("useParkPosition"));
        useParkPosition->setGeometry(QRect(19, 135, 151, 20));
        tabs->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        labelTime = new QLabel(tab_5);
        labelTime->setObjectName(QString::fromUtf8("labelTime"));
        labelTime->setGeometry(QRect(20, 20, 51, 16));
        timeoftheDay = new QComboBox(tab_5);
        timeoftheDay->setObjectName(QString::fromUtf8("timeoftheDay"));
        timeoftheDay->setGeometry(QRect(16, 40, 111, 26));
        label_Weather = new QLabel(tab_5);
        label_Weather->setObjectName(QString::fromUtf8("label_Weather"));
        label_Weather->setGeometry(QRect(250, 20, 101, 16));
        Weather = new QComboBox(tab_5);
        Weather->setObjectName(QString::fromUtf8("Weather"));
        Weather->setGeometry(QRect(247, 40, 211, 26));
        metarText = new QPlainTextEdit(tab_5);
        metarText->setObjectName(QString::fromUtf8("metarText"));
        metarText->setGeometry(QRect(250, 120, 251, 78));
        setTime = new QCheckBox(tab_5);
        setTime->setObjectName(QString::fromUtf8("setTime"));
        setTime->setGeometry(QRect(18, 90, 151, 20));
        useMetar = new QCheckBox(tab_5);
        useMetar->setObjectName(QString::fromUtf8("useMetar"));
        useMetar->setGeometry(QRect(250, 90, 141, 20));
        year = new QLineEdit(tab_5);
        year->setObjectName(QString::fromUtf8("year"));
        year->setGeometry(QRect(60, 120, 41, 22));
        year->setMaxLength(4);
        month = new QLineEdit(tab_5);
        month->setObjectName(QString::fromUtf8("month"));
        month->setGeometry(QRect(110, 120, 30, 22));
        month->setMaxLength(2);
        day = new QLineEdit(tab_5);
        day->setObjectName(QString::fromUtf8("day"));
        day->setGeometry(QRect(150, 120, 30, 22));
        day->setMaxLength(2);
        label_date = new QLabel(tab_5);
        label_date->setObjectName(QString::fromUtf8("label_date"));
        label_date->setGeometry(QRect(20, 124, 31, 16));
        second = new QLineEdit(tab_5);
        second->setObjectName(QString::fromUtf8("second"));
        second->setGeometry(QRect(150, 150, 30, 22));
        second->setMaxLength(2);
        minute = new QLineEdit(tab_5);
        minute->setObjectName(QString::fromUtf8("minute"));
        minute->setGeometry(QRect(110, 150, 30, 22));
        minute->setMaxLength(2);
        hour = new QLineEdit(tab_5);
        hour->setObjectName(QString::fromUtf8("hour"));
        hour->setGeometry(QRect(60, 150, 30, 22));
        hour->setMaxLength(2);
        label_date_2 = new QLabel(tab_5);
        label_date_2->setObjectName(QString::fromUtf8("label_date_2"));
        label_date_2->setGeometry(QRect(20, 154, 31, 16));
        tabs->addTab(tab_5, QString());
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 15, 511, 21));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(20, 360, 521, 181));
        tab_8 = new QWidget();
        tab_8->setObjectName(QString::fromUtf8("tab_8"));
        label_Multiplayerserver = new QLabel(tab_8);
        label_Multiplayerserver->setObjectName(QString::fromUtf8("label_Multiplayerserver"));
        label_Multiplayerserver->setGeometry(QRect(230, 15, 141, 16));
        label_Multiplayerserver_2 = new QLabel(tab_8);
        label_Multiplayerserver_2->setObjectName(QString::fromUtf8("label_Multiplayerserver_2"));
        label_Multiplayerserver_2->setGeometry(QRect(30, 75, 141, 16));
        callSign = new QLineEdit(tab_8);
        callSign->setObjectName(QString::fromUtf8("callSign"));
        callSign->setGeometry(QRect(30, 100, 113, 22));
        callSign->setMaxLength(7);
        callSign->setReadOnly(false);
        label_7 = new QLabel(tab_8);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(30, 125, 311, 16));
        label_Multiplayerserver_3 = new QLabel(tab_8);
        label_Multiplayerserver_3->setObjectName(QString::fromUtf8("label_Multiplayerserver_3"));
        label_Multiplayerserver_3->setGeometry(QRect(230, 75, 141, 16));
        multiplayerPort = new QLineEdit(tab_8);
        multiplayerPort->setObjectName(QString::fromUtf8("multiplayerPort"));
        multiplayerPort->setGeometry(QRect(230, 100, 81, 22));
        multiplayerPort->setReadOnly(false);
        label_8 = new QLabel(tab_8);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(230, 125, 311, 16));
        enableMultiplayer = new QCheckBox(tab_8);
        enableMultiplayer->setObjectName(QString::fromUtf8("enableMultiplayer"));
        enableMultiplayer->setGeometry(QRect(30, 40, 181, 20));
        pathMultiplayerOut = new QComboBox(tab_8);
        pathMultiplayerOut->setObjectName(QString::fromUtf8("pathMultiplayerOut"));
        pathMultiplayerOut->setGeometry(QRect(230, 38, 221, 26));
        pathMultiplayerOut->setEditable(true);
        tabWidget->addTab(tab_8, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        enableFGCom = new QCheckBox(tab_6);
        enableFGCom->setObjectName(QString::fromUtf8("enableFGCom"));
        enableFGCom->setGeometry(QRect(20, 15, 181, 20));
        fgcomShell = new QLineEdit(tab_6);
        fgcomShell->setObjectName(QString::fromUtf8("fgcomShell"));
        fgcomShell->setGeometry(QRect(20, 40, 231, 22));
        fgcomShell->setReadOnly(true);
        label_5 = new QLabel(tab_6);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(22, 67, 311, 16));
        label_port = new QLabel(tab_6);
        label_port->setObjectName(QString::fromUtf8("label_port"));
        label_port->setGeometry(QRect(270, 44, 41, 16));
        portFGCOM = new QLineEdit(tab_6);
        portFGCOM->setObjectName(QString::fromUtf8("portFGCOM"));
        portFGCOM->setGeometry(QRect(312, 40, 101, 22));
        label_6 = new QLabel(tab_6);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(313, 65, 311, 31));
        tabWidget->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QString::fromUtf8("tab_7"));
        enableLog = new QCheckBox(tab_7);
        enableLog->setObjectName(QString::fromUtf8("enableLog"));
        enableLog->setGeometry(QRect(320, 20, 161, 20));
        logLevel = new QComboBox(tab_7);
        logLevel->setObjectName(QString::fromUtf8("logLevel"));
        logLevel->setGeometry(QRect(320, 70, 141, 26));
        label_loglevel = new QLabel(tab_7);
        label_loglevel->setObjectName(QString::fromUtf8("label_loglevel"));
        label_loglevel->setGeometry(QRect(323, 50, 62, 16));
        commandLine = new QPlainTextEdit(tab_7);
        commandLine->setObjectName(QString::fromUtf8("commandLine"));
        commandLine->setGeometry(QRect(20, 50, 271, 78));
        label_loglevel_2 = new QLabel(tab_7);
        label_loglevel_2->setObjectName(QString::fromUtf8("label_loglevel_2"));
        label_loglevel_2->setGeometry(QRect(20, 20, 231, 20));
        tabWidget->addTab(tab_7, QString());
        label_9 = new QLabel(centralwidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(170, 555, 371, 20));
        label_9->setOpenExternalLinks(true);
        mainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(mainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        mainWindow->setStatusBar(statusbar);
        menubar = new QMenuBar(mainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 568, 22));
        mainWindow->setMenuBar(menubar);

        retranslateUi(mainWindow);

        tabs->setCurrentIndex(0);
        screenSize->setCurrentIndex(1);
        tabWidget->setCurrentIndex(0);
        logLevel->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(mainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindow)
    {
        mainWindow->setWindowTitle(QApplication::translate("mainWindow", "FlightGear Launcher", 0, QApplication::UnicodeUTF8));
        fgStart->setText(QApplication::translate("mainWindow", "Start FlightGear", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fgfsPath->setToolTip(QApplication::translate("mainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">For hudson nightly builds use paths like this one: <span style=\" color:#0035ff;\">/Users/joefoo/Desktop/nightly/FlightGear.app/Contents/MacOS</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        fgfsPath->setText(QString());
        label->setText(QApplication::translate("mainWindow", "Directory of FlightGear binary (fgfs)", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("mainWindow", "Path to FlightGear data (fgdata)", 0, QApplication::UnicodeUTF8));
        fgdataPath->setText(QString());
#ifndef QT_NO_TOOLTIP
        set_fgdata_path_Button->setToolTip(QApplication::translate("mainWindow", "Click this when you enter a new path to flightgear data. Will refresh aircraft and airport list with the new fgdata path (if exists!)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        set_fgdata_path_Button->setText(QApplication::translate("mainWindow", "refresh", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("mainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; color:#717171;\">i.e. /Users/joefoo/Documents/fgdata</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        useTerraSync->setText(QApplication::translate("mainWindow", "Use TerraSync for scenery download", 0, QApplication::UnicodeUTF8));
        useFGXfgfs->setText(QApplication::translate("mainWindow", "built-in", 0, QApplication::UnicodeUTF8));
        tabs->setTabText(tabs->indexOf(tab_3), QApplication::translate("mainWindow", "Paths and Programs", 0, QApplication::UnicodeUTF8));
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
        labelLocation->setText(QApplication::translate("mainWindow", "Airport (ICAO/IATA)", 0, QApplication::UnicodeUTF8));
        labelLatitude->setText(QApplication::translate("mainWindow", "Latitude", 0, QApplication::UnicodeUTF8));
        labelLongitude->setText(QApplication::translate("mainWindow", "Longitude", 0, QApplication::UnicodeUTF8));
        label_runway->setText(QApplication::translate("mainWindow", "Runway", 0, QApplication::UnicodeUTF8));
        sceneryCheck->setText(QApplication::translate("mainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#717171;\">Scenery Check</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        useCoordinates->setText(QApplication::translate("mainWindow", "Use coordinates:", 0, QApplication::UnicodeUTF8));
        useParkPosition->setText(QApplication::translate("mainWindow", "Use Park Position:", 0, QApplication::UnicodeUTF8));
        tabs->setTabText(tabs->indexOf(tab_4), QApplication::translate("mainWindow", "Location", 0, QApplication::UnicodeUTF8));
        labelTime->setText(QApplication::translate("mainWindow", "Time", 0, QApplication::UnicodeUTF8));
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
        label_Weather->setText(QApplication::translate("mainWindow", "Weather fetch", 0, QApplication::UnicodeUTF8));
        Weather->clear();
        Weather->insertItems(0, QStringList()
         << QApplication::translate("mainWindow", "real", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "disable", 0, QApplication::UnicodeUTF8)
        );
        setTime->setText(QApplication::translate("mainWindow", "Set time", 0, QApplication::UnicodeUTF8));
        useMetar->setText(QApplication::translate("mainWindow", "Use this METAR:", 0, QApplication::UnicodeUTF8));
        year->setText(QApplication::translate("mainWindow", "yyyy", 0, QApplication::UnicodeUTF8));
        month->setText(QApplication::translate("mainWindow", "mm", 0, QApplication::UnicodeUTF8));
        day->setText(QApplication::translate("mainWindow", "dd", 0, QApplication::UnicodeUTF8));
        label_date->setText(QApplication::translate("mainWindow", "Date", 0, QApplication::UnicodeUTF8));
        second->setText(QApplication::translate("mainWindow", "ss", 0, QApplication::UnicodeUTF8));
        minute->setText(QApplication::translate("mainWindow", "mm", 0, QApplication::UnicodeUTF8));
        hour->setText(QApplication::translate("mainWindow", "hh", 0, QApplication::UnicodeUTF8));
        label_date_2->setText(QApplication::translate("mainWindow", "Time", 0, QApplication::UnicodeUTF8));
        tabs->setTabText(tabs->indexOf(tab_5), QApplication::translate("mainWindow", "Time and Weather", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("mainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600; color:#0035ff;\">FGX 2.2pre</span><span style=\" font-weight:600; color:#ff1500;\"> </span><span style=\" color:#717171;\">FlightGear launcher for OSX (with built-in FlightGear 2.2pre)</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_Multiplayerserver->setText(QApplication::translate("mainWindow", "Multiplayer server", 0, QApplication::UnicodeUTF8));
        label_Multiplayerserver_2->setText(QApplication::translate("mainWindow", "Your Callsign", 0, QApplication::UnicodeUTF8));
        callSign->setText(QString());
        label_7->setText(QApplication::translate("mainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; color:#717171;\">max. 7 letters</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_Multiplayerserver_3->setText(QApplication::translate("mainWindow", "Multiplayer port", 0, QApplication::UnicodeUTF8));
        multiplayerPort->setText(QApplication::translate("mainWindow", "5000", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("mainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; color:#717171;\">5000 is standard.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
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
        tabWidget->setTabText(tabWidget->indexOf(tab_8), QApplication::translate("mainWindow", "Multiplayer", 0, QApplication::UnicodeUTF8));
        enableFGCom->setText(QApplication::translate("mainWindow", "Enable FGCom", 0, QApplication::UnicodeUTF8));
        fgcomShell->setText(QApplication::translate("mainWindow", "fgcom -Sfgcom.flightgear.org.uk", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("mainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; color:#717171;\">Call FlightGear default fgcom server.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_port->setText(QApplication::translate("mainWindow", "Port", 0, QApplication::UnicodeUTF8));
        portFGCOM->setText(QApplication::translate("mainWindow", "16661", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("mainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; color:#717171;\">Default FGCOM UDP Port </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QApplication::translate("mainWindow", "Voice Communication", 0, QApplication::UnicodeUTF8));
        enableLog->setText(QApplication::translate("mainWindow", "Write log files to disk", 0, QApplication::UnicodeUTF8));
        logLevel->clear();
        logLevel->insertItems(0, QStringList()
         << QApplication::translate("mainWindow", "warn", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "info", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "debug", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "bulk", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("mainWindow", "alert", 0, QApplication::UnicodeUTF8)
        );
        label_loglevel->setText(QApplication::translate("mainWindow", "Log-level", 0, QApplication::UnicodeUTF8));
        label_loglevel_2->setText(QApplication::translate("mainWindow", "Add other command line options:", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_7), QApplication::translate("mainWindow", "Command line / Log", 0, QApplication::UnicodeUTF8));
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
