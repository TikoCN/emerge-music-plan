#include "LrcSceneGraphItem.h"

#include <QQuickWindow>
#include <QSGNode>
#include <QSGTextNode>
#include <QTextLayout>
#include <QTextOption>
#include <algorithm>

namespace {

/**
 * @brief 按相同的换行规则完成测量和绘制所使用的文本排版。
 * @param layout 待排版的文本。
 * @param width 可用宽度。
 * @return 排版后的总高度。
 */
qreal layoutText(QTextLayout &layout, const qreal width) {
    QTextOption option;
    option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    layout.setTextOption(option);
    layout.setCacheEnabled(true);
    layout.beginLayout();
    qreal height = 0;
    while (true) {
        QTextLine line = layout.createLine();
        if (!line.isValid()) {
            break;
        }
        line.setLineWidth(width);
        line.setPosition(QPointF(0, height));
        height += line.height();
    }
    layout.endLayout();
    return height;
}

}

void LrcSceneGraphItem::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) {
    QQuickItem::geometryChange(newGeometry, oldGeometry);
    if (newGeometry.width() != oldGeometry.width()) {
        updateImplicitHeight();
    }
    if (newGeometry.size() != oldGeometry.size()) {
        update();
    }
}

LrcSceneGraphItem::LrcSceneGraphItem(QQuickItem *parent)
    : QQuickItem(parent) {
    setFlag(ItemHasContents, true);
}

QString LrcSceneGraphItem::text() const {
    return m_text;
}

void LrcSceneGraphItem::setText(const QString &text) {
    if (m_text == text) {
        return;
    }
    m_text = text;
    updateImplicitHeight();
    emit textChanged();
    update();
}

QFont LrcSceneGraphItem::font() const {
    return m_font;
}

void LrcSceneGraphItem::setFont(const QFont &font) {
    if (m_font == font) {
        return;
    }
    m_font = font;
    updateImplicitHeight();
    emit fontChanged();
    update();
}

QColor LrcSceneGraphItem::color() const {
    return m_color;
}

QColor LrcSceneGraphItem::normalColor() const {
    return m_normalColor;
}

void LrcSceneGraphItem::setNormalColor(const QColor &color) {
    if (m_normalColor == color) {
        return;
    }
    m_normalColor = color;
    emit normalColorChanged();
    update();
}

void LrcSceneGraphItem::updateImplicitHeight() {
    if (m_text.isEmpty() || width() <= 0) {
        setImplicitHeight(0);
        return;
    }

    // QTextLayout 使用行分隔符强制换行；测量和绘制必须使用同一份文本。
    QString displayText = m_text;
    displayText.replace(QLatin1Char('\n'), QChar(QChar::LineSeparator));
    QTextLayout layout(displayText, m_font);
    setImplicitHeight(layoutText(layout, width()));
}

void LrcSceneGraphItem::setColor(const QColor &color) {
    if (m_color == color) {
        return;
    }
    m_color = color;
    emit colorChanged();
    update();
}

QVariantList LrcSceneGraphItem::words() const {
    return m_words;
}

void LrcSceneGraphItem::setWords(const QVariantList &words) {
    if (m_words == words) {
        return;
    }
    m_words = words;
    updateHighlightedWord();
    emit wordsChanged();
    update();
}

QVariantList LrcSceneGraphItem::wordStarts() const {
    return m_wordStarts;
}

void LrcSceneGraphItem::setWordStarts(const QVariantList &wordStarts) {
    if (m_wordStarts == wordStarts) {
        return;
    }
    m_wordStarts = wordStarts;
    updateHighlightedWord();
    emit wordStartsChanged();
    update();
}

QVariantList LrcSceneGraphItem::wordEnds() const {
    return m_wordEnds;
}

void LrcSceneGraphItem::setWordEnds(const QVariantList &wordEnds) {
    if (m_wordEnds == wordEnds) {
        return;
    }
    m_wordEnds = wordEnds;
    updateHighlightedWord();
    emit wordEndsChanged();
    update();
}

qint64 LrcSceneGraphItem::position() const {
    return m_position;
}

void LrcSceneGraphItem::setPosition(const qint64 position) {
    if (m_position == position) {
        return;
    }
    m_position = position;
    emit positionChanged();
    updateHighlightedWord();
}

void LrcSceneGraphItem::updateHighlightedWord() {
    int highlightedWord = -1;
    qreal wordProgress = 0.0;
    if (m_playing) {
        const int wordCount = static_cast<int>(std::min(m_words.size(), std::min(m_wordStarts.size(), m_wordEnds.size())));
        for (int i = 0; i < wordCount; ++i) {
            const qint64 wordStart = m_wordStarts[i].toLongLong();
            const qint64 wordEnd = m_wordEnds[i].toLongLong();
            if (m_position < wordStart) {
                break;
            }
            highlightedWord = i;
            if (wordEnd > wordStart && m_position < wordEnd) {
                wordProgress = static_cast<qreal>(m_position - wordStart) / (wordEnd - wordStart);
                break;
            }
            wordProgress = 1.0;
        }
    }
    if (m_highlightedWord != highlightedWord || !qFuzzyCompare(m_wordProgress + 1.0, wordProgress + 1.0)) {
        m_highlightedWord = highlightedWord;
        m_wordProgress = wordProgress;
        update();
    }
}

bool LrcSceneGraphItem::playing() const {
    return m_playing;
}

void LrcSceneGraphItem::setPlaying(const bool playing) {
    if (m_playing == playing) {
        return;
    }
    m_playing = playing;
    emit playingChanged();
    updateHighlightedWord();
    update();
}

QSGNode *LrcSceneGraphItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) {
    if (width() <= 0 || height() <= 0 || window() == nullptr) {
        delete oldNode;
        return nullptr;
    }

    QSGTextNode *node = static_cast<QSGTextNode *>(oldNode);
    if (node == nullptr) {
        node = window()->createTextNode();
    }
    node->clear();
    node->setColor(m_normalColor);
    node->setRenderType(QSGTextNode::QtRendering);
    node->setViewport(boundingRect());

    // 完整文本始终参与绘制，时间数组只决定高亮，不决定文字是否可见。
    QString displayText = m_text;
    displayText.replace(QLatin1Char('\n'), QChar(QChar::LineSeparator));
    QTextLayout layout(displayText, m_font);
    QList<QTextLayout::FormatRange> formats;
    int offset = 0;
    const int timedWordCount = static_cast<int>(std::min(m_words.size(), std::min(m_wordStarts.size(), m_wordEnds.size())));
    for (int i = 0; i < timedWordCount; ++i) {
        const int length = static_cast<int>(m_words[i].toString().size());
        if (m_playing && i <= m_highlightedWord) {
            QTextLayout::FormatRange range;
            range.start = offset;
            range.length = length;
            QColor wordColor = m_color;
            if (i == m_highlightedWord && m_wordProgress < 1.0) {
                wordColor.setRedF(m_normalColor.redF() + (m_color.redF() - m_normalColor.redF()) * m_wordProgress);
                wordColor.setGreenF(m_normalColor.greenF() + (m_color.greenF() - m_normalColor.greenF()) * m_wordProgress);
                wordColor.setBlueF(m_normalColor.blueF() + (m_color.blueF() - m_normalColor.blueF()) * m_wordProgress);
                wordColor.setAlphaF(m_normalColor.alphaF() + (m_color.alphaF() - m_normalColor.alphaF()) * m_wordProgress);
            }
            range.format.setForeground(wordColor);
            formats.append(range);
        }
        offset += length;
    }
    layout.setFormats(formats);
    layoutText(layout, width());
    node->addTextLayout(QPointF(0, 0), &layout);
    return node;
}
