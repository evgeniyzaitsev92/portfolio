#include "searchhighlight.h"

#include <QTextCharFormat>

SearchHighLight::SearchHighLight(QTextDocument* parent) : BaseClass(parent)
{
    // Устанавливаем цвет подсветки
    m_format.setBackground(Qt::green);
}

void SearchHighLight::highlightBlock(const QString& text)
{
    // С помощью регулярного выражения находим все совпадения
    QRegularExpressionMatchIterator matchIterator = m_pattern.globalMatch(text);
    while (matchIterator.hasNext())
    {
        // Подсвечиваем все совпадения
        QRegularExpressionMatch match = matchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), m_format);
    }
}

void SearchHighLight::searchText(const QString& text)
{
    // Устанавливаем текст в виде регулярного выражения
    m_pattern = QRegularExpression(text, QRegularExpression::CaseInsensitiveOption);
    rehighlight(); // Перезапускаем подсветку
}
