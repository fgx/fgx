#ifndef PATHSSETTINGSWIDGET_H
#define PATHSSETTINGSWIDGET_H


#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>

#include "xobjects/mainobject.h"

class PathsSettingsWidget : public QWidget
{
Q_OBJECT
public:
	explicit PathsSettingsWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;

	QCheckBox *checkBoxUseDefaults;
	QGroupBox *grpFgfs;
	QGroupBox *grpFgRoot;
	QGroupBox *grpFgScenery;

	QLineEdit *txtFgfs;
	QLineEdit *txtFgRoot;


	QPushButton *buttSceneryUp;
	QPushButton *buttSceneryDown;
	QPushButton *buttSceneryRemove;


signals:

public slots:
	void on_select_fgfs_path();
	void on_fgfs_autodetect();
	void on_select_fg_root_path();
	void on_use_defaults();

	void load_settings();
	void save_settings();

	QString get_frame_style(bool);
	void disable_scenery_actions(bool);

	bool validate_paths();


};

#endif // PATHSSETTINGSWIDGET_H
