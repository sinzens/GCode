#include "CodeEdit.h"
#include "LineArea.h"
#include "MainWindow.h"

#include <QPainter>
#include <QStyleOption>
#include <QTextBlock>
#include <QCompleter>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QDebug>

static QVector<QPair<QString, QString>> parentheses =
{
    {"(", ")"},
    {"{", "}"},
    {"[", "]"},
    {"\"", "\""},
    {"'", "'"},
    {"<", ">"},
    {"-", ">"}
};

CodeEdit::CodeEdit(QWidget* parent)
    : QPlainTextEdit(parent)
    , completer(nullptr)
{
    this->lineArea = new LineArea(this);

    connect(this, &CodeEdit::blockCountChanged, this, &CodeEdit::updateLineAreaWidth);
    connect(this, &CodeEdit::updateRequest, this, &CodeEdit::updateLineArea);
    connect(this, &CodeEdit::cursorPositionChanged, this, &CodeEdit::updateExtraSelection);

    this->updateLineAreaWidth(0);
}

void CodeEdit::lineAreaPaintEvent(QPaintEvent* event)
{
    QStyleOption option;
    option.init(this->lineArea);
    QPainter painter(this->lineArea);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this->lineArea);

    //painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while(block.isValid() && top <= event->rect().bottom())
    {
        if(block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.drawText(-5, top, lineArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

int CodeEdit::lineAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while(max >= 10)
    {
        max /= 10;
        ++digits;
    }
    int space = 15 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void CodeEdit::setTheme(int theme)
{
    switch(theme)
    {
    case DarkTheme:
        this->theme = DarkTheme;
        break;
    case LightTheme:
        this->theme = LightTheme;
        break;
    }

    this->updateExtraSelection();
}

QChar CodeEdit::charUnderCursor(int offset)
{
    int block = this->textCursor().blockNumber();
    int index = this->textCursor().positionInBlock();
    QString text = this->document()->findBlockByNumber(block).text();

    index += offset;

    if (index < 0 || index >= text.size())
    {
        return {};
    }

    return text[index];
}

QString CodeEdit::wordUnderCursor()
{
    QTextCursor cursor = this->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    return cursor.selectedText();
}

int CodeEdit::getIndentationSpace()
{
    QString blockText = this->textCursor().block().text();
    int indentationLevel = 0;

    for(int i = 0; i < blockText.size() && QString("\t ").contains(blockText[i]); i++)
    {
        if(blockText[i] == ' ')
        {
            indentationLevel++;
        }
        else
        {
            //indentationLevel += static_cast<int>(tabStopDistance() / fontMetrics().averageCharWidth());
            //indentationLevel += tabStopDistance() / fontMetrics().averageCharWidth();
            indentationLevel += 4;
        }
    }
    return indentationLevel;
}

bool CodeEdit::completerBegin(QKeyEvent *event)
{
    if(this->completer && this->completer->popup()->isVisible())
    {
        switch(event->key())
        {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            event->ignore();
            return true;
        default:
            break;
        }
    }
    bool isShortcut = ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_Space);
    return !(!this->completer || !isShortcut);
}

void CodeEdit::completerEnd(QKeyEvent *event)
{
    QFlags<Qt::KeyboardModifier> ctrlOrShift = event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);

    if(!this->completer || (ctrlOrShift && event->text().isEmpty()) ||
        event->key() == Qt::Key_Delete)
    {
        return;
    }

    static QString eow(R"(~!@#$%^&*()_+{}|:"<>?,./;'[]\-=)");

    bool isShortcut = ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_Space);
    QString completionPrefix = wordUnderCursor();

    if(!isShortcut && (event->text().isEmpty() ||
        completionPrefix.length() < 2 ||
        eow.contains(event->text().right(1))))
    {
        this->completer->popup()->hide();
        return;
    }

    if (completionPrefix != this->completer->completionPrefix())
    {
        this->completer->setCompletionPrefix(completionPrefix);
        this->completer->popup()->setCurrentIndex(this->completer->completionModel()->index(0, 0));
    }

    QRect cursRect = cursorRect();
    cursRect.setWidth
    (
        this->completer->popup()->sizeHintForColumn(0) +
        this->completer->popup()->verticalScrollBar()->sizeHint().width()
    );

    this->completer->complete(cursRect);
}

void CodeEdit::setCompleter(QCompleter* completer_)
{
    if(this->completer)
    {
        disconnect(this->completer, nullptr, this, nullptr);
    }

    this->completer = completer_;

    if(!this->completer)
    {
        return;
    }

    this->completer->setWidget(this);
    this->completer->setCompletionMode(QCompleter::CompletionMode::PopupCompletion);

    connect
    (
        this->completer,
        QOverload<const QString&>::of(&QCompleter::activated),
        this,
        &CodeEdit::insertCompletion
    );
}

void CodeEdit::insertCompletion(QString str)
{
    if(this->completer->widget() != this)
    {
        return;
    }

    QTextCursor cursor = this->textCursor();
    cursor.select(QTextCursor::SelectionType::WordUnderCursor);
    cursor.insertText(str);
    setTextCursor(cursor);
}

void CodeEdit::resizeEvent(QResizeEvent* event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect contentRect = contentsRect();
    this->lineArea->setGeometry(QRect(contentRect.left(), contentRect.top(), lineAreaWidth(), contentRect.height()));
}

void CodeEdit::keyPressEvent(QKeyEvent* event)
{
    const int defaultIndent = 4;

    bool skipCompleter = this->completerBegin(event);

    if(!skipCompleter)
    {
        if(event->key() == Qt::Key_Tab && event->modifiers() == Qt::NoModifier)
        {
            insertPlainText(QString(4, ' '));
            return;
        }
        int indentationLevel = this->getIndentationSpace();
        //qDebug() << defaultIndent;

        int tabCount = static_cast<int>(indentationLevel * fontMetrics().averageCharWidth() / tabStopDistance());

        if((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) &&
            charUnderCursor() == '}' && charUnderCursor(-1) == '{')
        {
            int charsBack = 0;
            insertPlainText("\n");

            insertPlainText(QString(indentationLevel + defaultIndent, ' '));

            insertPlainText("\n");
            charsBack++;

            insertPlainText(QString(indentationLevel, ' '));
            charsBack += indentationLevel;

            while(charsBack--)
            {
                this->moveCursor(QTextCursor::MoveOperation::Left);
            }
            return;
        }

        if(event->key() == Qt::Key_Backtab)
        {
            QTextCursor cursor = this->textCursor();

            cursor.movePosition(QTextCursor::MoveOperation::StartOfLine);
            cursor.movePosition(QTextCursor::MoveOperation::Right, QTextCursor::MoveMode::KeepAnchor, indentationLevel);

            cursor.removeSelectedText();
            return;
        }

        QPlainTextEdit::keyPressEvent(event);

        if((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter))
        {
            this->insertPlainText(QString(indentationLevel, ' '));
        }

        for(QPair<QString, QString> & pair : parentheses)
        {
            if(pair.first == event->text())
            {
                this->insertPlainText(pair.second);
                this->moveCursor(QTextCursor::MoveOperation::Left);
                break;
            }
            if(pair.second == event->text())
            {
                QChar symbol = this->charUnderCursor();
                if(symbol == pair.second)
                {
                    this->textCursor().deletePreviousChar();
                    this->moveCursor(QTextCursor::MoveOperation::Right);
                }
                break;
            }
        }
    }
    this->completerEnd(event);
}

void CodeEdit::updateLineAreaWidth(int newBlockCount)
{
    Q_UNUSED(newBlockCount)
    this->setViewportMargins(lineAreaWidth(), 0, 0, 0);
}

void CodeEdit::highlightCurrentLine(QList<QTextEdit::ExtraSelection>& extraSelections)
{
    if(!this->isReadOnly())
    {
        QTextEdit::ExtraSelection selection;
        QColor lineColor;

        switch(this->theme)
        {
        case DarkTheme:
            lineColor = QColor(110, 110, 110);
            break;
        case LightTheme:
            lineColor = QColor(230, 230, 230);
            break;
        default:
            lineColor = QColor(110, 110, 110);
            break;
        }

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();

        extraSelections.append(selection);
    }
}

void CodeEdit::highlightParenthesis(QList<QTextEdit::ExtraSelection>& extraSelections)
{
    QChar currentSymbol = this->charUnderCursor();
    QChar prevSymbol = this->charUnderCursor(-1);

    for(QPair<QString, QString> & pair : parentheses)
    {
        int direction;

        QChar counterSymbol;
        QChar activeSymbol;
        int position = this->textCursor().position();

        if(pair.first == currentSymbol)
        {
            direction = 1;
            counterSymbol = pair.second[0];
            activeSymbol = currentSymbol;
        }
        else if(pair.second == prevSymbol)
        {
            direction = -1;
            counterSymbol = pair.first[0];
            activeSymbol = prevSymbol;
            position--;
        }
        else
        {
            continue;
        }

        int counter = 1;

        while(counter != 0 && position > 0 && position < (document()->characterCount() - 1))
        {
            position += direction;

            QChar character = this->document()->characterAt(position);
            if (character == activeSymbol)
            {
                ++counter;
            }
            else if (character == counterSymbol)
            {
                --counter;
            }
        }

        QTextCharFormat format;
        QColor parenthesisColor;
        switch(this->theme)
        {
        case DarkTheme:
            parenthesisColor = QColor(164, 194, 196);
            break;
        case LightTheme:
            parenthesisColor = QColor(255, 255, 104);
            break;
        default:
            parenthesisColor = QColor(164, 194, 196);
            break;
        }
        format.setBackground(parenthesisColor);

        if(counter == 0)
        {
            QTextEdit::ExtraSelection selection{};

            QTextCursor::MoveOperation directionEnum = direction < 0 ? QTextCursor::MoveOperation::Left : QTextCursor::MoveOperation::Right;

            selection.format = format;
            selection.cursor = textCursor();
            selection.cursor.clearSelection();
            selection.cursor.movePosition
            (
                directionEnum,
                QTextCursor::MoveMode::MoveAnchor,
                std::abs(textCursor().position() - position)
            );

            selection.cursor.movePosition
            (
                QTextCursor::MoveOperation::Right,
                QTextCursor::MoveMode::KeepAnchor,
                1
            );

            extraSelections.append(selection);

            selection.cursor = textCursor();
            selection.cursor.clearSelection();
            selection.cursor.movePosition
            (
                directionEnum,
                QTextCursor::MoveMode::KeepAnchor,
                1
            );

            extraSelections.append(selection);
        }
        break;
    }
}

void CodeEdit::updateExtraSelection()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    this->highlightCurrentLine(extraSelections);
    this->highlightParenthesis(extraSelections);

    this->setExtraSelections(extraSelections);
}

void CodeEdit::updateLineArea(const QRect &rect, int dy)
{
    if(dy)
    {
        this->lineArea->scroll(0, dy);
    }
    else
    {
        this->lineArea->update(0, rect.y(), lineArea->width(), rect.height());
    }

    if(rect.contains(viewport()->rect()))
    {
        this->updateLineAreaWidth(0);
    }
}
