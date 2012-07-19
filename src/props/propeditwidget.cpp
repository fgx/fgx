// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// FGx FlightGear Launcher // propeditwidget.cpp
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-
// (c) 2010-2012
// Yves Sablonier, Pete Morgan
// Geoff McLane
// GNU GPLv2, see main.cpp and shipped licence.txt for further information

#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTreeWidgetItem>

#include "propeditwidget.h"


PropEditWidget::PropEditWidget(MainObject *mOb, QDialog *parent):
	QDialog(parent)
{

	mainObject = mOb;
	
	mainObject->settings->restoreWindow(this);

	setWindowTitle("Edit Property");
	setWindowIcon(QIcon(":/icons/node_val"));


	QGridLayout *grid = new QGridLayout();
	setLayout(grid);
	int row = 0;

	//* Top Label
	labelNode = new QLabel(this);
	labelNode->setStyleSheet("font-weight: bold; padding: 5px; background-color: white;");
	grid->addWidget(labelNode, row, 0, 1, 2);

	//* Value Edit
	row++;
	txtValue = new QLineEdit(this);
	grid->addWidget(txtValue, row, 0, 1, 1);

	//* Type
	labelType = new QLabel(this);
	grid->addWidget(labelType, row, 1, 1, 1);


	//* Bottom button Box
	row++;
	QHBoxLayout *buttonBox = new QHBoxLayout();
	grid->addLayout(buttonBox, row, 0, 1, 2);
	buttonBox->addStretch(10);

	//* Cancel Button
	QPushButton *buttonCancel = new QPushButton();
	buttonCancel->setIcon(QIcon(":/icons/black"));
	buttonCancel->setText("Cancel");
	buttonBox->addWidget(buttonCancel);
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));

	//* Save Button
	QPushButton *buttonSave = new QPushButton();
	buttonSave->setIcon(QIcon(":/icons/save"));
	buttonSave->setText("Set");
	buttonBox->addWidget(buttonSave);
	connect(buttonSave, SIGNAL(clicked()), this, SLOT(on_set_property()));
}




void PropEditWidget::set_from_item(QTreeWidgetItem *item){

	labelType->setText( item->text(2) );
	txtValue->setText( item->text(1) );
	labelNode->setText( item->text(3));

}



void PropEditWidget::on_set_property(){

	//mainObject->telnet->set_property(labelNode->text(), txtValue->text());
	accept();
}
