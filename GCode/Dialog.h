#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class MainWindow;
class OptionWindow;

enum ParentType
{
    Main,
    Option
};

union ParentPtr
{
    MainWindow* parentMain;
    OptionWindow* parentOption;
};

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget* parent = nullptr, ParentType = Main);
    ~Dialog() Q_DECL_OVERRIDE;
    inline void initTheme();
    inline void initMouseStatus();
    inline void initEventFilter();

    void setConfirmText(QString text);
    void setCancelText(QString text);
    void setButtonText(QString confirmText, QString cancelText);
    void setInfo(QString text);

protected:
    bool eventFilter(QObject* object, QEvent* event) Q_DECL_OVERRIDE;

private:
    Ui::Dialog *ui; 
    int styleTheme;

    ParentPtr parent;

    bool mouseDrag;
    QPoint mousePos;

    ParentType parentType;
};

#endif // DIALOG_H
