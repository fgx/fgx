#ifndef TERRASYNCPAGE_H
#define TERRASYNCPAGE_H

#include <QWizardPage>
#include <QCheckBox>
#include <QLineEdit>
#include <QToolButton>
#include <QLabel>


#include "xobjects/mainobject.h"

class TerraSyncPage : public QWizardPage
{
Q_OBJECT
public:
	explicit TerraSyncPage(MainObject *mob, QWidget *parent = 0);

	MainObject *mainObject;
	QCheckBox *checkBoxUseTerrasync;
	QToolButton *buttSelectPath;
	QLineEdit *txtTerraSyncPath;
	QLabel *lblHelp;

signals:

public slots:
	void on_checkbox_clicked();
	void on_select_path();
};

#endif // TERRASYNCPAGE_H
