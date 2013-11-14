#include "mapmainwindow.h"

MapMainWindow::MapMainWindow(MainObject *mob, QWidget *parent) :
    QMainWindow(parent)
{

    this->mainObject = mob;


    this->setWindowTitle("FGx - The Utlimate Map ;)");
    this->setWindowIcon(QIcon(":/icon/map_type"));

}
