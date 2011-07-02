#ifndef PROPEDITWIDGET_H
#define PROPEDITWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidgetItem>

#include "xobjects/mainobject.h"


class PropEditWidget : public QDialog
{
Q_OBJECT
public:
	explicit PropEditWidget( MainObject *mOb, QDialog *parent = 0);

	 MainObject *mainObject;

	 QLabel *labelNode;
	 QLineEdit *txtValue;
	 QLabel * labelType;

	void set_from_item(QTreeWidgetItem *item);

signals:

public slots:
	void on_set_property();
};

#endif // PROPEDITWIDGET_H
