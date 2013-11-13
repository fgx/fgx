// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // execontrols.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef EXECONTROLS_H
#define EXECONTROLS_H

#include <QString>
#include <QStringList>

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QToolButton>
#include <QStatusBar>
#include <QProcess>

class ExeControls : public QGroupBox
{
Q_OBJECT
public:
	explicit ExeControls(QString title, QWidget *parent = 0);
	
	QToolButton *buttonStart;
	QToolButton *buttonStop;

signals:
	void start();
	void stop();


public slots:
	void on_start_clicked();
	void on_stop_clicked();

	void set_running(bool running);
};

#endif // EXECONTROLS_H
