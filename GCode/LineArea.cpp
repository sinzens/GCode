#include "LineArea.h"
#include "CodeEdit.h"

LineArea::LineArea(CodeEdit* editor) : QWidget(editor), codeEditor(editor)
{

}

QSize LineArea::sizeHint() const
{
    return QSize(codeEditor->lineAreaWidth(), 0);
}

void LineArea::paintEvent(QPaintEvent* event)
{
    codeEditor->lineAreaPaintEvent(event);
}
