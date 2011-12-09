/* ********************************************************
   *  dirDialog.h
   *
   *  Created by Geoff R. Mclane, Paris
   *  (C) Dec 2011 GPL2 (or later)
   *
   *  A rather crude getFileName capability
   *
   ******************************************************** */
#ifndef DIRDIALOG_H
#define DIRDIALOG_H

#include "app_config.h"
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QTreeWidget>
#include <QAbstractItemView>
#include <QModelIndex>

enum DCOLS{
    D_TYPE = 0,
    D_NAME
};

// these are 'special' paths
#define ST_ROOT     "<root>"    // in linux system - at the 'root'
#define ST_DRIVES "<drives>"    // in windows system - at the 'root' with active DRIVES listed

class dirDialog : public QDialog
{
    Q_OBJECT
public:
    explicit dirDialog(QWidget *parent = 0);
    // ***TBD*** could also have an instatiation that included the 'title', 'previous' and filter
    void init(QString title = QString(), QString prev = QString());
    QString getDirName();  // get results. Return "" if user cancelled
    bool got_cancel;
    // this ONLY applies to WINDOWS
    int getDriveCount();
    int optionFlag;

signals:
    void set_file(QString);

public slots:
    void on_ok();
    void on_cancel();
    void on_tree_selection_changed();
    void on_tree_double_clicked(QModelIndex);
    void on_name_change(QString);
    void on_path_change(QString);


private:
    // no user access to these
    void fill_tree();   // using pathEd->text()
    QGridLayout *mainLayout;    // = new QGridLayout(this);
    QLabel *info1;              // = new QLabel("Enter file Name: or Path: or navigate via the tree list below.",this);
    QLabel *info2;              // = new QLabel("Green if exists. Red if new.",this);
    QLabel *nameLab;    // = new QLabel("Name:",this);
    QLabel *pathLab;    // = new QLabel("Path:",this);
    QLineEdit *nameEd;  // = new QLineEdit(this);
    QLineEdit *pathEd;  // = new QLineEdit(this);
    QTreeWidget *treeWidget;    // = new QTreeWidget(this);
    QPushButton *cancelButton;    // = new QPushButton(this);
    QPushButton *okButton;        // = new QPushButton(this);
};


#endif // DIRDIALOG_H
