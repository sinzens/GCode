#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QCloseEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initDockWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initDockWidget()
{
    ui->centralwidget->setVisible(false);
    splitDockWidget(ui->FileListDock, ui->ConsoleDock, Qt::Vertical);
    splitDockWidget(ui->FileListDock, ui->EditorDock, Qt::Horizontal);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QMessageBox::StandardButton result = QMessageBox::question(this, "提示", "即将退出\n\n确定吗");
    result == QMessageBox::Yes ? event->accept() : event->ignore();
}

