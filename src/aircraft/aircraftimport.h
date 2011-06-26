#ifndef AEROTOOLS_H
#define AEROTOOLS_H

#include <QObject>
#include <QtCore/QStringList>

#include "xobjects/mainobject.h"

class AircraftImport : public QObject
{
Q_OBJECT
public:
	explicit AircraftImport(QObject *parent, MainObject *mOb);

	MainObject *mainObject;
        QList<QStringList> rows;

signals:

public slots:
	
	//void create_db_tables();
	void scan_xml_sets();
	//void execute_sql_commands_list(QStringList sql_commands);

};

#endif // AEROTOOLS_H
