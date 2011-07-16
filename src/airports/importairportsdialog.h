#ifndef IMPORTAIRPORTSWIDGET_H
#define IMPORTAIRPORTSWIDGET_H


#include <QtGui/QDialog>
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QProgressDialog>

#include "xobjects/xsettings.h"


class ImportAirportsDialog : public QDialog
{
Q_OBJECT
public:
	explicit ImportAirportsDialog(QWidget *parent = 0);


    QRadioButton *buttAllAiports;
    QRadioButton *buttIcaoOnly;
    QCheckBox *chkImportSeaports;
    QCheckBox *chkImportHeliports;
    QCheckBox *chkBoxAdditionalAirports;

    QPushButton *buttImport;
    QPushButton *buttCancelImport;

	//QProgressDialog *progress;



signals:

public slots:

};

#endif // IMPORTAIRPORTSWIDGET_H
