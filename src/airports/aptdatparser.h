#ifndef APTDATPARSER_H
#define APTDATPARSER_H

#include <QtCore/QObject>
#include <QtCore/QString>


class AptDatParser : public QObject
{
Q_OBJECT
public:

	explicit AptDatParser(QObject *parent = 0);

    int estimated_lines;
    int line_counter;
    //void set_tarball(QString file_path);
	void import_aptdat(QString tarball_fullpath, QWidget *parentWidget);

signals:

public slots:
};

#endif // APTDATPARSER_H
