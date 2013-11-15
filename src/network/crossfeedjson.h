#ifndef CROSSFEEDJSON_H
#define CROSSFEEDJSON_H

#include <QObject>

#include <QTimer>

class CrossFeedJson : public QObject
{
    Q_OBJECT
public:
    explicit CrossFeedJson(QObject *parent = 0);

    QTimer *timmy;


signals:

public slots:

};

#endif // CROSSFEEDJSON_H
