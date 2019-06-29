#include "monitorclient.h"
#include "ui_monitorclient.h"
#include <QStringList>
#include <QBuffer>
#include <QImageReader>
#include <QMovie>

MonitorClient::MonitorClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MonitorClient)
{
    ui->setupUi(this);
    //connect slot
    this->connect(ui->pushButton_ConnectServer,SIGNAL(clicked()),this,SLOT(slotConnectServer()));
    this->connect(ui->pushButton_login,SIGNAL(clicked()),this,SLOT(slotLogin()));
    this->connect(ui->pushButton_regist,SIGNAL(clicked()),this,SLOT(slotRegist()));
    this->connect(ui->pushButton_return,SIGNAL(clicked()),this,SLOT(slotReturn()));
    this->connect(ui->pushButton_shot,SIGNAL(clicked()),this,SLOT(slotShot()));
    this->connect(ui->pushButton_stop,SIGNAL(clicked()),this,SLOT(slotStop()));

    //set views
    getPic = false;
    ui->label_warning->setText("Input the server IP and port");
    ui->label_name->setText("IP:");
    ui->label_pw->setText("Port:");
    ui->lineEdit_name->setText("127.0.0.1");
    ui->lineEdit_pw->setText("19999");
    ui->splitter_log->setVisible(false);
    ui->splitter_screen->setVisible(false);
    ui->label_image->setVisible(false);
}

MonitorClient::~MonitorClient()
{
    delete ui;
}

//get response from server & deal response
void MonitorClient::slotReadMesg()
{
    //read the response
    QByteArray qba= pTcpSocketClient->readAll();
    //if it's a screen shot request
    if (getPic) {
        //convert QByteArray to Image
        QImage *image2 = new QImage;
        image2->loadFromData(qba, "PNG");
        //show the Image at the label
        ui->label_image->setPixmap(QPixmap::fromImage(*image2));
        ui->label_image->show();
        //set the getImage flag to false
        getPic = false;
        return;
    }
    //convert the request to the QString to handle
    QString ss=QVariant(qba).toString();
    QStringList resp = ss.split("-");
    if (resp[0] == "login") {
        //login response
        if (resp[1] == "success") {
            ui->label_warning->setText("Login success");
            name = resp[2];
            ui->splitter_user->setVisible(false);
            ui->splitter_screen->setVisible(true);
            ui->label_image->setVisible(true);
        } else{
            ui->label_warning->setText("Login fail");
            ui->splitter_name->setVisible(false);
            ui->splitter_pw->setVisible(false);
            ui->splitter_log->setVisible(false);
        }
    } else if (resp[0] == "regist"){
        //regist response
        if (resp[1] == "success") {
            ui->label_warning->setText("Regist success");
            name = resp[2];
            ui->splitter_user->setVisible(false);
            ui->splitter_screen->setVisible(true);
            ui->label_image->setVisible(true);
        } else{
            ui->label_warning->setText("Regist fail");
            ui->splitter_name->setVisible(false);
            ui->splitter_pw->setVisible(false);
            ui->splitter_log->setVisible(false);
        }
    }
}

////connect to server
void MonitorClient::slotConnectServer()
{
    //get IP and Port
    if(ui->lineEdit_name->text().isEmpty() ||ui->lineEdit_pw->text().isEmpty())
    {
        //Warning
        QMessageBox::warning(this,tr("Warning"),tr("Please input IP OR NetPort!"));
        return;
    }
    //abort and reconnect
    pTcpSocketClient = new QTcpSocket(this);
    pTcpSocketClient->abort();
    QString qStrIP = ui->lineEdit_name->text();
    quint16 netport = ui->lineEdit_pw->text().toInt();
    pTcpSocketClient->connectToHost(qStrIP,netport);

    //bind the slot and readyRead
    connect(pTcpSocketClient,SIGNAL(readyRead()),this,SLOT(slotReadMesg()));

    //set visible
    ui->label_warning->clear();
    ui->lineEdit_name->clear();
    ui->lineEdit_pw->clear();
    ui->label_name->setText("UserName:");
    ui->label_pw->setText("Password:");
    ui->lineEdit_pw->setEchoMode(QLineEdit::Password);
    ui->pushButton_ConnectServer->setVisible(false);
    ui->splitter_log->setVisible(true);
}


//send request to server
void MonitorClient::SendMesg(QString strMesg) //发送消息
{
    pTcpSocketClient->write(strMesg.toStdString().c_str(),strlen(strMesg.toStdString().c_str()));
}

//log in, send login request
void MonitorClient::slotLogin()
{
    QString command = "login";
    QString name = ui->lineEdit_name->text();
    QString pw = ui->lineEdit_pw->text();
    QString msg = command + "-" + name + "-" + pw + "-end";
    this->SendMesg(msg);
}

//regist, send regist request
void MonitorClient::slotRegist()
{
    QString command = "regist";
    QString name = ui->lineEdit_name->text();
    QString pw = ui->lineEdit_pw->text();
    QString msg = command + "-" + name + "-" + pw + "-end";
    this->SendMesg(msg);
}

//return, return to the last layer
void MonitorClient::slotReturn()
{
    ui->splitter_name->setVisible(true);
    ui->splitter_pw->setVisible(true);
    ui->splitter_log->setVisible(true);
}

//screen shot, send screen shot request
void MonitorClient::slotShot()
{
    //need to get Image from server
    this->getPic = true;
    //set views
    QString command = "shot";
    QString name = ui->lineEdit_name->text();
    QString file = ui->lineEdit_test->text();
    QString msg = command + "-" + name + "-" + file + "-end";
    this->SendMesg(msg);
    ui->label_warning->setText("Screen Shot");
}

//stop/continue, send stop/ contimue request
void MonitorClient::slotStop() {
    if (ui->pushButton_stop->text() == "Stop") {
        //set pushButton
        ui->pushButton_stop->setText("Continue");
        QString command = "stop";
        QString name = ui->lineEdit_name->text();
        QString msg = command + "-" + name + "-end-end";
        this->SendMesg(msg);
    } else {
        //set pushButton
        ui->pushButton_stop->setText("Stop");
        QString command = "continue";
        QString name = ui->lineEdit_name->text();
        QString msg = command + "-" + name + "-end-end";
        this->SendMesg(msg);
    }
}
