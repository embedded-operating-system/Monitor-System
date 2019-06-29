#ifndef MONITORSERVER_H
#define MONITORSERVER_H

#include <QWidget>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <map>
#include <iostream>
#include <QMessageBox>
#include <QSqlDatabase>
using namespace std;

class QCamera;
class QCameraViewfinder;
class QCameraImageCapture;

namespace Ui {
class MonitorServer;
}

class MonitorServer : public QWidget
{
    Q_OBJECT
    
public:
    explicit MonitorServer(QWidget *parent = 0);
    ~MonitorServer();
    QTcpServer *m_pTcpServer;

    QTcpSocket *pTcpSocketServer;           //Server

    QSqlDatabase db;                        //user account information database
    QString user;                           //current user

    //user regist
    void Regist(QString name, QString pw);

    //comfirm user exsist or not
    bool ComfirmUser(QString name, QString pw);
    QMovie *movie;
    
private:
    Ui::MonitorServer *ui;

    //send response to client
    void SendMesg(QString strMesg);

    QCamera*             m_pCamera;         //load QCamera
    QCameraViewfinder*   m_pViewfinder;     //randered QCamera
    QCameraImageCapture* m_pImageCapture;   //get QCamera current frame

private slots:
    //new client connect
    void slotNewConnect();

    //get request from client and deal with response
    void slotReadMesg();

    //test pushButton
    void slotTest();

    //capture current Image
    void cameraImageCaptured(int,QImage);
};

#endif // MONITORSERVER_H
