#include "TextBubble.h"
#include <QFontMetricsF>
#include <QDebug>
#include <QFont>
#include "globle.h"
#include <QTimer>
#include <QTextDocument>
#include <QTextBlock>
#include <QTextLayout>
#include <QFont>
TextBubble::TextBubble(ChatRole role, const QString &text, QWidget *parent)
    :BubbleFrame(role, parent)
{
    m_pTextEdit = new QTextEdit();
    m_pTextEdit->setReadOnly(true);
    m_pTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pTextEdit->installEventFilter(this);
    QFont font("Microsoft YaHei");
    font.setPointSize(12);
    m_pTextEdit->setFont(font);
    setPlainText(text);
    setWidget(m_pTextEdit);
    initStyleSheet();
}

void TextBubble::setPlainText(const QString &text)
{
    m_pTextEdit->setPlainText(text);

    // 1. 获取文档边距
    qreal doc_margin = m_pTextEdit->document()->documentMargin();
    int margin_left = this->layout()->contentsMargins().left();
    int margin_right = this->layout()->contentsMargins().right();

    QFontMetricsF fm(m_pTextEdit->font());
    QTextDocument *doc = m_pTextEdit->document();
    qreal max_width = 0;

    // 2. 遍历每一段，使用 qreal 保留精度
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())
    {
        // 使用 horizontalAdvance 获取精确浮点宽度
        qreal txtW = fm.horizontalAdvance(it.text());
        max_width = qMax(max_width, txtW);
    }

    // 3. 关键修改：增加 2-3 像素的“缓冲区”补偿误差
    // 并且使用 qCeil 向上取整，确保空间足够
    int finalWidth = qCeil(max_width + (doc_margin * 2) + (margin_left + margin_right) + 3);

    setMaximumWidth(finalWidth);
}
// void TextBubble::setPlainText(const QString &text)
// {
//     m_pTextEdit->setPlainText(text);
//     //m_pTextEdit->setHtml(text);
//     //找到段落中最大宽度
//     qreal doc_margin = m_pTextEdit->document()->documentMargin();
//     int margin_left = this->layout()->contentsMargins().left();
//     int margin_right = this->layout()->contentsMargins().right();
//     QFontMetricsF fm(m_pTextEdit->font());
//     QTextDocument *doc = m_pTextEdit->document();
//     int max_width = 0;
//     //遍历每一段找到 最宽的那一段
//     for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())    //字体总长
//     {
//         int txtW = int(fm.horizontalAdvance(it.text()));
//         max_width = max_width < txtW ? txtW : max_width;                 //找到最长的那段
//     }

//     //设置这个气泡的最大宽度 只需要设置一次
//     setMaximumWidth(max_width + doc_margin * 2 + (margin_left + margin_right));        //设置最大宽度
// }

bool TextBubble::eventFilter(QObject *o, QEvent *e)
{
    if(m_pTextEdit == o && e->type() == QEvent::Paint)
    {
        adjustTextHeight(); //PaintEvent中设置
    }
    return BubbleFrame::eventFilter(o, e);
}

void TextBubble::adjustTextHeight()
{
    qreal doc_margin = m_pTextEdit->document()->documentMargin();    //字体到边框的距离默认为4
    QTextDocument *doc = m_pTextEdit->document();
    qreal text_height = 0;

    //把每一段的高度相加=文本高
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next())
    {
        QTextLayout *pLayout = it.layout();
        QRectF text_rect = pLayout->boundingRect();                             //这段的rect
        text_height += text_rect.height();
    }
    int vMargin = this->layout()->contentsMargins().top();
    //设置这个气泡需要的高度 文本高+文本边距+TextEdit边框到气泡边框的距离
    setFixedHeight(text_height + doc_margin *2 + vMargin*2 );

    // 最终高度 = 文本总高 + 边距，移除冗余的vMargin*2（避免高度过度放大）


}

void TextBubble::initStyleSheet()
{
    m_pTextEdit->setStyleSheet("QTextEdit{background:transparent;border:none}");
}

