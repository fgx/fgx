#ifndef PROPSTREEWIDGET_H
#define PROPSTREEWIDGET_H



#include <QtCore/QTimer>
#include <QtCore/QStringList>

#include <QtGui/QWidget>

#include <QtGui/QTreeView>
#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>
#include <QtGui/QSortFilterProxyModel>
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

	QTreeView *tree;
	QStandardItemModel *model;
	QSortFilterProxyModel *proxyModel;


	QComboBox *comboHost;
	QLineEdit *txtPort;
	QAction *actionConnect;
	QAction *actionDisconnect;

    QCheckBox *chkAutoRefresh;
    QComboBox *comboAutoRefreshRate;

    QStatusBar *statusBarTree;

	QAction *actionEditProperty;

	void closeEvent(QCloseEvent *event);
	QList<QStandardItem *> make_row(QString node_name, QString node_value, QString node_type, QString full_path, bool is_folder);

signals:

public slots:
    void load_nodes();

	void on_props_folder(QString, QString);
	void on_props_value(QString, QString, QString, QString);

    void on_item_expanded(QTreeWidgetItem *);
	void on_item_double_clicked(QTreeWidgetItem *, int );
	void on_item_selection_changed();

    void on_auto_refresh_enabled();
    void on_set_timer_rate();

	void on_edit_property();
	void on_test_();

	void telnet_connect();
	void telnet_disconnect();

	void on_telnet_connected(bool);
};

#endif // PROPSTREEWIDGET_H
