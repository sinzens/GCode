#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

enum UiStyle
{
    LightTheme,
    DarkTheme
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Slide;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() Q_DECL_OVERRIDE;
    void slideProcess(Slide* slide);
    inline void initDockWidget();
    inline void initMenuBar();
    inline void initToolBar();
    inline void initTabWidget();
    inline void initWindowStatus();
    inline void initMouseStatus();
    inline void initEventFilter();
    inline void initSetting();
    inline void initStylesheet();
    inline void delayToShow(int mesc);
    void changeEditorFont(QFont font);
    void changeCompilerPath(QString path);
    void changeThemeTo(UiStyle theme);
    UiStyle theme();
    QSettings* settings();
public slots:
    void receivefindsignal(QString Data,int flag,QString Data1);

protected:
    bool eventFilter(QObject* object, QEvent* event) Q_DECL_OVERRIDE;
    bool nativeEvent(const QByteArray &eventType, void* message, long* result) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

protected slots:
    void switchStatus();
    void removeTab(int index);
    void showOption();
    void showFindReplace();
    void showAboutThis();
    void showAboutUs();
private:
    Ui::MainWindow* ui;

    UiStyle styleTheme;

    QString compilerPath;

    bool mouseDrag;
    QPoint mousePos;

    QSettings* setting;
};
#endif // MAINWINDOW_H
