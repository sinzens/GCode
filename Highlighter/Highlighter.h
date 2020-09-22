#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include "Highlighter_global.h"

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QTextDocument>

struct HighlightingRule
{
    QRegularExpression pattern;
    QTextCharFormat format;
};

struct HighlightingRule2
{
    QRegularExpression pattern;
    QTextCharFormat format;
};

class HIGHLIGHTER_EXPORT Highlighter : public QSyntaxHighlighter
{
public:
    explicit Highlighter(QTextDocument* parent = nullptr);

    inline void initKeyword();
    inline void initClass();
    inline void initSingleComment();
    inline void initMultiComment();
    inline void initQuotation();
    inline void initFunction();

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    QVector<HighlightingRule> highlightingRules;
    QVector<HighlightingRule2> highlightingRules2;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat keywordFormat2;
    QTextCharFormat stringFormat;
};

#endif // HIGHLIGHTER_H
