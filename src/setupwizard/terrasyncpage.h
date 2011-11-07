#ifndef TERRASYNCPAGE_H
#define TERRASYNCPAGE_H

#include <QWizardPage>
#include <QCheckBox>
#include <QLineEdit>
#include <QToolButton>
#include <QLabel>


#include "xobjects/mainobject.h"

class TerraSyncPage : public QWizardPage
{
Q_OBJECT
public:
	explicit TerraSyncPage(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;
	QCheckBox *checkBoxUseTerrasync;
	QToolButton *buttSelectDataPath;
	QToolButton *buttSelectExePath;
	QLabel *terraSyncPathLabel;
	QLineEdit *txtTerraSyncPath;
	QLabel *terraSyncExePathLabel;
	QLineEdit *txtTerraSyncExePath;
	QLabel *lblHelp;
	QLabel *lblHelpExe;
	
	QCheckBox *checkBoxUseCustomScenery;
	QLabel *customScenePathLabel;
	QLineEdit *txtCustomScenePath;
	QToolButton *buttSelectCustomDataPath;
	QLabel *lblHelpCust;
	


signals:

public slots:
	void on_checkbox_clicked();
	void on_checkbox_scenery_clicked();
	void on_select_data_path();
	void on_select_exe_path();
	void on_select_customscene_data_path();
	void check_data_paths();
	void check_exe_paths();
	void check_custom_data_paths();

	void initializePage();
	bool validatePage();
};

#endif // TERRASYNCPAGE_H
