#ifndef XSETTINGSMODEL_H
#define XSETTINGSMODEL_H

#include <QStandardItemModel>

class XSettingsModel : public QStandardItemModel
{
Q_OBJECT
public:
    explicit XSettingsModel(QObject *parent = 0);


	enum COLS{
		C_ENABLED = 0,
		C_OPTION = 1,
		C_VALUE = 2,
		C_DEFAULT = 3,
		C_LEVEL = 4,
		C_DESCRIPTION = 5
	};

	void add_option(bool enabled, QString option, QString value, QString preset, int level, QString description, QString area);

	QString getx(QString option);


signals:
	void upx(bool enabled, QString option, QString value);

public slots:

	void set_option(bool enabled, QString option, QString value);
};

#endif // XSETTINGSMODEL_H
