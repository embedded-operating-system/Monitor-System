#ifndef MONITORCLIENT_H
#define MONITORCLIENT_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <map>
#include <iostream>
#include <QMessageBox>

using namespace std;
namespace Ui {
class MonitorClient;
}

class MonitorClient : public QWidget
{
    Q_OBJECT
    
public:
    explicit MonitorClient(QWidget *parent = 0);
    ~MonitorClient();

    QTcpSocket *pTcpSocketClient;           //socket(client)
    QString name;                           //username

    bool getPic;                            //recieve picture(true), other request
    void SendMesg(QString strMesg);         //send request to server
private:
    Ui::MonitorClient *ui;


private slots:
    //connect to server
    void slotConnectServer();

    //get response from server & deal response
    void slotReadMesg();

    //log in, send login request
    void slotLogin();

    //regist, send regist request
    void slotRegist();

    //return, return to the last layer
    void slotReturn();

    //screen shot, send screen shot request
    void slotShot();

    //stop/continue, send stop/ contimue request
    void slotStop();
};
#endif // MONITORCLIENT_H



