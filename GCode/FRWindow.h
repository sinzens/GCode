#ifndef FRWINDOW_H
#define FRWINDOW_H

#include <QDialog>

enum FRFlag
{
    FindNext,
    FindAll,
    ReplaceNext,
    ReplaceAll
};

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

signals:
    void frRequested(QString str1, QString str2, int flag);

protected:
    bool eventFilter(QObject* object, QEvent* event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

protected slots:
    void findNext();
    void findAll();
    void replaceNext();
    void replaceAll();

private:
    Ui::FRWindow *ui;
    int styleTheme;

    MainWindow* parent;

    bool mouseDrag;
    QPoint mousePos;
};

#endif // FRWINDOW_H
