#ifndef MAPMAINWINDOW_H
#define MAPMAINWINDOW_H

#include <QMainWindow>


#include "xobjects/mainobject.h"

//#include "marble/MarbleGlobal.h"
#include "marble/MarbleWidget.h"



class MapMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MapMainWindow(MainObject*, QWidget *parent = 0);

    MainObject *mainObject;

    Marble::MarbleWidget *mapWidget;

signals:

public slots:

};

#endif // MAPMAINWINDOW_H
