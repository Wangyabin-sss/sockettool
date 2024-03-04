#include "tcpserver.h"

TcpServer::TcpServer()
{

}

int TcpServer::tcpserver_init(QString ip,int port)
{
    tcpsocket = NULL;
    bool ret = tcpserver.listen(QHostAddress(ip), port);
    connect(&tcpserver, &QTcpServer::newConnection, this, &TcpServer::new_connect);
}

void TcpServer::new_connect()
{
    tcpsocket = tcpserver.nextPendingConnection();

    TcpSockets_List.append(tcpsocket);//把新进来的客户端存入列表
    QString ip=tcpsocket->peerAddress().toString();//获取连接的 ip地址
    int port=tcpsocket->peerPort();//获取连接的 端口号
    emit TcpServer_new_connect(ip,port);

    // 连接QTcpSocket的信号槽，以读取新数据
    connect(tcpsocket, &QTcpSocket::readyRead, this, &TcpServer::recv_data);
    connect(tcpsocket, &QTcpSocket::disconnected, this, &TcpServer::dis_connect);
}


void TcpServer::recv_data()
{
    for(int i = 0; i < TcpSockets_List.count(); i++)
    {
        tcpsocket = TcpSockets_List.at(i);

        QByteArray buffer = tcpsocket->readAll();
        if(buffer.length()==0)
            continue;
        QString ip=tcpsocket->peerAddress().toString();//获取连接的 ip地址
        int port=tcpsocket->peerPort();//获取连接的 端口号
        emit TcpServer_recv_data(ip,port,QString(buffer));
    }
}


int TcpServer::send_data(QString ip,int port,QString data)
{
    for(int i = 0; i < TcpSockets_List.count(); i++)
    {
        tcpsocket = TcpSockets_List.at(i);
        if(tcpsocket->peerAddress().toString() == ip)
        {
            if(tcpsocket->peerPort()==port)
            {
                QByteArray byteArray = data.toUtf8();
                qint64 ret = tcpsocket->write(byteArray);
                return ret;
            }
        }
    }
    return -1;
}


void TcpServer::dis_connect()
{

    for(int i=0; i < TcpSockets_List.count(); i++)
    {
        tcpsocket = TcpSockets_List.at(i);
        if(tcpsocket->state()==QAbstractSocket::UnconnectedState)
        {
            TcpSockets_List.removeAt(i);
            tcpsocket->close();
            emit TcpServer_dis_connect(tcpsocket->peerAddress().toString(),tcpsocket->peerPort());
        }
    }
}

int TcpServer::tcpserver_deinit()
{
    for(int i=0; i < TcpSockets_List.count(); i++)
    {
        tcpsocket = TcpSockets_List.at(i);
        tcpsocket->close();
    }
    tcpsocket=NULL;
    disconnect(&tcpserver, &QTcpServer::newConnection, this, &TcpServer::new_connect);
    tcpserver.close();
    return 0;
}
