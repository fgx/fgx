
#include <QList>
#include <QAbstractButton>
#include <QVariant>

#include "helpers.h"

Helpers::Helpers()
{
}


void Helpers::select_radio(QButtonGroup *buttonGroup, QString value){
	QList<QAbstractButton *> buttons = buttonGroup->buttons();
	for(int idx = 0; idx < buttons.size(); idx++){
		if(buttons.at(idx)->property("value").toString() == value){
			buttons.at(idx)->setChecked(true);
			return;
		}
	}
	// select first anyway
	buttons.at(0)->setChecked(true);
}


void Helpers::select_combo(QComboBox *combo, QString value){
	int idx = combo->findData( value );
	combo->setCurrentIndex( idx == -1 ? 0 : idx );
}
