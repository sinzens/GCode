#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

typedef void doNothing;

enum UiStyle
{
    LightTheme,
    DarkTheme
};

struct StyleTheme
{
    UiStyle theme;
    QStringList stylesheetList;
};

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
    inline void initMenuBar();
    inline void initWindowStatus();
    inline void initMouseStatus();
    inline void initEventFilter();
    inline void initStylesheet();
    inline void delayToShow(int mesc);

protected:
    bool eventFilter(QObject* object, QEvent* event) Q_DECL_OVERRIDE;
    bool nativeEvent(const QByteArray &eventType, void* message, long* result) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

protected slots:
    void switchStatus();

private:
    Ui::MainWindow* ui;

    StyleTheme styleTheme;

    bool mouseDrag;
    QPoint mousePos;
};
#endif // MAINWINDOW_H
