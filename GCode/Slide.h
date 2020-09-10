#ifndef SLIDE_H
#define SLIDE_H

#include <QWidget>

namespace Ui {
class Slide;
}

class Slide : public QWidget
{
    Q_OBJECT

public:
    explicit Slide(QWidget *parent = nullptr);
    ~Slide();
    void progressAdd(int percent);
    void progressDec(int percent);

private:
    Ui::Slide *ui;
};

#endif // SLIDE_H
