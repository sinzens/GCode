#ifndef OPTIONWINDOW_H
#define OPTIONWINDOW_H

#include <QMainWindow>

namespace Ui {
class OptionWindow;
}

class OptionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OptionWindow(QWidget *parent = nullptr);
    ~OptionWindow() Q_DECL_OVERRIDE;
    inline void initTitleBar();
    inline void initSplitter();
    inline void initWindowStatus();
    inline void initMouseStatus();
    inline void initEventFilter();

protected:
    bool eventFilter(QObject* object, QEvent* event) Q_DECL_OVERRIDE;

protected slots:
    void switchStatus();
    void confirmOption();
    void applyOption();
    void cancelOption();
    void handleStack();

private:
    Ui::OptionWindow *ui;

    bool mouseDrag;
    QPoint mousePos;
};

#endif // OPTIONWINDOW_H
