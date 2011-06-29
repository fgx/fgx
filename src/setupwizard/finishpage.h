#ifndef FINISHPAGE_H
#define FINISHPAGE_H

#include<QtDebug>


#include <QWizardPage>
#include <QCheckBox>
#include <QLineEdit>
#include <QToolButton>
#include <QLabel>


#include "xobjects/mainobject.h"

class FinishPage : public QWizardPage
{
Q_OBJECT
public:
	explicit FinishPage(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;

	QLabel *lblFgExeUsingDefault;
	QLabel *lblFgExePath;

	QLabel *lblFgRootUsingDefault;
	QLabel *lblFgRootPath;

	QLabel *lblUsingTerraSync;
	QLabel *lblTerraSyncPath;

	QLabel *lblImportAirports;
	QLabel *lblImportAicraft;

	void initializePage();
	bool validatePage();
signals:

public slots:

};

#endif // FINISHPAGE_H
