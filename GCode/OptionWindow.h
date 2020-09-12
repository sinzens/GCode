#ifndef OPTIONWINDOW_H
#define OPTIONWINDOW_H

#include <QMainWindow>

namespace Ui {
class OptionWindow;
}

class MainWindow;

class OptionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OptionWindow(QWidget *parent = nullptr);
    ~OptionWindow() Q_DECL_OVERRIDE;
    inline void initTheme();
    inline void initTitleBar();
    inline void initSplitter();
    inline void initWindowStatus();
    inline void initMouseStatus();
    inline void initEventFilter();
    inline void readSetting();
    MainWindow* parentWindow();

protected:
    bool eventFilter(QObject* object, QEvent* event) Q_DECL_OVERRIDE;

protected slots:
    void switchStatus();
    void confirmOption();
    void applyOption();
    void cancelOption();
    void handleStack();
    void optionHasChanged();

private:
    Ui::OptionWindow *ui;
    int styleTheme;

    MainWindow* parent;

    bool mouseDrag;
    QPoint mousePos;

    bool optionChanged;
    bool optionConfirmed;
};

#endif // OPTIONWINDOW_H
