#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QMessageBox>
#include <tcpserver.h>
#include <tcpclient.h>
#include <json.hpp>

using njson = nlohmann::json;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_msglog_currentIndexChanged(int index);

    void tcps_new_connect(QString ip,int port);
    void tcps_recv_data(QString ip,int port,QString data);
    void tcps_dis_connect(QString ip,int port);

    void on_tooltype_currentIndexChanged(int index);
    void put_text_msglog(std::string data);

    void tcpc_recv_data(QString data);
    void tcpc_dis_connect();

    void on_startbutton_clicked();

    void on_sendbutton_clicked();

    void on_tooltypemsg_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    TcpServer tcps;
    TcpClient tcpc;
    std::vector<std::string> senddatas;

};
#endif // MAINWINDOW_H
