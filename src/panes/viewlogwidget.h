#ifndef VIEWLOGWIDGET_H
#define VIEWLOGWIDGET_H


#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

#include "xobjects/mainobject.h"

class MainObject;

class ViewLogWidget : public QWidget
{
Q_OBJECT
public:
	explicit ViewLogWidget(MainObject *mob, QWidget *parent = 0);


	MainObject *mainObject;

	QPlainTextEdit *txtLog;
	QPushButton *buttonRefresh;
	
	void closeEvent(QCloseEvent *event);

signals:

public slots:
	void on_load_log_file();
};

#endif // VIEWLOGWIDGET_H
