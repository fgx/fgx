#include "setupwizard.h"


SetupWizard::SetupWizard(MainObject *mOb, QWidget *parent) :
    QWizard(parent)
{
	mainObject = mOb;
	setWindowTitle(tr("Setup Wizard"));
	setWindowIcon(QIcon(":icon/wizard"));

	fgExePage = new FgExePage(mainObject);
	addPage(fgExePage);

	fgRootPage = new FgRootPage(mainObject);
	addPage(fgRootPage);

	terraSyncPage = new TerraSyncPage(mainObject);
	addPage(terraSyncPage);
}




 
