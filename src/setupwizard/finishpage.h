#ifndef TERRASYNCPAGE_H
#define TERRASYNCPAGE_H

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


signals:

public slots:

};

#endif // TERRASYNCPAGE_H
