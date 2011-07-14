#ifndef XSETTINGSMODEL_H
#define XSETTINGSMODEL_H

#include <QStandardItemModel>

class XSettingsModel : public QStandardItemModel
{
Q_OBJECT
public:
    explicit XSettingsModel(QObject *parent = 0);

	void add_option(bool enabled, QString option, QString value, QString preset, int level, QString description, QString area);




signals:

public slots:

	void set_option(bool enabled, QString option, QString value);
};

#endif // XSETTINGSMODEL_H
