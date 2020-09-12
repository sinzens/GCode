#ifndef ABOUTTHISWINDOW_H
#define ABOUTTHISWINDOW_H

#include <QMainWindow>

namespace Ui {
class AboutThisWindow;
}

class MainWindow;

class AboutThisWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AboutThisWindow(QWidget *parent = nullptr);
    ~AboutThisWindow() Q_DECL_OVERRIDE;
    inline void initTheme();
    inline void initTitleBar();
    inline void initWindowStatus();
    inline void initMouseStatus();
    inline void initEventFilter();

protected:
    bool eventFilter(QObject* object, QEvent* event) Q_DECL_OVERRIDE;

protected slots:
    void switchStatus();

private:
    Ui::AboutThisWindow *ui;
    int styleTheme;

    MainWindow* parent;

    bool mouseDrag;
    QPoint mousePos;
};

#endif // ABOUTTHISWINDOW_H
