#ifndef FGROOTPAGE_H
#define FGROOTPAGE_H

#include <QRadioButton>
#include <QWizardPage>
#include <QToolButton>
#include <QLabel>
#include <QLineEdit>

#include "xobjects/mainobject.h"

class FgRootPage : public QWizardPage
{
Q_OBJECT
public:
	explicit FgRootPage(MainObject *mainObject, QWidget *parent = 0);
	MainObject *mainObject;


	QRadioButton *radioDefault;
	QRadioButton *radioCustom;
	QToolButton *buttSelect;

	QLabel *lblDefault;
	QLabel *lblCustom;
	QLineEdit *txtFgRoot;

	bool validatePage();
	void initializePage();

signals:

public slots:
	void on_default_toggled(bool);
	void on_select_path();
	void check_paths();

};

#endif // FGROOTPAGE_H
