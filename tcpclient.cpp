#include "tcpclient.h"


TcpClient::TcpClient()
{

}

void TcpClient::tcpclient_connect(QString ip,int port)
{
    socket.connectToHost(ip,port);
    connect(&socket,&QTcpSocket::readyRead,this,&TcpClient::tcpclient_recv);
    connect(&socket,&QTcpSocket::disconnected,this,&TcpClient::tcpclient_disconnect);
}


void TcpClient::tcpclient_close()
{
    disconnect(&socket,&QTcpSocket::readyRead,this,&TcpClient::tcpclient_recv);
    disconnect(&socket,&QTcpSocket::disconnected,this,&TcpClient::tcpclient_disconnect);
    socket.close();
}


int TcpClient::tcpclient_send(QString data)
{
    QByteArray byteArray = data.toUtf8();
    qint64 ret = socket.write(byteArray);
    return ret;
}


void TcpClient::tcpclient_recv()
{
    QByteArray buffer = socket.readAll();
    emit TcpClient_recv_data(QString(buffer));
}


void TcpClient::tcpclient_disconnect()
{
    emit TcpClient_dis_connect();
}

