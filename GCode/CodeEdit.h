#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QPlainTextEdit>

class QCompleter;

class CodeEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CodeEdit(QWidget* parent = nullptr);

    void lineAreaPaintEvent(QPaintEvent* event);
    int lineAreaWidth();

    void setTheme(int theme);

    QChar charUnderCursor(int offset = 0);
    QString wordUnderCursor();
    int getIndentationSpace();

    bool completerBegin(QKeyEvent* event);
    void completerEnd(QKeyEvent* event);
    void setCompleter(QCompleter* completer);
    void insertCompletion(QString str);

protected:
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

private slots:
    void updateLineAreaWidth(int newBlockCount);

    void highlightCurrentLine(QList<QTextEdit::ExtraSelection>& extraSelections);
    void highlightParenthesis(QList<QTextEdit::ExtraSelection>& extraSelections);
    void updateExtraSelection();

    void updateLineArea(const QRect &rect, int dy);

private:
    QWidget* lineArea;
    QCompleter* completer;

    int theme;
};

#endif // CODEEDIT_H
