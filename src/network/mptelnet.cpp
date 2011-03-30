#include "mptelnet.h"

#include <QtCore/QIODevice>


MpTelnet::MpTelnet(QObject *parent) :
    QObject(parent)
{

    telnet_reply =  QString("");
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(on_telnet_connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(on_telnet_disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(on_telnet_ready_read()));

   // connect(this, SIGNAL(telnet_data(QString, QString)), this, SLOT(foo_telnet_data(QString, QString )));

}

void MpTelnet::get_info(QString ip_address){
    telnet_address = QString(ip_address);
    qDebug() << "GET INFO >>" << telnet_address;
    socket->connectToHost(telnet_address, 5001, QIODevice::ReadOnly);

}


void MpTelnet::on_telnet_connected(){
   // qDebug("connected");
}

void MpTelnet::on_telnet_ready_read(){
    //qDebug() << "READ < " << telnet_address;
   // qDebug() << socket->readAll();
    telnet_reply.append( QString(socket->readAll()) );
}

void MpTelnet::on_telnet_disconnected(){
    //qDebug() << "\n--------------------------------\non_telnet_disconnectedan-------------\n" ; //<< telnet_reply;
    //qDebug() << "DONE" << telnet_reply;

    //QString


    emit telnet_data(telnet_address, telnet_reply); //, telnet_reply);
}

//void MpTelnet::foo_telnet_data(QString ip_address, QString telnet_reply){
    //qDebug() << "CHK" << ip_address << telnet_reply;
//}


