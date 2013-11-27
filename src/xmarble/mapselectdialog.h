#ifndef MAPSELECTDIALOG_H
#define MAPSELECTDIALOG_H

#include <QActionGroup>

#include <QDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "xmarble/mapviewsmodel.h"
class MapViewsModel;

#include "xobjects/mainobject.h"
class MainObject;

class MapSelectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MapSelectDialog(MainObject *mob, QWidget *parent = 0);


    MainObject *mainObject;

    QActionGroup *actionGroup;

    QTreeWidget *tree;

signals:
    void open_map_view(QString tab_action, QString view);
public slots:
    void load_tree();

    void on_action(QAction*);
};

#endif // MAPSELECTDIALOG_H
