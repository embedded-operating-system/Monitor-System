#include <QApplication>
#include "monitorserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MonitorServer w;
    w.show();
    
    return a.exec();
}
