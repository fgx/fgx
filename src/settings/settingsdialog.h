// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // settingsdialog.h
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#ifndef SettingsDialog_H
#define SettingsDialog_H

#include <QString>

#include <QDialog>
#include <QWidget>
//#include <QStatusBar>
#include <QPushButton>
#include <QTabWidget>

#include "xobjects/mainobject.h"


class SettingsDialog : public QDialog
{
Q_OBJECT
public:
	explicit SettingsDialog(MainObject *mOb, QWidget *parent = 0);

	QTabWidget *tabWidget;

private:
	MainObject *mainObject;






	QString get_frame_style(bool is_valid);
    void disable_scenery_actions(bool);

	void closeEvent(QCloseEvent *event);
	bool validate_paths();

signals:

public slots:


	void load_settings();
	void on_save_clicked();

};

#endif // SettingsDialog_H
