#ifndef FGEXEPAGE_H
#define FGEXEPAGE_H

#include <QtGui/QWizardPage>
#include <QtGui/QRadioButton>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>

#include "xobjects/mainobject.h"

class FgExePage : public QWizardPage
{
Q_OBJECT
public:
	explicit FgExePage(	MainObject *mainObject, QWidget *parent = 0);
	MainObject *mainObject;

	QRadioButton *radioDefault;
	QRadioButton *radioCustom;

	QLabel *lblDefault;
	QLabel *lblCustom;
	QLineEdit *txtFgfs;

	bool validatePage();
	void initializePage();

signals:

public slots:
	void on_fgfs_autodetect();
	void on_select_fgfs_path();
	void check_paths();
};

#endif // FGEXEPAGE_H
