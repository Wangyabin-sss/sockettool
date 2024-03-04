#ifndef TCPCLIENT_H
#define TCPCLIENT_H


#include <QTcpSocket>


class TcpClient : public QObject
{
    Q_OBJECT
public:
    TcpClient();

    void tcpclient_connect(QString ip,int port);
    void tcpclient_close();
    int tcpclient_send(QString data);
    void tcpclient_recv();
    void tcpclient_disconnect();

signals:
    void TcpClient_recv_data(QString);//data
    void TcpClient_dis_connect();

private:

    QTcpSocket socket;

};

#endif // TCPCLIENT_H
