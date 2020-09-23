#ifndef LINEAREA_H
#define LINEAREA_H

#include <QWidget>

class CodeEdit;

class LineArea : public QWidget
{
    Q_OBJECT
public:
    explicit LineArea(CodeEdit* editor = nullptr);
    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

private:
    CodeEdit* codeEditor;
};

#endif // LINEAREA_H
