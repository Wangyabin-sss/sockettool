#ifndef TCPSERVER_H
#define TCPSERVER_H


#include <QTcpSocket>
#include <QTcpServer>
#include <Qlist>
#include <QDebug>
#include <QString>
#include <QObject>


class TcpServer : public QObject
{
    Q_OBJECT
public:
    TcpServer();

    int tcpserver_init(QString ip,int port);
    int tcpserver_deinit();

    void new_connect();
    void recv_data();
    int send_data(QString ip,int port,QString data);
    void dis_connect();

signals:
    void TcpServer_new_connect(QString,int); //ip  port
    void TcpServer_recv_data(QString,int,QString);//ip  data
    void TcpServer_dis_connect(QString,int);//ip  port

private:
    QTcpSocket *tcpsocket;
    QTcpServer tcpserver;
    QList<QTcpSocket*> TcpSockets_List;
};

#endif // TCPSERVER_H
