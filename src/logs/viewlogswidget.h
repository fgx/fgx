#ifndef VIEWLOGSWIDGET_H
#define VIEWLOGSWIDGET_H


#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include <QtGui/QTabWidget>

#include "xobjects/mainobject.h"

class MainObject;

class ViewLogsWidget : public QWidget
{
Q_OBJECT
public:
	explicit ViewLogsWidget(MainObject *mob, QWidget *parent = 0);
    ~ViewLogsWidget();

	MainObject *mainObject;

	QTabWidget *tabWidget;


	QPlainTextEdit *txtFgFsLog;
	QPlainTextEdit *txtTerraSyncLog;
	QPlainTextEdit *txtFgComLog;

	QPlainTextEdit *txtFgxLog;
	QPushButton *buttonFgxRefresh;

    QString stgFgFsLog;
    QString stgTerraSyncLog;
    QString stgFgComLog;
    QString stgFgxLog;

	void add_log(QString log_name, QString log);
	void clear_log(QString log_name );

	void closeEvent(QCloseEvent *event);

signals:

public slots:
	void on_load_fgx_log_file();
};

#endif // VIEWLOGSWIDGET_H
