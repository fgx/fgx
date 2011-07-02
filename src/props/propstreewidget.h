#ifndef PROPSTREEWIDGET_H
#define PROPSTREEWIDGET_H



#include <QtCore/QTimer>
#include <QtCore/QStringList>

#include <QtGui/QWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QStatusBar>

#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QAction>

#include "xobjects/mainobject.h"
#include "props/telnetslave.h"

class MainObject;

class PropsTreeWidget : public QWidget
{
Q_OBJECT
public:

	enum COLS{
		C_NODE = 0,
		C_VALUE = 1,
		C_TYPE = 2,
		C_PATH = 3
	};


    explicit PropsTreeWidget(MainObject *mOb, QWidget *parent = 0);

    MainObject *mainObject;

	TelnetSlave *telnet;

    QTimer *timer;
    QStringList refreshList;

    QTreeWidget *treeWidget;

    QCheckBox *chkAutoRefresh;
    QComboBox *comboAutoRefreshRate;

    QStatusBar *statusBarTree;

	QAction *actionEditProperty;

	void closeEvent(QCloseEvent *event);


signals:

public slots:
    void load_nodes();

    void on_props_path(QString, QString);
    void on_props_node(QString, QString, QString, QString);
    void on_item_expanded(QTreeWidgetItem *);
	void on_item_double_clicked(QTreeWidgetItem *, int );
	void on_item_selection_changed();

    void on_auto_refresh_enabled();
    void on_set_timer_rate();

	void on_edit_property();
	void on_test_();
};

#endif // PROPSTREEWIDGET_H
