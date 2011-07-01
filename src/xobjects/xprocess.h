#ifndef XPROCESS_H
#define XPROCESS_H

#include <QObject>
#include <QProcess>

#include "xobjects/mainobject.h"

class XProcess : public QObject
{
Q_OBJECT
public:
	explicit XProcess(MainObject *mainObject, QObject *parent = 0);

	MainObject *mainObject;
	QProcess *process;

signals:

public slots:

};

#endif // XPROCESS_H
