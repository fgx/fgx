#ifndef SETUPWIZARD_H
#define SETUPWIZARD_H

#include <QWizard>
#include <QWizardPage>

#include "setupwizard/fgexepage.h"
#include "setupwizard/fgrootpage.h"
#include "setupwizard/terrasyncpage.h"
#include "setupwizard/otherpage.h"
#include "setupwizard/confirmpage.h"
#include "setupwizard/completedpage.h"

#include "xobjects/mainobject.h"

class SetupWizard : public QWizard
{
Q_OBJECT
public:
	explicit SetupWizard(MainObject *mObject, QWidget *parent = 0);

	MainObject *mainObject;
	QWizardPage *fgExePage;
	QWizardPage *fgRootPage;
	QWizardPage *terraSyncPage;
	QWizardPage *otherPage;
	QWizardPage *confirmPage;
	QWizardPage *completedPage;



signals:

public slots:

};

#endif // SETUPWIZARD_H
