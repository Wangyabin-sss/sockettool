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
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_msglog_currentIndexChanged(int index);

    void tcps_new_connect(QString ip,int port);
    void tcps_recv_data(QString ip,int port,QString data);
    void tcps_dis_connect(QString ip,int port);

    void on_tooltype_currentIndexChanged(int index);

    void tcpc_recv_data(QString data);
    void tcpc_dis_connect();

private:
    Ui::MainWindow *ui;
    TcpServer tcps;
    TcpClient tcpc;

};
#endif // MAINWINDOW_H
