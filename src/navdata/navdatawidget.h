#ifndef NAVDATAWIDGET_H
#define NAVDATAWIDGET_H

#include <QString>

#include <QWidget>
#include <QLineEdit>
#include <QTreeView>

#include "navdatamodel.h"
class NavDataModel;


class NavDataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NavDataWidget(QWidget *parent = 0);

    NavDataModel *navDataModel;

    QLineEdit *txtSearch;

    QTreeView *tree;

signals:

public slots:

    void on_text_changed(QString);
    void on_return_pressed();
    void on_search();

};

#endif // NAVDATAWIDGET_H
