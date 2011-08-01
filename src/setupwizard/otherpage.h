#ifndef OTHERPAGE_H
#define OTHERPAGE_H

#include <QWizardPage>
#include <QCheckBox>
#include <QLineEdit>
#include <QToolButton>
#include <QLabel>


#include "xobjects/mainobject.h"

class OtherPage : public QWizardPage
{
Q_OBJECT
public:
	explicit OtherPage(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;

	//QToolButton *buttFgComPath;

	QLabel *lblFgCom;
	QLineEdit *txtFgCom;

	QLabel *lblJoystickDemo;
	QLineEdit *txtJoystickDemo;

	//QLabel *terraSyncExePathLabel;
	//QLineEdit *txtTerraSyncExePath;
	//QLabel *lblHelp;
	//QLabel *lblHelpExe;


signals:
    void setx(QString option, bool enabled, QString value);

public slots:
	//void on_checkbox_clicked();
	void on_select_fgcom_path();
	void on_select_joystick_path();

	void on_fgcom_autodetect();
	void on_joystick_autodetect();

    bool check_fgcom(QString exe);
    bool check_jsdemo(QString exe);
	void check_paths();
    void on_fgcom();
    void on_jsdemo();
    //void check_exe_paths();

	void initializePage();
	bool validatePage();
};

#endif // OTHERPAGE_H
