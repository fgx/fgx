

#include <QtCore/QByteArray>
#include <QtCore/QStringList>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

#include "telnetslave.h"


TelnetSlave::TelnetSlave(QObject *parent) :
    QObject(parent)
{

    current_node_path = "";
    in_request = false;


    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(on_error( QAbstractSocket::SocketError))
    );
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(on_state_changed(QAbstractSocket::SocketState))
    );
    connect(socket, SIGNAL(hostFound()), this, SLOT(on_host_found()));
    connect(socket, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(on_disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(on_ready_read()));
}

//*********************************************************************************************
 //** Connect / Disconnect
//********************************************************************************************
void TelnetSlave::telnet_connect(QString address, int portt){
	hostAddress = address;
	port = portt;
	socket->connectToHost(hostAddress, port);
	//qDebug() << "open" << hostAddress << port;
}



void TelnetSlave::telnet_disconnect(){
    socket->close();
}

void TelnetSlave::set_property(QString path, QString value){
    //TODO
	//qDebug() << path << "=" << value;
	QByteArray command("set ");
	command.append(path).append(" ").append(value).append("\r\n");
	//QByteArray cmd(command);
	qDebug() << command;
	//*** TODO #### ARGH.. this crashes with ASSERT failure in QList<T>::operator[]: "index out of range", file /usr/include/qt4/QtCore/qlist.h, line 463
	socket->write( command );
}

void TelnetSlave::add_que(QString node)
{
	//que->
}

//==========================================================================================
//== Get Node
//==========================================================================================
void TelnetSlave::get_node(QString node_path){
    if( !socket->isOpen() ){
		// TODO emit error maybe ?
        return;
    }
    current_node_path = node_path; // << store the "node_path" in the current_node_path vars
	QByteArray command = QByteArray("ls ").append(node_path).append("\r\n");
    //** Send request to telnet socket
    if(in_request){
        qDebug() << "IN_REQUEST";
        qDebug() << socket->bytesToWrite();
        return;
    }
    in_request = true;
    socket->write( command );
 }


//************************************************************
//*** READ READY

void TelnetSlave::on_ready_read(){

    QString reply(socket->readAll());
    QStringList lines = reply.split("\r\n");

	//qDebug() << reply;
    for(int i = 0; i < lines.size(); ++i){
        QString line = lines.at(i).trimmed();
       // qDebug() << "line=" << line;

		//= end line is /> so skip
        if(line == "/>"){
            qDebug("END");

        }else if( line.endsWith("/") ){
			emit props_folder(current_node_path, line);

        }else{
            if(line.count("chat") > 0){
                 qDebug() << "CHAT=" << line << "=" << line.count("chat") ;
            }
            // check the = sign is there.
            if( line.count("=") > 0 ){
                //* eg right-aileron-pos-norm =\t'0.02699490675'\t(double)
                //QString val_part = line.section("=", 0, 0).trimmed();
                //* Split the values on a tab
                QStringList val_parts = line.split("\t");
                //ssqDebug() << val_parts;

                //TODO - maybe some regex
                //** the node_name ends with " =" so remove eg "my-node ="
                QString node_name = val_parts[0].left( val_parts[0].length() - 2 );

				//== node value in enclosed in ' so remove eg 'true'
                QString node_value = val_parts.size() == 1 ?
                                     "" :
                                     val_parts[1].replace("'","");

				//== the node_type is encoded in () eg (double)
                QString node_type = val_parts.size() == 2 ?
                                    "" :
                                    val_parts[2].mid(1, val_parts[2].length() - 2);

               // QString node_name = val_parts[0].trimmed();
                //qDebug() << "VAL=" << node_name << " = " << node_value << "=" << node_type;
                //qDebug();
				emit props_value(current_node_path, node_name, node_value, node_type);
            }else{
                qDebug() << "UMM=" << line << "=" << line.count("=") ;
                 //qDebug() << reply;
            }
        }
    } /* for */
    in_request = false;
}

//*********************************************************************************************
 //** Socket Events
//*********************************************************************************************

void TelnetSlave::on_host_found(){
		qDebug("on_host_found");
}

void TelnetSlave::on_connected(){
	qDebug("on_telnet_connected");
    emit telnet_connected(true);
}

void TelnetSlave::on_disconnected(){
	qDebug("on_telnet_disconnected"); // << "DONE" << telnet_reply;
    emit telnet_connected(false);
}

void TelnetSlave::on_error(QAbstractSocket::SocketError socketError){
    qDebug("on_error");
    qDebug() << socket->errorString();
    emit telnet_error(socketError, socket->errorString());
}



void TelnetSlave::on_state_changed(QAbstractSocket::SocketState socketState ){
      //qDebug("on_state_changed");
	  qDebug() << "state=" << socketState;
	Q_UNUSED(socketState);
}
