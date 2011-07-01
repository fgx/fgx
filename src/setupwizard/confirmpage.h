#ifndef CONFIRMPAGE_H
#define CONFIRMPAGE_H

#include<QtDebug>


#include <QWizardPage>
#include <QCheckBox>
#include <QLineEdit>
#include <QToolButton>
#include <QLabel>
#include <QCheckBox>


#include "xobjects/mainobject.h"

class ConfirmPage : public QWizardPage
{
Q_OBJECT
public:
	explicit ConfirmPage(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;

	QLabel *lblFgExeUsingDefault;
	QLabel *lblFgExePath;

	QLabel *lblFgRootUsingDefault;
	QLabel *lblFgRootPath;

	QLabel *lblUsingTerraSync;
	QLabel *lblTerraSyncPath;

	QCheckBox *checkBoxImportAirports;
	QCheckBox *checkBoxImportAircaft;

	void initializePage();
	bool validatePage();
signals:

public slots:

};

#endif // CONFIRMPAGE_H
