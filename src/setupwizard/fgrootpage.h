#ifndef FGROOTPAGE_H
#define FGROOTPAGE_H

#include <QWizardPage>
#include "xobjects/mainobject.h"

class FgRootPage : public QWizardPage
{
Q_OBJECT
public:
	explicit FgRootPage(MainObject *mainObject, QWidget *parent = 0);
	MainObject *mainObject;
signals:

public slots:

};

#endif // FGROOTPAGE_H
