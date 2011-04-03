#ifndef SettingsDialog_H
#define SettingsDialog_H



#include <QtCore/QString>

#include <QtGui/QDialog>
#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QStatusBar>
#include <QtGui/QTreeWidget>
#include <QtGui/QPushButton>
#include <QtGui/QGroupBox>
#include <QtGui/QCheckBox>

#include "xobjects/xsettings.h"

class SettingsDialog : public QDialog
{
Q_OBJECT
public:
	explicit SettingsDialog(QWidget *parent = 0);

private:
	XSettings settings;

	QCheckBox *checkBoxUseDefaults;
	QGroupBox *grpFgfs;
    QGroupBox *grpFgRoot;
    QGroupBox *grpFgScenery;

	QLineEdit *txtFgfs;
    QLineEdit *txtFgRoot;
    QTreeWidget *treeFgScenery;
    QStatusBar *statusBar;

    QPushButton *buttSceneryUp;
    QPushButton *buttSceneryDown;
    QPushButton *buttSceneryRemove;


	QString get_frame_style(bool is_valid);
    void disable_scenery_actions(bool);

	void closeEvent(QCloseEvent *event);
	bool validate_paths();

signals:

public slots:
	void on_select_fgfs_path();
	void on_fgfs_autodetect();
	void on_select_fg_root_path();

	void load_settings();
	void on_save_clicked();
	void on_use_defaults();
};

#endif // SettingsDialog_H
