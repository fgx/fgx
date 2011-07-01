#ifndef HELPERS_H
#define HELPERS_H

#include <QButtonGroup>
#include <QString>
#include <QComboBox>

class Helpers
{
public:
    Helpers();

	static void select_radio(QButtonGroup *button_group, QString value);

	static void select_combo(QComboBox *combo, QString value);
};

#endif // HELPERS_H
