#include "mapmainwindow.h"

#include "xobjects/xsettings.h"

//using namespace Marble;


MapMainWindow::MapMainWindow(MainObject *mob, QWidget *parent) :
    QMainWindow(parent)
{

    this->mainObject = mob;

    this->setWindowTitle("FGx - The Utlimate Map ;)");
    this->setWindowIcon(QIcon(":/icon/map_type"));


    this->marbleWidget = new Marble::MarbleWidget();
    this->setCentralWidget(this->marbleWidget);

    XSettings::autosizeWindow(this, 100, 100);
}
