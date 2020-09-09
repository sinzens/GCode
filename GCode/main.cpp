#include "MainWindow.h"

#include <QApplication>
//#include <QList>
//#include <QTime>
//#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    /*QList<QString> list;

    QTime time1 = QTime::currentTime();

    for(int i = 0; i < 2000000; i++)
    {
        list.append("TestString");
    }

    QTime time2 = QTime::currentTime();

    qDebug() << time1.msecsTo(time2) << "ms";*/

    return app.exec();
}
