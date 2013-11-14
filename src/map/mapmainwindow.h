#ifndef MAPMAINWINDOW_H
#define MAPMAINWINDOW_H

#include <QMainWindow>

#include "xobjects/mainobject.h"

class MapMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MapMainWindow(MainObject*, QWidget *parent = 0);

    MainObject *mainObject;

signals:

public slots:

};

#endif // MAPMAINWINDOW_H
