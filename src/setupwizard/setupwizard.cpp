#include "setupwizard.h"


SetupWizard::SetupWizard(MainObject *mOb, QWidget *parent) :
    QWizard(parent)
{
	mainObject = mOb;
	setWindowTitle(tr("Setup Wizard"));

	fgExePage = new FgExePage(mainObject);
	addPage(fgExePage);

	fgRootPage = new FgRootPage(mainObject);
	addPage(fgRootPage);
}




 
