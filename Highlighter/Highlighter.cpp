#include "Highlighter.h"

Highlighter::Highlighter(QTextDocument* parent) :
    QSyntaxHighlighter(parent)
{
    /*HighlightingRule rule,rule2;

    classFormat.setForeground(QColor(202,105,36));

    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;

    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(QColor(50,205,50));

    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;

    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QColor(50,205,50));

    quotationFormat.setForeground(QColor(93,172,129));

    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;

    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(QColor(30,144,255));

    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;

    highlightingRules.append(rule);

    keywordFormat.setFontWeight(9);
    keywordFormat.setForeground(QColor(238,130,238));

    QStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b"     << "\\bclass\\b"     << "\\bconst\\b"
                    << "\\bdouble\\b"   << "\\benum\\b"      << "\\bexplicit\\b"
                    << "\\bfriend\\b"   << "\\binline\\b"    << "\\bint\\b"
                    << "\\blong\\b"     << "\\bnamespace\\b" << "\\boperator\\b"
                    << "\\bprivate\\b"  << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\bshort\\b"    << "\\bsignals\\b"   << "\\bsigned\\b"
                    << "\\bslots\\b"    << "\\bstatic\\b"    << "\\bstruct\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b"   << "\\btypename\\b"
                    << "\\bunion\\b"    << "\\bunsigned\\b"  << "\\bvirtual\\b"
                    << "\\bvoid\\b"     << "\\bvolatile\\b"  << "\\bbool\\b"
                    << "\\bfor\\b"      << "\\bif\\b"        << "\\belse\\b"
                    << "\\bwhile\\b";
    foreach(const QString & pattern, keywordPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }*/

    /*HighlightingRule rule2;
    keywordFormat2.setFontWeight(9);
    keywordFormat2.setForeground(QColor(202,105,36));

    QStringList keywordPatterns2;
    keywordPatterns2 << "\\bprintf\\b" << "\\bscanf\\b" << "\\bcin\\b"
                     << "\\bcout\\b" << "\\bmemset\\b" << "\\b#include\\b";
    foreach(const QString & pattern, keywordPatterns2)
    {
        rule2.pattern = QRegularExpression(pattern);
        rule2.format = keywordFormat2;
        highlightingRules.append(rule2);
    }*/

    //commentStartExpression = QRegularExpression("/\\*");
    //commentEndExpression = QRegularExpression("\\*/");

    this->initClass();
    this->initFunction();
    this->initKeyword();
    this->initQuotation();
    this->initSingleComment();
    this->initMultiComment();
}

void Highlighter::initKeyword()
{
    HighlightingRule rule;

    keywordFormat.setForeground(QColor(238,130,238));

    QStringList keywordPatterns;
    keywordPatterns << "\\basm\\b"
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
                    << "\\bcompl\\b";

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
    rule.pattern = QRegularExpression("\".*\"|\'.*\'|^#.*");
    rule.format = quotationFormat;

    highlightingRules.append(rule);
}

void Highlighter::initFunction()
{
    HighlightingRule rule;

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(QColor(30, 144, 255));
    //rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+[\\s]*(?=\\()");
    rule.format = functionFormat;

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
