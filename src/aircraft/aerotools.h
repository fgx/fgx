#ifndef AEROTOOLS_H
#define AEROTOOLS_H

#include <QObject>
#include <QtCore/QStringList>

#include "xobjects/xsettings.h"

class AeroTools : public QObject
{
Q_OBJECT
public:
    explicit AeroTools(QObject *parent = 0);

	XSettings settings;

signals:

public slots:

	QStringList scan_xml_sets();

};

#endif // AEROTOOLS_H
