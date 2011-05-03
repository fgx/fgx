#ifndef DBSETTINGSWIDGET_H
#define DBSETTINGSWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>

#include "xobjects/mainobject.h"

class DbSettingsWidget : public QWidget
{
Q_OBJECT
public:
	explicit DbSettingsWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;

	QComboBox *comboDbType;
	QLineEdit *txtHost;
	QLineEdit *txtUser;
	QLineEdit *txtPass;
	QLineEdit *txtDatabase;
	QPushButton *buttonTest;
	QStatusBar *statusBar;
signals:

public slots:

	void load_settings();
	void save_settings();

	void on_combo_engine();
	void on_test_connection();
};

#endif // DBSETTINGSWIDGET_H
