#include "monitorserver.h"
#include "ui_monitorserver.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QImage>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringList>
#include <QImageWriter>
#include <QBuffer>
#include <QMovie>
#include <QFileDialog>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>

MonitorServer::MonitorServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MonitorServer)
{
    ui->setupUi(this);
    //start server
    m_pTcpServer = new QTcpServer();
    m_pTcpServer->listen(QHostAddress::Any,19999);
    this->connect(m_pTcpServer,SIGNAL(newConnection()),this,SLOT(slotNewConnect()));
    this->connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(slotTest()));

    //initial QCarema
    m_pCamera = new QCamera(this);
    m_pViewfinder = new QCameraViewfinder(this);
    m_pImageCapture = new QCameraImageCapture(m_pCamera);
    mainLayout->addWidget(m_pViewfinder);
    connect(m_pImageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(cameraImageCaptured(int,QImage)));
    m_pImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    m_pCamera->setCaptureMode(QCamera::CaptureStillImage);
    m_pCamera->setViewfinder(m_pViewfinder);
    m_pCamera->start();



    //open database
    db = QSqlDatabase::addDatabase("QSQLITE", "QSQLITE");
    db.setDatabaseName("./MyDB.db");
    bool ok = db.open();

    if (ok) {
        QSqlQuery qsQuery = QSqlQuery(db);
        QString strSqlText = QString("CREATE TABLE User ( Username       VARCHAR(256),Password    VARCHAR(256));");
        qsQuery.prepare(strSqlText);
        if(!qsQuery.exec())
        {
            QSqlError lastError = qsQuery.lastError();
            qDebug() << lastError.driverText() << QString(QObject::tr("Create failed."));
        } else {
            qDebug() << "Create table user";
        }
        qsQuery.finish();
    }
}

MonitorServer::~MonitorServer()
{
    delete ui;
}

//new client connect
void MonitorServer::slotNewConnect()
{
    pTcpSocketServer =  m_pTcpServer->nextPendingConnection();
    this->connect(pTcpSocketServer,SIGNAL(readyRead()),this,SLOT(slotReadMesg()));
}

//get request from client and deal with response
void MonitorServer::slotReadMesg()
{
    QByteArray qba= pTcpSocketServer->readAll(); //读取

    QString ss=QVariant(qba).toString();
    QStringList req = ss.split("-");
    if(req[0] == "login") {
        qDebug() << "User Log in";
        if(this->ComfirmUser(req[1], req[2])) {
            qDebug() << "Yes, ok";
            QString resp = "login-success-" + req[1] + "-end";
            this->SendMesg(resp);
            ui->label_user->setText("current user: " + req[1]);
        } else{
            QString resp = "login-fail-" + req[1] + "-end";
            this->SendMesg(resp);
        }
    } else if (req[0] == "regist") {
        qDebug() << "User Regist";
        if(this->ComfirmUser(req[1], req[2])) {
            qDebug() << "No, ok";
            QString resp = "regist-fail-" + req[1] + "-end";
            this->SendMesg(resp);
        } else{
            this->Regist(req[1], req[2]);
            ui->label_user->setText("current user: " + req[1]);
            QString resp = "regist-success-" + req[1] + "-end";
            this->SendMesg(resp);
        }
    } else if(req[0] == "shot") {
        this->m_pImageCapture->capture();
    } else if (req[0] == "stop") {
        ui->label_user->setText("current user: " + this->user + " STOP");
        this->m_pCamera->stop();
    } else {
        ui->label_user->setText("current user: " + this->user);
        this->m_pCamera->start();
    }
}

//send message
void MonitorServer::SendMesg(QString strMesg)
{
    qDebug() << strMesg;
    pTcpSocketServer->write(strMesg.toStdString().c_str(),strlen(strMesg.toStdString().c_str()));
}

//user regist
void MonitorServer::Regist(QString name, QString pw)
{
    QSqlQuery qsQuery = QSqlQuery(db);
    qsQuery.prepare("INSERT INTO User (Username, Password) "
                      "VALUES (:name, :pw)");
    qsQuery.bindValue(":name", name);
    qsQuery.bindValue(":pw", pw);
    if(!qsQuery.exec())
    {
        QSqlError lastError = qsQuery.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("Insert failed."));
    }
    qsQuery.finish();
}

//comfirm account exsists or not
bool MonitorServer::ComfirmUser(QString name, QString pw)
{
    QSqlQuery qsQuery = QSqlQuery(db);
    qsQuery.exec("select * from User WHERE Username = '" + name + "' AND Password = '" + pw + "'");
    if(!qsQuery.exec())
    {
        QSqlError lastError = qsQuery.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("Query failed."));
    }
    if (qsQuery.next()) {
        qsQuery.finish();
        return true;
    } else {
        qsQuery.finish();
        return false;
    }
}

void MonitorServer::slotTest() {
    QSqlQuery qsQuery = QSqlQuery(db);
    qsQuery.exec("select * from User");
    if(!qsQuery.exec())
    {
        QSqlError lastError = qsQuery.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("Query failed."));
    }
    while(qsQuery.next())
    {
        qDebug() << qsQuery.value(0) << qsQuery.value(1);
    }
}


//capture current Image
void MonitorServer::cameraImageCaptured(int, QImage image)
{
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QImage *image2 = new QImage;
    image2->loadFromData(ba, "PNG");
    ui->label->setPixmap(QPixmap::fromImage(*image2));
    ui->label->show();
    this->SendMesg(QString(ba));
}
