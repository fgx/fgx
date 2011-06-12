#ifndef MPMAPWIDGET_H
#define MPMAPWIDGET_H

#include <QtCore/QString>

#include <QtGui/QWidget>
#include <QtGui/QProgressBar>
#include <QtGui/QStatusBar>
#include <QtGui/QComboBox>

#include <QtGui/QCloseEvent>

#include <QWebView>

#include "xobjects/mainobject.h"

class MainObject;

class MpMapWidget : public QWidget
{
Q_OBJECT
public:
	explicit MpMapWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;
	QString *mpmapFollowCallsign;
	
private:
    QWebView *browser;
    QProgressBar *progressBar;
    QStatusBar *statusBar;
    QComboBox *comboServer;
	
    //QString *server_url;

signals:

public slots:
    void start_progress();
    void update_progress(int progress);
    void end_progress(bool Ok);

    void on_combo_server(int index);
    void closeEvent(QCloseEvent *event);
};

#endif // MPMAPWIDGET_H
