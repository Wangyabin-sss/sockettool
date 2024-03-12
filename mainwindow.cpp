#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

QString comboxtcps = "TCP Server";
QString comboxtcpc = "TCP Client";
QString comboxudp = "UDP";



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tooltype->addItem(comboxtcps);
    ui->tooltype->addItem(comboxtcpc);
    ui->tooltype->addItem(comboxudp);
    ui->startbutton->setStyleSheet("background-color: rgb(0, 255, 0);");
    ui->startbutton->setText("start");
    ui->lineEdit->setText("127.0.0.1");
    ui->lineEdit_2->setText("8099");

    QMessageBox::warning(this,"info","如果开启了VPN\n请先关闭!!!");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::tcps_new_connect(QString ip,int port)
{
    ui->tooltypemsg->addItem(ip+":"+QString::number(port));
}
void MainWindow::tcps_recv_data(QString ip,int port,QString data)
{
    QString client = ip+":"+QString::number(port);
    if(client == ui->tooltypemsg->currentText())
    {
        ui->textEdit->append(data);
    }
}
void MainWindow::tcps_dis_connect(QString ip,int port)
{
    QString client = ip+":"+QString::number(port);
    int index = ui->tooltypemsg->findText(client);
    ui->tooltypemsg->removeItem(index);
}

void MainWindow::tcpc_recv_data(QString data)
{
    ui->textEdit->append(data);
    if(ui->tooltypemsg->currentText()=="自动响应")
    {
        this->on_sendbutton_clicked();
    }
}
void MainWindow::tcpc_dis_connect()
{
    this->on_startbutton_clicked();
}


void MainWindow::put_text_msglog(std::string data)
{
    for(size_t i=0;i<senddatas.size();i++)
    {
        if(data == senddatas[i])
            return;
    }
    senddatas.push_back(data);
    ui->msglog->addItem(QString(data.c_str()));
}


//发送按钮
void MainWindow::on_sendbutton_clicked()
{
    QString type = ui->tooltype->currentText();
    if(type==comboxtcps)
    {
        QString client = ui->tooltypemsg->currentText();
        QString ip = client.left(client.indexOf(":"));
        int port = client.mid(client.indexOf(":")+1,client.length()).toInt();
        //qDebug()<<client<<"  "<<ip<<"  "<<port<<"  "<<client.indexOf(":")<<"  "<<client.length();
        tcps.send_data(ip,port,ui->textEdit_2->toPlainText());
        put_text_msglog(ui->textEdit_2->toPlainText().toStdString());
    }
    else if(type==comboxtcpc)
    {
        tcpc.tcpclient_send(ui->textEdit_2->toPlainText());
        put_text_msglog(ui->textEdit_2->toPlainText().toStdString());
    }
    else if(type==comboxudp)
    {

    }

}


bool isValidIP(const QString &ip) {
    QHostAddress address(ip);
    return address.protocol() == QAbstractSocket::IPv4Protocol || address.protocol() == QAbstractSocket::IPv6Protocol;
}


//开始停止按钮
void MainWindow::on_startbutton_clicked()
{
    QString type = ui->tooltype->currentText();
    if(ui->startbutton->text() == "start")
    {
        ui->startbutton->setStyleSheet("background-color: rgb(255, 0, 0);");
        ui->startbutton->setText("stop");
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
                    bool ret = tcpc.tcpclient_connect(ui->lineEdit->text(),port);
                    connect(&tcpc,&TcpClient::TcpClient_recv_data,this,&MainWindow::tcpc_recv_data);
                    connect(&tcpc,&TcpClient::TcpClient_dis_connect,this,&MainWindow::tcpc_dis_connect);
                    if(!ret)
                    {
                        QMessageBox::warning(this,"error","connect server timeout");
                        this->on_startbutton_clicked();
                    }
                }
                else
                    QMessageBox::warning(this,"error","port error");
            }
            else
                QMessageBox::warning(this,"error","ip error");
        }
        else if(type==comboxudp)
        {

        }

    }
    else
    {
        if(type==comboxtcps)
        {
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
        else if(type==comboxudp)
        {

        }

        ui->startbutton->setStyleSheet("background-color: rgb(0, 255, 0);");
        ui->startbutton->setText("start");
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
    ui->tooltypemsg->clear();
    if(type==comboxtcps)
    {
        ui->tooltypemsg->setDisabled(false);
    }
    else if(type==comboxtcpc)
    {
        ui->tooltypemsg->setDisabled(false);
        ui->tooltypemsg->addItem("手动发送");
        ui->tooltypemsg->addItem("自动响应");
    }
    else if(type==comboxudp)
    {
        ui->tooltypemsg->setDisabled(false);
    }

}




void MainWindow::on_tooltypemsg_currentIndexChanged(int index)
{

}

