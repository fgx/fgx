#ifndef MAPSELECTDIALOG_H
#define MAPSELECTDIALOG_H

#include <QDialog>
#include <QTreeView>

#include "xobjects/mainobject.h"
class MainObject;

class MapSelectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MapSelectDialog(MainObject *mob, QWidget *parent = 0);

    MainObject *mainObject;
    QTreeView *tree;

signals:

public slots:

};

#endif // MAPSELECTDIALOG_H
