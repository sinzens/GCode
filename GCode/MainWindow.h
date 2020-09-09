#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() Q_DECL_OVERRIDE;
    inline void initDockWidget();

protected:
    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
