#include "LrcSceneGraphItem.h"

#include <QMatrix4x4>
#include <QQuickWindow>
#include <QSGClipNode>
#include <QSGTransformNode>
#include <QTextBoundaryFinder>
#include <QSGNode>
#include <QSGTextNode>
#include <QTextLayout>
#include <QTextOption>
#include <QVector>
#include <algorithm>

namespace {

/** @brief 保存单字节点及其时间区间，播放时只更新变换和裁剪。 */
struct LyricCharacterNode {
    QSGTransformNode *transform = nullptr;
    QSGClipNode *normalClip = nullptr;
    QSGClipNode *playingClip = nullptr;
    QPointF position;
    qreal advance = 0;
    qreal padding = 0;
    qreal height = 0;
    qreal startTime = 0;
    qreal endTime = 0;
    qreal progress = -1;
    bool timed = false;
};

/** @brief 在场景图线程缓存歌词排版、字形节点和对应的输入数据。 */
struct LyricLineNode : public QSGNode {
    QString text;
    QFont font;
    QColor color;
    QColor normalColor;
    qreal width = -1;
    QVariantList words;
    QVariantList starts;
    QVariantList ends;
    QVector<LyricCharacterNode> characters;
};

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
    // 为已播放文字预留上浮空间，测量和绘制使用相同的顶部留白。
    qreal height = 3.0;
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
    update();
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
    update();
}

QSGNode *LrcSceneGraphItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) {
    if (width() <= 0 || height() <= 0 || window() == nullptr) {
        delete oldNode;
        return nullptr;
    }

    LyricLineNode *node = static_cast<LyricLineNode *>(oldNode);
    if (node == nullptr) {
        node = new LyricLineNode;
    }
    const bool rebuild = node->text != m_text || node->font != m_font || node->width != width()
                         || node->color != m_color || node->normalColor != m_normalColor
                         || node->words != m_words || node->starts != m_wordStarts || node->ends != m_wordEnds;
    if (rebuild) {
        node->characters.clear();
        while (node->firstChild() != nullptr) {
            delete node->firstChild();
        }
        node->text = m_text;
        node->font = m_font;
        node->width = width();
        node->color = m_color;
        node->normalColor = m_normalColor;
        node->words = m_words;
        node->starts = m_wordStarts;
        node->ends = m_wordEnds;

        QString displayText = m_text;
        displayText.replace(QLatin1Char('\n'), QChar(QChar::LineSeparator));
        QTextLayout layout(displayText, m_font);
        layoutText(layout, width());
        QVector<qreal> starts(displayText.size(), 0.0);
        QVector<qreal> ends(displayText.size(), 0.0);
        QVector<bool> timed(displayText.size(), false);
        int offset = 0;
        const int wordCount = static_cast<int>(std::min(m_words.size(), std::min(m_wordStarts.size(), m_wordEnds.size())));
        for (int i = 0; i < wordCount; ++i) {
            const QString word = m_words[i].toString();
            QList<int> boundaries;
            boundaries.append(0);
            QTextBoundaryFinder wordFinder(QTextBoundaryFinder::Grapheme, word);
            int boundary = wordFinder.toNextBoundary();
            while (boundary >= 0) {
                boundaries.append(boundary);
                boundary = wordFinder.toNextBoundary();
            }
            const int count = static_cast<int>(boundaries.size()) - 1;
            const qreal startTime = m_wordStarts[i].toLongLong();
            const qreal duration = m_wordEnds[i].toLongLong() - startTime;
            // 原有单字时间戳直接保留，多字片段的时长只在重建时分配一次。
            for (int character = 0; character < count; ++character) {
                for (int position = offset + boundaries[character];
                     position < offset + boundaries[character + 1] && position < displayText.size(); ++position) {
                    starts[position] = startTime + duration * character / count;
                    ends[position] = startTime + duration * (character + 1) / count;
                    timed[position] = true;
                }
            }
            offset += static_cast<int>(word.size());
        }

        QTextBoundaryFinder finder(QTextBoundaryFinder::Grapheme, displayText);
        int start = 0;
        int end = finder.toNextBoundary();
        while (end >= 0) {
            const QString character = displayText.mid(start, end - start);
            const QTextLine line = layout.lineForTextPosition(start);
            if (line.isValid() && character != QChar(QChar::LineSeparator)) {
                QTextLayout characterLayout(character, m_font);
                characterLayout.beginLayout();
                QTextLine characterLine = characterLayout.createLine();
                characterLine.setLineWidth(width());
                characterLayout.endLayout();
                LyricCharacterNode cached;
                cached.position = QPointF(line.cursorToX(start), line.y() + line.ascent() - characterLine.ascent());
                cached.advance = characterLine.horizontalAdvance();
                cached.padding = characterLine.height();
                cached.height = characterLine.height();
                cached.startTime = starts[start];
                cached.endTime = ends[start];
                cached.timed = timed[start];
                cached.transform = new QSGTransformNode;
                node->appendChildNode(cached.transform);

                // 两个颜色图层长期复用，播放过程中不重新生成字形或创建节点。
                for (int layer = 0; layer < 2; ++layer) {
                    QSGClipNode *clip = new QSGClipNode;
                    clip->setIsRectangular(true);
                    cached.transform->appendChildNode(clip);
                    if (layer == 0) {
                        cached.normalClip = clip;
                    } else {
                        cached.playingClip = clip;
                    }
                    QSGTextNode *textNode = window()->createTextNode();
                    textNode->setColor(layer == 0 ? m_normalColor : m_color);
                    textNode->setRenderType(QSGTextNode::QtRendering);
                    textNode->setViewport(QRectF(-cached.padding, -cached.padding,
                                                cached.advance + cached.padding * 2, cached.height + cached.padding * 2));
                    textNode->addTextLayout(QPointF(0, 0), &characterLayout);
                    clip->appendChildNode(textNode);
                }
                node->characters.append(cached);
            }
            start = end;
            end = finder.toNextBoundary();
        }
    }

    // 播放位置更新只修改进度发生变化的单字，已唱和未唱部分保持缓存。
    for (LyricCharacterNode &character : node->characters) {
        qreal progress = 0.0;
        if (m_playing && character.timed) {
            if (m_position >= character.endTime) {
                progress = 1.0;
            } else if (m_position > character.startTime && character.endTime > character.startTime) {
                progress = (m_position - character.startTime) / (character.endTime - character.startTime);
            }
        }
        if (qFuzzyCompare(character.progress + 1.0, progress + 1.0)) {
            continue;
        }
        character.progress = progress;
        QMatrix4x4 transform;
        transform.translate(character.position.x(), character.position.y() - progress * 3.0);
        character.transform->setMatrix(transform);
        const qreal left = -character.padding;
        const qreal right = character.advance + character.padding;
        const qreal split = progress <= 0.0 ? left : (progress >= 1.0 ? right : character.advance * progress);
        character.normalClip->setClipRect(QRectF(split, -character.padding,
                                                 right - split, character.height + character.padding * 2));
        character.playingClip->setClipRect(QRectF(left, -character.padding,
                                                  split - left, character.height + character.padding * 2));
        character.normalClip->markDirty(QSGNode::DirtyGeometry);
        character.playingClip->markDirty(QSGNode::DirtyGeometry);
    }
    return node;
}
