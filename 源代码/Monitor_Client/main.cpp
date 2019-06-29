#include <QApplication>
#include "monitorclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MonitorClient w;
    w.show();
    
    return a.exec();
}
