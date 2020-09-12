#ifndef ABOUTUSWINDOW_H
#define ABOUTUSWINDOW_H

#include <QMainWindow>

namespace Ui {
class AboutUsWindow;
}

class MainWindow;

class AboutUsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AboutUsWindow(QWidget *parent = nullptr);
    ~AboutUsWindow() Q_DECL_OVERRIDE;
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
    Ui::AboutUsWindow *ui;

    int styleTheme;

    MainWindow* parent;

    bool mouseDrag;
    QPoint mousePos;
};

#endif // ABOUTUSWINDOW_H
