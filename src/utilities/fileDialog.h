/* ********************************************************
   *  fileDialog.h
   *
   *  Created by Geoff R. Mclane, Paris
   *  (C) Dec 2011 GPL2 (or later)
   *
   *  A rather crude getFileName capability
   *
   ******************************************************** */
#ifndef FILEDIALOG_H
#define FILEDIALOG_H

// #include "app_config.h"
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QTreeWidget>
#include <QAbstractItemView>
#include <QStatusBar>

enum COLS{
    C_TYPE = 0,
    C_NAME
};

// these are 'special' paths
#define ST_ROOT     "<root>"    // in linux system - at the 'root'
#define ST_DRIVES "<drives>"    // in windows system - at the 'root' with active DRIVES listed

#define flg_AddExtent   0x0001

#define flg_Default (flg_AddExtent)

class fileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit fileDialog(QWidget *parent = 0);
    // ***TBD*** could also have an instatiation that included the 'title', 'previous' and filter
    void init(QString title = QString(), QString prev = QString(), QStringList filt = QStringList());
    QString getFileName();  // get results. Return "" if user cancelled, and ADDS extension if single filter given, and not added
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
    void on_name_change(QString);
    void on_path_change(QString);


private:
    // no user access to these
    void fill_tree();   // using pathEd->text()
    QLabel *nameLab;    // = new QLabel("Name:",this);
    QLabel *pathLab;    // = new QLabel("Path:",this);
    QLineEdit *nameEd;  // = new QLineEdit(this);
    QLineEdit *pathEd;  // = new QLineEdit(this);
    QLabel *filtLabel1; // = new QLabel("Filter:",this);
    QLabel *filtLabel2; // = new QLabel("*",this);
    QTreeWidget *treeWidget;    // = new QTreeWidget(this);
    int filt_count;     // if user supplies a filter like '*.ini'
};


#endif // FILEDIALOG_H
