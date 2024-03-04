#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

QString comboxtcps = "TCP Server";
QString comboxtcpc = "TCP Client";
QString comboxudps = "UDP Server";
QString comboxudpc = "UDP Client";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tooltype->addItem(comboxtcps);
    ui->tooltype->addItem(comboxtcpc);
    ui->tooltype->addItem(comboxudps);
    ui->tooltype->addItem(comboxudpc);
    ui->pushButton->setStyleSheet("background-color: rgb(0, 255, 0);");
    ui->pushButton->setText("start");
    ui->lineEdit->setText("127.0.0.1");
    ui->lineEdit_2->setText("8099");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::tcps_new_connect(QString ip,int port)
{
    ui->tcpclient->addItem(ip+":"+QString::number(port));
}
void MainWindow::tcps_recv_data(QString ip,int port,QString data)
{
    QString client = ip+":"+QString::number(port);
    if(client == ui->tcpclient->currentText())
    {
        ui->textEdit->append(data);
    }
}
void MainWindow::tcps_dis_connect(QString ip,int port)
{
    QString client = ip+":"+QString::number(port);
    int index = ui->tcpclient->findText(client);
    ui->tcpclient->removeItem(index);
}

void MainWindow::tcpc_recv_data(QString data)
{
    ui->textEdit->append(data);
}
void MainWindow::tcpc_dis_connect()
{
    this->on_pushButton_clicked();
}

//发送按钮
void MainWindow::on_pushButton_2_clicked()
{
    QString type = ui->tooltype->currentText();
    if(type==comboxtcps)
    {
        QString client = ui->tcpclient->currentText();
        QString ip = client.left(client.indexOf(":"));
        int port = client.mid(client.indexOf(":")+1,client.length()).toInt();
        //qDebug()<<client<<"  "<<ip<<"  "<<port<<"  "<<client.indexOf(":")<<"  "<<client.length();
        tcps.send_data(ip,port,ui->textEdit_2->toPlainText());
    }
    else if(type==comboxtcpc)
    {
        tcpc.tcpclient_send(ui->textEdit_2->toPlainText());
    }
    else if(type==comboxudps)
    {

    }
    else if(type==comboxudpc)
    {

    }
}


bool isValidIP(const QString &ip) {
    QHostAddress address(ip);
    return address.protocol() == QAbstractSocket::IPv4Protocol || address.protocol() == QAbstractSocket::IPv6Protocol;
}


//开始停止按钮
void MainWindow::on_pushButton_clicked()
{
    QString type = ui->tooltype->currentText();
    if(ui->pushButton->text() == "start")
    {
        ui->pushButton->setStyleSheet("background-color: rgb(255, 0, 0);");
        ui->pushButton->setText("stop");
        ui->tooltype->setDisabled(true);
        if(type==comboxtcps)
        {
            if(isValidIP(ui->lineEdit->text()))
            {
                int port = ui->lineEdit_2->text().toInt();
                if(port<65535&&port>0)
                {
                    tcps.tcpserver_init(ui->lineEdit->text(),port);
                    connect(&tcps,&TcpServer::TcpServer_new_connect,this,&MainWindow::tcps_new_connect);
                    connect(&tcps,&TcpServer::TcpServer_recv_data,this,&MainWindow::tcps_recv_data);
                    connect(&tcps,&TcpServer::TcpServer_dis_connect,this,&MainWindow::tcps_dis_connect);
                }
                else
                    QMessageBox::warning(this,"error","port error");
            }
            else
                QMessageBox::warning(this,"error","ip error");
        }
        else if(type==comboxtcpc)
        {
            if(isValidIP(ui->lineEdit->text()))
            {
                int port = ui->lineEdit_2->text().toInt();
                if(port<65535&&port>0)
                {
                    tcpc.tcpclient_connect(ui->lineEdit->text(),port);
                    connect(&tcpc,&TcpClient::TcpClient_recv_data,this,&MainWindow::tcpc_recv_data);
                    connect(&tcpc,&TcpClient::TcpClient_dis_connect,this,&MainWindow::tcpc_dis_connect);
                }
                else
                    QMessageBox::warning(this,"error","port error");
            }
            else
                QMessageBox::warning(this,"error","ip error");
        }
        else if(type==comboxudps)
        {

        }
        else if(type==comboxudpc)
        {

        }
    }
    else
    {
        if(type==comboxtcps)
        {
            ui->tcpclient->clear();
            disconnect(&tcps,&TcpServer::TcpServer_new_connect,this,&MainWindow::tcps_new_connect);
            disconnect(&tcps,&TcpServer::TcpServer_recv_data,this,&MainWindow::tcps_recv_data);
            disconnect(&tcps,&TcpServer::TcpServer_dis_connect,this,&MainWindow::tcps_dis_connect);
            tcps.tcpserver_deinit();
        }
        else if(type==comboxtcpc)
        {
            disconnect(&tcpc,&TcpClient::TcpClient_recv_data,this,&MainWindow::tcpc_recv_data);
            disconnect(&tcpc,&TcpClient::TcpClient_dis_connect,this,&MainWindow::tcpc_dis_connect);
            tcpc.tcpclient_close();
        }
        else if(type==comboxudps)
        {

        }
        else if(type==comboxudpc)
        {

        }
        ui->pushButton->setStyleSheet("background-color: rgb(0, 255, 0);");
        ui->pushButton->setText("start");
        ui->tooltype->setDisabled(false);
    }
}


void MainWindow::on_msglog_currentIndexChanged(int index)
{
    ui->textEdit_2->setText(ui->msglog->currentText());
}


void MainWindow::on_tooltype_currentIndexChanged(int index)
{
    QString type = ui->tooltype->currentText();
    if(type==comboxtcps)
    {
        ui->tcpclient->setDisabled(false);
    }
    else if(type==comboxtcpc)
    {
        ui->tcpclient->setDisabled(true);
    }
    else if(type==comboxudps)
    {
        ui->tcpclient->setDisabled(false);
    }
    else if(type==comboxudpc)
    {
        ui->tcpclient->setDisabled(true);
    }
}

