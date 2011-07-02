
#include <QtCore/QIODevice>

#include "mptelnet.h"




MpTelnet::MpTelnet(QObject *parent) :
    QObject(parent)
{
    telnet_reply =  QString("");
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(on_telnet_connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(on_telnet_disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(on_telnet_ready_read()));
}

void MpTelnet::get_info(QString ip_address){
    telnet_address = QString(ip_address);
    socket->connectToHost(telnet_address, 5001, QIODevice::ReadOnly);
}


void MpTelnet::on_telnet_connected(){
   // qDebug("connected");
}

void MpTelnet::on_telnet_ready_read(){

    telnet_reply.append( QString(socket->readAll()) );
}

void MpTelnet::on_telnet_disconnected(){
    emit telnet_data(telnet_address, telnet_reply); //, telnet_reply);
}



