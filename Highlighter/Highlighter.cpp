#include "Highlighter.h"

Highlighter::Highlighter(QTextDocument* parent, QStringList keyList) :
    QSyntaxHighlighter(parent)
{
    this->initClass();
    this->initFunction();
    //this->initDefType();
    this->initKeyword(keyList);
    this->initNumber();
    this->initQuotation();
    this->initSingleComment();
    this->initMultiComment();
}

void Highlighter::initKeyword(QStringList keyList)
{
    HighlightingRule rule;

    keywordFormat.setForeground(QColor(238,130,238));

    QStringList keywordPatterns;
    /*keywordPatterns << "\\basm\\b"
                    << "\\bauto\\b"
                    << "\\bbool\\b"
                    << "\\bbreak\\b"
                    << "\\bcase\\b"
                    << "\\bcatch\\b"
                    << "\\bchar\\b"
                    << "\\bclass\\b"
                    << "\\bconst\\b"
                    << "\\bconst_cast\\b"
                    << "\\bcontinue\\b"
                    << "\\bdefault\\b"
                    << "\\bdelete\\b"
                    << "\\bdo\\b"
                    << "\\bdouble\\b"
                    << "\\bdynamic_cast\\b"
                    << "\\belse\\b"
                    << "\\benum\\b"
                    << "\\bexplicit\\b"
                    << "\\bexport\\b"
                    << "\\bextern\\b"
                    << "\\bfalse\\b"
                    << "\\bfloat\\b"
                    << "\\bfor\\b"
                    << "\\bfriend\\b"
                    << "\\bgoto\\b"
                    << "\\bif\\b"
                    << "\\binline\\b"
                    << "\\bint\\b"
                    << "\\blong\\b"
                    << "\\bmutable\\b"
                    << "\\bnamespace\\b"
                    << "\\bnew\\b"
                    << "\\boperator\\b"
                    << "\\bprivate\\b"
                    << "\\bprotected\\b"
                    << "\\bpublic\\b"
                    << "\\bregister\\b"
                    << "\\breinterpret_cast\\b"
                    << "\\breturn\\b"
                    << "\\bshort\\b"
                    << "\\bsigned\\b"
                    << "\\bsizeof\\b"
                    << "\\bstatic\\b"
                    << "\\bstatic_cast\\b"
                    << "\\bstruct\\b"
                    << "\\bswitch\\b"
                    << "\\btemplate\\b"
                    << "\\bthis\\b"
                    << "\\bthrow\\b"
                    << "\\btrue\\b"
                    << "\\btry\\b"
                    << "\\btypedef\\b"
                    << "\\btypeid\\b"
                    << "\\btypename\\b"
                    << "\\bunion\\b"
                    << "\\bunsigned\\b"
                    << "\\busing\\b"
                    << "\\bvirtual\\b"
                    << "\\bvoid\\b"
                    << "\\bvolatile\\b"
                    << "\\bwchar_t\\b"
                    << "\\bwhile\\b"
                    << "\\balignas\\b"
                    << "\\balignof\\b"
                    << "\\bchar16_t\\b"
                    << "\\bchar32_t\\b"
                    << "\\bconstexpr\\b"
                    << "\\bdecltype\\b"
                    << "\\bnoexcept\\b"
                    << "\\bnullptr\\b"
                    << "\\bstatic_assert\\b"
                    << "\\bthread_local\\b"
                    << "\\bbitand\\b"
                    << "\\bbitor\\b"
                    << "\\bor\\b"
                    << "\\band\\b"
                    << "\\bnot\\b"
                    << "\\bxor\\b"
                    << "\\bor_eq\\b"
                    << "\\band_eq\\b"
                    << "\\bnot_eq\\b"
                    << "\\bxor_eq\\b"
                    << "\\bcompl\\b";*/
    for(QString & keyword : keyList)
    {
        keywordPatterns << QString("\\b" + keyword + "\\b");
    }

    for(const QString & pattern : keywordPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
}

void Highlighter::initClass()
{
    HighlightingRule rule;

    classFormat.setForeground(QColor(202, 105, 36));
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;

    highlightingRules.append(rule);
}

void Highlighter::initSingleComment()
{
    HighlightingRule rule;

    singleLineCommentFormat.setForeground(QColor(50, 205, 50));
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;

    highlightingRules.append(rule);
}

void Highlighter::initMultiComment()
{
    multiLineCommentFormat.setForeground(QColor(50, 205, 50));
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void Highlighter::initQuotation()
{
    HighlightingRule rule;

    quotationFormat.setForeground(QColor(93, 172, 129));
    //rule.pattern = QRegularExpression("\".*\"|\'.*\'|^#.*");
    //rule.pattern = QRegularExpression("\".*\"|\'.*\'|#[a-zA-Z_]+[\\s]*.*");
    rule.pattern = QRegularExpression("\".*\"|\'.*\'|^\\s*#\\s*include\\s*([<\"][^:?\"<>\\|]*[\">])|#[a-zA-Z_]+");
    rule.format = quotationFormat;

    highlightingRules.append(rule);
}

void Highlighter::initFunction()
{
    HighlightingRule rule;

    //functionFormat.setFontItalic(true);
    functionFormat.setForeground(QColor(30, 144, 255));
    //rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+[\\s]*(?=\\()");
    rule.pattern = QRegularExpression("\\b([_a-zA-Z][_a-zA-Z0-9]*\\s+)?((?:[_a-zA-Z][_a-zA-Z0-9]*\\s*::\\s*)*[_a-zA-Z][_a-zA-Z0-9]*)(?=\\s*\\()");
    rule.format = functionFormat;

    highlightingRules.append(rule);
}

void Highlighter::initNumber()
{
    HighlightingRule rule;

    numberFormat.setForeground(QColor(187, 119, 255));
    rule.pattern = QRegularExpression("((?<=\\b|\\s|^)(?i)(?:(?:(?:(?:(?:\\d+(?:'\\d+)*)?\\.(?:\\d+(?:'\\d+)*)(?:e[+-]?(?:\\d+(?:'\\d+)*))?)|(?:(?:\\d+(?:'\\d+)*)\\.(?:e[+-]?(?:\\d+(?:'\\d+)*))?)|(?:(?:\\d+(?:'\\d+)*)(?:e[+-]?(?:\\d+(?:'\\d+)*)))|(?:0x(?:[0-9a-f]+(?:'[0-9a-f]+)*)?\\.(?:[0-9a-f]+(?:'[0-9a-f]+)*)(?:p[+-]?(?:\\d+(?:'\\d+)*)))|(?:0x(?:[0-9a-f]+(?:'[0-9a-f]+)*)\\.?(?:p[+-]?(?:\\d+(?:'\\d+)*))))[lf]?)|(?:(?:(?:[1-9]\\d*(?:'\\d+)*)|(?:0[0-7]*(?:'[0-7]+)*)|(?:0x[0-9a-f]+(?:'[0-9a-f]+)*)|(?:0b[01]+(?:'[01]+)*))(?:u?l{0,2}|l{0,2}u?)))(?=\\b|\\s|$))");
    //rule.pattern = QRegularExpression(R"((?<=\b|\s|^)(?i)(?:(?:(?:(?:(?:\d+(?:'\d+)*)?\.(?:\d+(?:'\d+)*)(?:e[+-]?(?:\d+(?:'\d+)*))?)|(?:(?:\d+(?:'\d+)*)\.(?:e[+-]?(?:\d+(?:'\d+)*))?)|(?:(?:\d+(?:'\d+)*)(?:e[+-]?(?:\d+(?:'\d+)*)))|(?:0x(?:[0-9a-f]+(?:'[0-9a-f]+)*)?\.(?:[0-9a-f]+(?:'[0-9a-f]+)*)(?:p[+-]?(?:\d+(?:'\d+)*)))|(?:0x(?:[0-9a-f]+(?:'[0-9a-f]+)*)\.?(?:p[+-]?(?:\d+(?:'\d+)*))))[lf]?)|(?:(?:(?:[1-9]\d*(?:'\d+)*)|(?:0[0-7]*(?:'[0-7]+)*)|(?:0x[0-9a-f]+(?:'[0-9a-f]+)*)|(?:0b[01]+(?:'[01]+)*))(?:u?l{0,2}|l{0,2}u?)))(?=\b|\s|$))");
    rule.format = numberFormat;

    highlightingRules.append(rule);
}

void Highlighter::initDefType()
{
    HighlightingRule rule;

    defTypeFormat.setForeground(QColor(0, 0, 0));
    rule.pattern = QRegularExpression("\\b([_a-zA-Z][_a-zA-Z0-9]*)\\s+[_a-zA-Z][_a-zA-Z0-9]*\\s*[;=]");
    rule.format = defTypeFormat;

    highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text)
{
    for(const HighlightingRule & rule : highlightingRules)
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while(matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if(previousBlockState() != 1)
    {
        startIndex = text.indexOf(commentStartExpression);
    }

    while(startIndex >= 0)
    {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
