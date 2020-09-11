#include "MainWindow.h"
#include "OptionWindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();

    /*OptionWindow optionWindow;
    QFile qssFile(":/dark/stylesheets/MainWindowD.qss");
    qssFile.open(QFile::ReadOnly);
    optionWindow.setStyleSheet(qssFile.readAll());
    qssFile.close();
    optionWindow.show();*/

    return app.exec();
}
