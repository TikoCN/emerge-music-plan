#ifndef LRCSCENEGRAPHITEM_H
#define LRCSCENEGRAPHITEM_H

#include <QColor>
#include <QFont>
#include <QQuickItem>
#include <QVariantList>
#include <QtQml/qqmlregistration.h>

class QSGNode;

/**
 * @brief 使用 Qt Quick 场景图绘制单行歌词。
 */
class LrcSceneGraphItem : public QQuickItem {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged FINAL)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged FINAL)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)
    Q_PROPERTY(QColor normalColor READ normalColor WRITE setNormalColor NOTIFY normalColorChanged FINAL)
    Q_PROPERTY(QVariantList words READ words WRITE setWords NOTIFY wordsChanged FINAL)
    Q_PROPERTY(QVariantList wordStarts READ wordStarts WRITE setWordStarts NOTIFY wordStartsChanged FINAL)
    Q_PROPERTY(QVariantList wordEnds READ wordEnds WRITE setWordEnds NOTIFY wordEndsChanged FINAL)
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged FINAL)
    Q_PROPERTY(bool playing READ playing WRITE setPlaying NOTIFY playingChanged FINAL)

public:
    /**
     * @brief 创建场景图歌词项。
     * @param parent 父项。
     */
    explicit LrcSceneGraphItem(QQuickItem *parent = nullptr);

    /** @brief 获取包含辅助歌词的完整显示文本。 */
    [[nodiscard]] QString text() const;
    /** @brief 设置包含辅助歌词的完整显示文本。 */
    void setText(const QString &text);
    /** @brief 获取歌词字体。 */
    [[nodiscard]] QFont font() const;
    /** @brief 设置歌词字体。 */
    void setFont(const QFont &font);
    /** @brief 获取已唱字词的颜色。 */
    [[nodiscard]] QColor color() const;
    /** @brief 设置已唱字词的颜色。 */
    void setColor(const QColor &color);
    /** @brief 获取未唱字词的颜色。 */
    [[nodiscard]] QColor normalColor() const;
    /** @brief 设置未唱字词的颜色。 */
    void setNormalColor(const QColor &color);
    /** @brief 获取逐词歌词。 */
    [[nodiscard]] QVariantList words() const;
    /** @brief 设置逐词歌词。 */
    void setWords(const QVariantList &words);
    /** @brief 获取逐词开始时间。 */
    [[nodiscard]] QVariantList wordStarts() const;
    /** @brief 设置逐词开始时间。 */
    void setWordStarts(const QVariantList &wordStarts);
    /** @brief 获取逐词结束时间。 */
    [[nodiscard]] QVariantList wordEnds() const;
    /** @brief 设置逐词结束时间。 */
    void setWordEnds(const QVariantList &wordEnds);
    /** @brief 获取播放器位置，单位为毫秒。 */
    [[nodiscard]] qint64 position() const;
    /** @brief 设置播放器位置，单位为毫秒。 */
    void setPosition(qint64 position);
    /** @brief 获取当前行是否正在播放。 */
    [[nodiscard]] bool playing() const;
    /** @brief 设置当前行播放状态。 */
    void setPlaying(bool playing);

signals:
    void textChanged();
    void fontChanged();
    void colorChanged();
    void normalColorChanged();
    void wordsChanged();
    void wordStartsChanged();
    void wordEndsChanged();
    void positionChanged();
    void playingChanged();

protected:
    /**
     * @brief 尺寸变化后重新测量文本并安排绘制。
     * @param newGeometry 新尺寸和位置。
     * @param oldGeometry 原尺寸和位置。
     * @override
     */
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    /**
     * @brief 在场景图渲染线程生成歌词文字节点。
     * @param oldNode 上一帧的场景图节点。
     * @param data 场景图节点更新信息。
     * @return 更新后的歌词节点。
     * @override
     */
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;

private:
    /** @brief 使用与绘制一致的排版规则更新隐式高度。 */
    void updateImplicitHeight();
    /** @brief 根据当前播放状态和时间更新高亮字词边界。 */
    void updateHighlightedWord();

    QString m_text;
    QFont m_font;
    QColor m_color = Qt::white;
    QColor m_normalColor = Qt::gray;
    QVariantList m_words;
    QVariantList m_wordStarts;
    QVariantList m_wordEnds;
    qint64 m_position = 0;
    bool m_playing = false;
    int m_highlightedWord = -1;
    qreal m_wordProgress = 0.0;
};

#endif
