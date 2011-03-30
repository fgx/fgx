#ifndef MPTELNET_H
#define MPTELNET_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QByteArray>

class MpTelnet : public QObject
{
Q_OBJECT
public:
    explicit MpTelnet(QObject *parent = 0);

    void get_info(QString ip_address);
    QString telnet_address;
    QString telnet_reply;

private:
    QTcpSocket *socket;

signals:
    void telnet_data(QString telnet_address, QString reply); //QString ip_address); //, QString telnet_reply);

public slots:
    void on_telnet_connected();
    void on_telnet_ready_read();
    void on_telnet_disconnected();
    //void foo_telnet_data(QString ip_address, QString reply); //QString &ip_address);
};

#endif // MPTELNET_H
