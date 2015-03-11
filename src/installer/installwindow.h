// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // installwindow.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2015
// Pete Morgan
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef INSTALL_WINDOW_H
#define INSTALL_WINDOW_H

#include <QWidget>
#include <QPlainTextEdit>


#include "xobjects/mainobject.h"


class InstallWindow : public QWidget
{
Q_OBJECT
public:
    explicit InstallWindow(MainObject *mob, QWidget *parent = 0);

    MainObject *mainObject;

   // QPlainTextEdit *txtMetar;


signals:

public slots:
    //void load_metar(QString apt);
    //void clear_metar();

};

#endif // INSTALL_WINDOW_H
