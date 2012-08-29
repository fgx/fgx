// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // xdebugtreewidget.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef XDEBUGTREEWIDGET_H
#define XDEBUGTREEWIDGET_H


#include <QWidget>
#include <QActionGroup>
#include <QLineEdit>
#include <QSortFilterProxyModel>

#include "xobjects/mainobject.h"
class MainObject;



class XDebugTreeWidget : public QWidget
{
Q_OBJECT

public:
	explicit XDebugTreeWidget(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;

	QButtonGroup *groupFilter;

	QSortFilterProxyModel *proxyModel;

	QTreeView *tree;

	QLineEdit *txtFilter;

signals:


public slots:
	void on_filter_button(QAbstractButton *button);
	void on_filter_text_changed(QString);
	void on_select_a_node(QString option, bool enabled, QString value);
};

#endif // XDEBUGTREEWIDGET_H
