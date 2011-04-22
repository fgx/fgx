#ifndef AEROTOOLS_H
#define AEROTOOLS_H

#include <QObject>
#include <QtCore/QStringList>

#include "xobjects/mainobject.h"

class AeroTools : public QObject
{
Q_OBJECT
public:
	explicit AeroTools(QObject *parent, MainObject *mOb);

	MainObject *mainObject;

signals:

public slots:

	void scan_xml_sets();

};

#endif // AEROTOOLS_H
