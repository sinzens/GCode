#ifndef FRWINDOW_H
#define FRWINDOW_H

#include <QDialog>

namespace Ui {
class FRWindow;
}

class MainWindow;

class FRWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FRWindow(QWidget *parent = nullptr);
    ~FRWindow() Q_DECL_OVERRIDE;
    inline void initTheme();
    inline void initMouseStatus();
    inline void initEventFilter();

protected:
    bool eventFilter(QObject* object, QEvent* event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
signals:
    void sendfindsignal(QString Data,int flag,QString Data1);

private slots:
    void on_findnextButton_clicked();

    void on_findallButton_clicked();



    void on_replacenextButton_clicked();

    void on_replaceallButton_clicked();

private:
    Ui::FRWindow *ui;
    int styleTheme;

    MainWindow* parent;

    bool mouseDrag;
    QPoint mousePos;
};

#endif // FRWINDOW_H
