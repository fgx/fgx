#ifndef COMPLETEDPAGE_H
#define COMPLETEDPAGE_H

#include <QWizardPage>


#include "xobjects/mainobject.h"


class CompletedPage : public QWizardPage
{
Q_OBJECT
public:
	explicit CompletedPage(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;
signals:

public slots:

};

#endif // COMPLETEDPAGE_H
